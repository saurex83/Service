#include "SerialCom.hpp" 
#include "ServiceConfig.hpp"
#include "Transiver.hpp"
#include "debug.hpp"
#include <iostream>
//#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

#define ATYPE_PAR_ERR  0
#define ATYPE_CMD_ERR  1
#define ATYPE_CMD_OK  2

#define PAR_CRC16  1
#define PAR_NOCMD  2


#define CMD_LEN 1
#define CMD_ARG_VAL 2
#define CMD_SEEDING 3
#define CMD_NOSEEDING 4
#define CMD_TX_FULL 5
#define CMD_RX_EMPTY 6 


using namespace std;

/**
 * @brief Расчет кода crc16
 *
 * @param raw указатель на данные
 *
 * @return возвращает 2 байта crc16
 */
static unsigned short crc16(vector<unsigned char> raw){
    unsigned short crc = 0xFFFF;
    unsigned char i;
 	unsigned char *pcBlock = &raw.front();
	size_t len = raw.size();

    while (len--) {
        crc ^= *pcBlock++ << 8;
        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
};

/**
 * @brief Передача данных передатчику
 *
 * К данным добавляется размер и crc16
 * @param raw указатель на данные
 */
static void write_data(vector<unsigned char> raw){
	unsigned short crc = crc16(raw);
	//Увеличим длинну на 2 байта кода CRC16
	unsigned char len = raw.size() + 2; 
	raw.push_back(((unsigned char*)(&crc))[0]);
	raw.push_back(((unsigned char*)(&crc))[1]);
	raw.insert(raw.begin(), len);
	comm::write(raw);
};

/**
 * @brief Читает данные от трансивера
 *
 * Ответ должен соответствовать формату LEN, DATA, CRC16
 * Если ответ не верен, то вызывается исключение runtime_error
 * @param raw
 */
static void read_data(vector<unsigned char>& raw){
	comm::read_imp(raw);
	size_t read_len = raw.size();
//	SPDLOG_TRACE("READ LEN={}", raw.size());	
	// При чтении ответа может произойти две ситуации: трансивер завис 
	// и не отвечает, тогда длинна ответа равна 0, либо из-за 
	// буферицации ОС и микросхемы приемника ответ может прийти позже.
	// Первая ситуация это ошибка, вторая ситуация допустима. 
	// Для исключения второй ситуации время ожидания первого байта ответа в 
	// моудле SerialCom увеличено до 1000мс, последующих байт по 50мс.
	if (!read_len){
		SPDLOG_TRACE("No answer from transiver, size={}", read_len);
		throw(runtime_error("No answer from transiver."));
	};
	// Фактический размер на 1 байт больше указаного в пакете
	if (read_len != raw[0] + 1){
		SPDLOG_ERROR("Read data size is: {}. Byte LEN is {}",
			   	read_len, raw[0]);

		for (unsigned char it : raw)
			cout << unsigned(it) << " ";
		cout << endl;
		throw(runtime_error("Received size not match to specified"));
	};

	// Извлекаем crc16
	unsigned short crc = 0;
	vector<unsigned char>::iterator	it = raw.end();
	crc |= (unsigned short)(*it--) << 8;
	crc |= (unsigned short)(*it); 

	// Удаляем поля LEN и CRC16
	raw.erase(raw.begin());
	raw.pop_back();
	raw.pop_back();

// TODO Разобраться с crc16. Трансивер их не создает 
//	unsigned short crc_calc = crc16(raw);
//	if (crc_calc != crc){
//		SPDLOG_ERROR("CRC16 wrong");
//		throw(runtime_error("CRC16 wrong"));
//	};
};


/**
 * @brief Отправляет команду транисиверу и возвращает ответ
 *
 * @param cmd номер команды
 * @param args аргументы команды
 *
 * При ошибки выбрасывает исключение runtime_error
 * @return вектор с данными ответа
 */
static vector<unsigned char> 
	send_cmd(unsigned char cmd, vector<unsigned char> args){
	
	// Перед отправкой команды дадим время микроконтролеру
	// подготовиться к ее приему, после предыдущей команды
	boost::this_thread::sleep_for(boost::chrono::milliseconds(30));
	
	// Отправляем команду в формате CMD,ARGS
	
	args.insert(args.begin(), cmd);
	write_data(args);
// TODO Может добавить повторную отправку если была ошибка CRC16?
	// Принимает ответ
	
	vector<unsigned char> answer;
	read_data(answer);
	
	if (answer.size() == 0){
		SPDLOG_ERROR("Transiver answer has zero size");
		throw(runtime_error("Transiver answer has zero size"));
	};

	// Первый байт содержит тип ответа
	if (answer[0] == ATYPE_CMD_OK){
		answer.erase(answer.begin());
		return answer;
	};

#define ERR_MSG(msg){\
		SPDLOG_ERROR(msg);\
		throw runtime_error(msg);\
	};

	if (answer[0] == ATYPE_PAR_ERR){
		switch(answer[1]){
			case PAR_CRC16:
				ERR_MSG("Transiver read wrong crc16");
			case PAR_NOCMD:
				ERR_MSG("Transiver receive unknown cmd");
			default:
				ERR_MSG("Unknown ATYPE_PAR_ERR");
		};
	};

	if (answer[0] == ATYPE_CMD_ERR){
		switch(answer[1]){
			case CMD_LEN:
				ERR_MSG("CMD error. Incorrect args length");
			case CMD_ARG_VAL:
				ERR_MSG("CMD error. Incorrect args value");
			case CMD_SEEDING:
				ERR_MSG("CMD error. Network seed must be stoped");
			case CMD_NOSEEDING:
				ERR_MSG("CMD error. Network seed must be start");
			case CMD_TX_FULL:
				ERR_MSG("CMD error. TX buffer full");
			case CMD_RX_EMPTY:
				ERR_MSG("CMD error. RX buffer empty");
			default:
				ERR_MSG("Unknown ATYPE_CMD_ERR");
		};
	};

};

