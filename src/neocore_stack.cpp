#include "neocore_stack.hpp"
#include "database.hpp"
#include <sstream>
#include  <iomanip>
#include "exptimer.hpp"
#include "route_table.hpp"

namespace neocore{


static vector<Frame> *tx_pool;
static vector<Frame> *rx_pool;
static RouteTable RT;

struct {
	unsigned char pan_id;
	unsigned char sys_channel;
} MODEL;

/**
 * @brief Инициализация модуля
 *
 * @param tx указатель на вектор пакетов для передачи
 * @param rx указатель на вектор принятых пакетов
 */
void init(vector<Frame> *tx, vector<Frame> *rx){
	tx_pool = tx;
	rx_pool = rx;
	
	DataBase db;

	bool db_res = true;
	db_res &= db.get_SYS_CHANNEL(MODEL.sys_channel);
	db_res &= db.get_PAN_ID(MODEL.pan_id);

	if (db_res)
		SPDLOG_TRACE("MODEL data loaded");
	else
		SPDLOG_ERROR("MODEL data not loaded");

	RT.Clear();
};


/**
 * @brief Обрабатывает принятые пакеты согласно правилам протокола
 */
void process(){
	// Обработка принятых пакетов
	eth_processRxPool();
	
	// Выделение времени протоколам
	NP_TimeAlloc();
	
	// Проверям таблицу маршрутов
	RT.analyseTable();
};


/**
 * @brief Разбирает очередь принятых пакетов
 */
static void eth_processRxPool(){
	if (!rx_pool->size())
		return;

	// TODO доработать алгоритм. с удалением беда
	// Переберем все принятые пакеты
	for (vector<Frame>::iterator frame = rx_pool->begin();
			frame != rx_pool->end(); frame++){
	
		// Пропустим пакет через фильтр. Если пакет не соответсвует фильтру
		// удалим его и завершим анализ, так как цикл перебора нужно 
		// начать заново иначе ошибка.
		if (!eth_frame_filter(*frame)){
			rx_pool->erase(frame);
			return;
		};
		// Заполним метаданные пакета на основе заголовка
		eth_fill_metadata(*frame);
		// Отрежим заголовок
		frame->delHeader(ETH_LAY_SIZE);
		// Отдаем принятый пакет вышестоящему уровню
		RP_Receive(*frame);
		// Удаляем после обработки
		rx_pool->erase(frame);
		return;
	};
};

//********************************************
// Протокол IP
//********************************************

static bool ip_frame_filter(Frame& frame){ 
	if (frame.size() < sizeof(struct IP_H)){
		SPDLOG_TRACE("Filtered by size= {}", frame.size());
		return false;
	};
	return true;
};

static void ip_fill_meta(Frame& frame){
	struct IP_H& iph = *(struct IP_H*)&frame.payload.front();
	frame.meta.meta.ETX = iph.ETX;
	frame.meta.meta.FDST = iph.FDST;
	frame.meta.meta.FSRC = iph.FSRC;
	frame.meta.meta.IPP = iph.IPP;
};

static void IPP_Process(Frame& frame){
	switch(frame.meta.meta.IPP){
		case IPP_UDP:
			SPDLOG_TRACE("IP is UDP");
			UDP_Recive(frame);
			break;
		case IPP_TCP:
			SPDLOG_TRACE("IP is TCP");
			TCP_Recive(frame);
			break;    
		case IPP_AUTH:
			SPDLOG_TRACE("IP is AUTH");
			AUTH_IP_Recive(frame);
			break;  
		default:
			SPDLOG_TRACE("Unrecognized IPP");
  };
};

static void IP_Receive(Frame& frame){
	SPDLOG_TRACE("Received IP frame");
	
	if (!ip_frame_filter(frame))
		return;

	ip_fill_meta(frame);


	if (frame.meta.meta.FDST != 0){
		SPDLOG_TRACE("IP frame FDST not for gateway");
		return;
	}

	// Добавляем новый маршрут
	RT.addRoute(frame.meta.meta.NSRC, frame.meta.meta.FSRC, 
			frame.meta.meta.NSRC_TS, frame.meta.meta.NSRC_CH);

	// Обрабатываем пакеты только для шлюза
	IPP_Process(frame);

};

static void IP_Send(Frame& frame){
	unsigned short int fsrc = frame.meta.meta.FDST;
   	unsigned short int nsrc;
	unsigned char ts;
   	unsigned char ch;

	bool res = RT.getRoute(fsrc, &nsrc, &ts, &ch);	
	if (!res){
		SPDLOG_TRACE("No route to {}", fsrc);
	};

	struct IP_H iph;
	iph.ETX = 0;
	iph.FDST = frame.meta.meta.FDST;
	iph.FSRC = 0;
	iph.IPP = frame.meta.meta.IPP;

	frame.addHeader((unsigned char*)&iph, sizeof(IP_H));
	frame.meta.meta.PID = PID_IP;
	
	//Выбор через протокол ROUTE
	frame.meta.meta.CH = ch;
	frame.meta.meta.TS = ts;
	frame.meta.meta.NDST = nsrc;
		
	eth_send(frame);   
};

//********************************************
// Протокол UDP
//********************************************
static void UDP_Recive(Frame& frame){
	SPDLOG_TRACE("UDP recived");
};

//********************************************
// Протокол TCP
//********************************************
static void TCP_Recive(Frame& frame){
	SPDLOG_TRACE("TCP recived");
};

//********************************************
// Протокол AUTH_IP
//********************************************
static void AUTH_IP_Recive(Frame& frame){
	SPDLOG_TRACE("AUTH_IP received");
};

//********************************************
// Протокол NEIGHBORS
//********************************************
static void NP_Receive(Frame& frame){
	SPDLOG_TRACE("Received NP frame card. Gateway dont use neighbor table.");
};

static void NP_TimeAlloc(){
	NP_Periodic_Send_GWCard();
};

/**
 * @brief Создает и отправляет карту шлюза по протоколу соседей
 */
static void NP_Send_gw_card(){   
	struct NP_CARD card;
	// Не имеет значения чем заполним. Главное что бы проходили фильтры
	card.TS = 5; 
	card.CH = 11;
	// У шлюза ETX всегда равен 0
	card.ETX = 0;
  
	Frame fr;
	fr.addHeader((unsigned char*)&card, sizeof(struct NP_CARD));

	vector<unsigned char> cmd_vec = {NP_CMD_CARD};
	fr.addHeader(cmd_vec);

	fr.meta.meta.TS = SYS_TS;
	fr.meta.meta.CH = MODEL.sys_channel;
	fr.meta.meta.PID = PID_NP;
	fr.meta.meta.NDST = 0xffff;
	fr.meta.meta.NSRC = 0;

	eth_send(fr);

  	SPDLOG_TRACE("NP gateway card sended");
};

/**
 * @brief Периодическая передача карточки шлюза
 */
static void NP_Periodic_Send_GWCard(){
	static ExpTimer timer(60);
	if (!timer.Expired())
		return;

	NP_Send_gw_card();
	timer.Start();	
}

//********************************************
// Протокол ROUTE
//********************************************
static void RP_Receive(Frame& frame){
	// TODO Route table
	switch (frame.meta.meta.PID){
  		case PID_IP:
    		IP_Receive(frame);
    		break;
  		case PID_NP:
    		NP_Receive(frame);
    		break;
		case PID_AUTH:
			AUTH_ETH_Receive(frame);
			break;
  		default:
			SPDLOG_ERROR("Frame PID {} handler not found",
					frame.meta.meta.PID);
    		return; // Пакет удалит ethernet
  	};

//  	TB_Receive(frame);
};


//********************************************
// Протокол AUTH_ETH
//********************************************
static void AUTH_ETH_Receive(Frame& frame){
	if (!frame.size()){
		SPDLOG_TRACE("Filtered by size");
		return;
	}
	// Кадр передан в системном слоте не принимаем
	if (frame.meta.meta.TS == SYS_TS){
		SPDLOG_TRACE("Filtered by sys_channel");
		return;
	}
	// Адресс отправителя значения не имеет так как у узла нет адреса
	
	// Смотрим что за пакет
	unsigned char cmd = frame.payload[0];
	frame.delHeader(1);

	if (cmd != AUTH_CMD_REQ){
		SPDLOG_TRACE("AUTH_ETH CMD = {}. Drop frame", cmd);
		return;
	}

	if (frame.size() != sizeof(struct AUTH_ETH_REQ)){
		SPDLOG_TRACE("Received AUTH_ETH_REQ size is {}, expected {} ",
				frame.size(), sizeof(struct AUTH_ETH_REQ));
		return;
	};

	struct AUTH_ETH_REQ& req =*(AUTH_ETH_REQ*)&frame.payload.front();		

	// Узнаем из базы информацию об узле
	DataBase db;
	std::vector<unsigned char> mac;
	std::stringstream stream;
	std::string mac_str;
	
	for (size_t i = 0; i < 8; i++){
		mac.push_back(req.mac[i]);
		stream << std::setfill('0') << std::setw(2) << std::hex << 
			unsigned(req.mac[i]);
	};
	mac_str = stream.str();
	
	int ipaddr;
	std::string comment;
	std::string location;
	std::string name;
	bool search_res;
	bool res =db.get_NODELIST_by_MAC(mac, ipaddr, name, comment, location,
		   	search_res);

	if (!res)
		throw(std::runtime_error("Error acces to NODELIST"));

	if (!search_res){
		SPDLOG_WARN("Access mac {} depricated", mac_str);
		return;
	};

	SPDLOG_TRACE(
		"Access mac:{}, name:{}, IP:{}, NODE_TYPE:{}, NODE_VER:{}  granted",
		mac_str, name, ipaddr, req.NODE_TYPE, req.NODE_VER);

	//TODO MEASURE_POINT Разобрать sensor_types, channels

	// Нужно дать ответ
	Frame ans_frame;
	struct AUTH_ETH_RESP resp;

	for (int i = 0; i < 8; i++)
		resp.mac[i] = req.mac[i];
	
	if (search_res){
		resp.access = true;
		resp.ipaddr = ipaddr;
		//TODO params set
	} else {
		resp.access = false;
		resp.ipaddr = 0;
	};
	
	
	ans_frame.addHeader((unsigned char*)&resp, sizeof(struct AUTH_ETH_RESP));

	vector<unsigned char> cmd_vec = {AUTH_CMD_RESP};
	ans_frame.addHeader(cmd_vec);

	ans_frame.meta.meta.TS = SYS_TS;
	ans_frame.meta.meta.CH = MODEL.sys_channel;
	ans_frame.meta.meta.PID = PID_AUTH;
	ans_frame.meta.meta.NDST = 0xffff;
	ans_frame.meta.meta.NSRC = 0;

	eth_send(ans_frame);
};

//********************************************
// Протокол ETH
//********************************************

/**
 * @brief Вычисление значения XOR для ETH заголовка
 *
 * @param eth указатель на заголовок
 *
 * @return значение XOR
 */
static unsigned char calc_xor(struct ETH_LAY *eth){
  unsigned char xor_val = 0x57;
  unsigned char *ptr = (unsigned char*)eth;
  // Ксорим все кроме последнего байта структуы. он XOR
  for (int i = 0; i < sizeof(struct ETH_LAY) - 1; i++)
    xor_val ^=ptr[i]; 
  return xor_val;
};

/**
 * @brief Добавление заголовка ETH и постановка пакета в очередь на отправку
 *
 * @param frame указатель на пакет
 */
static void eth_send(Frame& frame){
	struct ETH_LAY eth_header;
	eth_header.ETH_T.bits.PID = frame.meta.meta.PID ;
	eth_header.ETH_T.bits.ETH_VER =HEADER_ETH_VER;
	eth_header.NETID = MODEL.pan_id;
	eth_header.NDST = frame.meta.meta.NDST;
	eth_header.NSRC = frame.meta.meta.NSRC;
	eth_header.NSRC_TS = 0xff;
	eth_header.NSRC_CH = 0xff;
 	eth_header.XOR = calc_xor(&eth_header); 
	frame.addHeader((unsigned char*)&eth_header, sizeof(struct ETH_LAY));
 
 	frame.meta.meta.tx_attempts = 5;
  //	SPDLOG_TRACE("Frame params. LEN:{}, ETH_VER:{}, PID:{}", frame.size(),
	//		eth_header.ETH_T.bits.ETH_VER,eth_header.ETH_T.bits.PID); 
	tx_pool->push_back(frame);	
};

static void eth_fill_metadata(Frame& frame){
	// Заполняем метаданные
	struct ETH_LAY& eth_header = *((struct ETH_LAY*)&frame.payload.front());
	frame.meta.meta.NDST = eth_header.NDST;
	frame.meta.meta.NSRC = eth_header.NSRC;
	frame.meta.meta.PID = eth_header.ETH_T.bits.PID;
   	frame.meta.meta.NSRC_TS = eth_header.NSRC_TS;	
   	frame.meta.meta.NSRC_CH = eth_header.NSRC_CH;	
	//SPDLOG_TRACE("ETH header: NDST = {}, NSRC = {}, PID = {}", 
	//		eth_header.NDST, eth_header.NSRC, eth_header.ETH_T.bits.PID);
};

static bool eth_frame_filter(Frame& frame){
	struct ETH_LAY& eth_header = *((struct ETH_LAY*)&frame.payload.front());

	// Фильтр 1: по размеру кадра
	if (frame.size() < ETH_LAY_SIZE){
 		//SPDLOG_TRACE("Frame filtered by size");
		return false;
	};
  
	// Фильтр 2: по XOR
	unsigned char xor_val = calc_xor(&eth_header);
	if (xor_val != eth_header.XOR){
		//SPDLOG_TRACE("Filtered XOR");
		return false;
	};
  	
  	// Фильтр 3: по версии протокола
	if (eth_header.ETH_T.bits.ETH_VER != HEADER_ETH_VER){
 		//SPDLOG_TRACE("Frame filtered by eth protocol version");
    	return false;
	};

  	// Фильтр 4: по идентификатору сети
	if (eth_header.NETID!= MODEL.pan_id){
 		//SPDLOG_TRACE("Frame filtered by netid");
    	return false;
	};

  	// Фильтр 5: по адресу получателю
  	if (eth_header.NDST != 0xffff )
    	if (eth_header.NDST != 0x0000){
 			SPDLOG_TRACE("Frame NDST address field");
      		return false;
		};
  
	// Фильтр 6: по каналу отправителя
  	if (eth_header.NSRC_CH  < 11 || eth_header.NSRC_CH  > 28){
    	SPDLOG_TRACE("Filtered NSRC_CH");
    	return false;
  	};

  	// Фильтр 7: по таймслоту отправителя
  	if (eth_header.NSRC_TS  < 1 || eth_header.NSRC_TS  > 49){
    	SPDLOG_TRACE("Filtered NSRC_TS");
    	return false;
  	};

	return true;
};
};