static void check_cmd_answer_len(vector<unsigned char>& answ, 
		unsigned char cmd,size_t exp_size){
	if (answ.size() == exp_size)
		return;
	SPDLOG_ERROR("CMD {}. Expected answer size is {}, but received {}",
			unsigned(cmd), to_string(exp_size), answ.size());
	throw runtime_error("Unexpected answer size");
};

/**
 * @brief Проверка состояния сети
 *
 * @return состояние сети
 */
bool Transiver::is_network_seed(){
	unsigned char cmd = 0x00;
	vector<unsigned char> args;
	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 1);
	return answ[0];	
};

/**
 * @brief Устанавка значения panid
 *
 * @param panid 
 */
void Transiver::set_panid(unsigned char panid){
	unsigned char cmd = 0x01;
	vector<unsigned char> args;
	args.push_back(panid);

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
};

void Transiver::set_tx_power(unsigned char power){
	unsigned char cmd = 0x10;
	vector<unsigned char> args;
	args.push_back(power);

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
}

/**
 * @brief Установка значения часов реального времени сети
 *
 * @param rtc
 */
void Transiver::set_rtc(uint32_t rtc){
	unsigned char cmd = 0x02;
	vector<unsigned char> args;
	args.push_back(((unsigned char*)&rtc)[0]);
	args.push_back(((unsigned char*)&rtc)[1]);
	args.push_back(((unsigned char*)&rtc)[2]);
	args.push_back(((unsigned char*)&rtc)[3]);

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
};

/**
 * @brief Изменение состояние транисивера
 *
 * @param status true-трансивер включен
 */
void Transiver::network_seed(bool status){
	unsigned char cmd = 0x03;
	vector<unsigned char> args;
	args.push_back((unsigned char)status);

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
};

/**
 * @brief Загрузка вектора инициализации потокового шифрования сети
 *
 * @param iv Вектор из 16 байт
 */
void Transiver::load_streem_iv(vector<unsigned char>& iv){
	if (iv.size() != 16){
		SPDLOG_ERROR("Stream IV size is :{}, not 16", iv.size());
		throw runtime_error("Stream IV size is not 16");
	};

	unsigned char cmd = 0x04;
	vector<unsigned char> args;
	args = iv;
	
	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
};

/**
 * @brief Загрузка ключа сети потокового шифрования
 *
 * @param key Вектор из 16 байт
 */
void Transiver::load_streem_key(vector<unsigned char>& key){
	if (key.size() != 16){
		SPDLOG_ERROR("Stream KEY size is :{}, not 16", key.size());
		throw runtime_error("Stream KEY size is not 16");
	};
	
	unsigned char cmd = 0x05;
	vector<unsigned char> args;
	args = key;

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
};

/**
 * @brief Открытие слота приема пакетов трансивера
 *
 * @param ts номер временого слота
 * @param ch номер частотного канала
 */
void Transiver::open_slot(unsigned char ts, unsigned char ch){
	unsigned char cmd = 0x07;
	vector<unsigned char> args;
	args.push_back(ts);
	args.push_back(ch);

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
};

/**
 * @brief Закрытие временого слота
 *
 * @param ts номер временого слота
 */
void Transiver::close_slot(unsigned char ts){
	unsigned char cmd = 0x08;
	vector<unsigned char> args;
	args.push_back(ts);

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
};


/**
 * @brief Количество принятых пакетоа
 *
 * @return 
 */
int Transiver::rx_frames(){
	//SPDLOG_TRACE("Read RX frames");
	unsigned char cmd = 0x09;
	vector<unsigned char> args;

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 1);

	return answ[0];	
};


/**
 * @brief Количество пакетов в очеренди на передачу
 *
 * @return 
 */
int Transiver::tx_frames(){
	//SPDLOG_TRACE("Read TX frames");
	unsigned char cmd = 0x0A;
	vector<unsigned char> args;

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 1);

	return answ[0];	
};


/**
 * @brief Добавить пакет в очередь на передачу
 *
 * @param frame указатель на пакет
 */
void Transiver::push_tx(Frame& frame){
	unsigned char cmd = 0x0B;
	vector<unsigned char> args;
	frame.convertRaw(args);

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 0);
};

/**
 * @brief Извлеч пакет из приемного буфера трансивера
 *
 * @param frame указатель на пакет
 */
void Transiver::pop_rx(Frame& frame){
	//SPDLOG_TRACE("POP RX frames");
	unsigned char cmd = 0x0C;
	vector<unsigned char> args;

	vector<unsigned char> answ = send_cmd(cmd, args);
//	check_cmd_answer_len(answ, cmd, 0);

	frame.parseRaw(answ);
};

/**
 * @brief Считать данные об энергии в канале
 *
 * После считывания, трансивер сбрасывает значения на минимальное значение 
 * в дБм. По мере сканирования радиоэфира, значения заполняются пиковыми
 * значениями энергии в канале.
 * @param energy
 */
void Transiver::read_energy(vector<signed char>& energy){
	energy.clear();
	
	unsigned char cmd = 0x11;
	vector<unsigned char> args;

	vector<unsigned char> answ = send_cmd(cmd, args);
	check_cmd_answer_len(answ, cmd, 18);
	
	for ( unsigned char it : answ)
		energy.push_back((signed char)it);			
};

