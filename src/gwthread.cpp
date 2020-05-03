#include "gwthread.hpp"
#include <boost/chrono.hpp>
#include <iostream>
#include "debug.hpp"
#include <exception>
#include "Transiver.hpp"
#include "database.hpp"
#include "valuechecker.hpp"
#include "neocore_stack.hpp"

using namespace Transiver;

#define CATCH_THREAD_EXC(name) \
		catch(std::exception &e){\
			this->threadStopReq = true;\
			this->thread_error = e.what();\
			this->threadException = true;\
			this->threadStarted = false;\
			SPDLOG_ERROR("Catched excepton in {}: {}", name, e.what());\
		}\
		catch(...){\
			this->threadStopReq = true;\
			this->thread_error = "Unhandeled exception";\
			this->threadException = true;\
			this->threadStarted = false;\
			SPDLOG_ERROR("Catched unhandled exception in {}", name);\
		};


GWThread::GWThread(AgrThread* agr){
	this->threadStarted = false;
	this->threadStopReq = false;
	this->thread_error = "";
	this->threadException = false;
	this->agrthread = agr;
}

GWThread::~GWThread(){
	if (this->threadStarted)
		stop();
}
bool GWThread::isRunnig(){
	return this->threadStarted;
};

void GWThread::start(){
	SPDLOG_INFO("Gateway thread prepared to start");
	this->taskPool.clear();
	this->threadStarted = false;
	this->threadStopReq = false;
	this->thread_error = "";
	Thread = boost::thread(&GWThread::gatewayThread, this);
}

void GWThread::stop(){
	SPDLOG_INFO("Gateway thread prepared to stop");
	this->threadStopReq = true;
	this->Thread.join();
	this->threadStarted = false;
	this->taskPool.clear();
	SPDLOG_INFO("Gateway thread stoped");
}

void GWThread::gatewayThread(){
	this->threadStarted = true;
	SPDLOG_INFO("Gateway thread started");

	neocore::init(&tx_pool, &rx_pool);	

	try{
		transiverInit();
	} CATCH_THREAD_EXC("transiverInit");

	while(!this->threadStopReq){
		taskRegistrate(); // Пометим задачи принятые к исполнению
		try{
			threadActions();
		} CATCH_THREAD_EXC("threadActions");
		// Удалим помеченные задачи.Возможно для нее не обработчик
		taskClear(); 
	};
	
	try{
		transiverDeInit();
	} CATCH_THREAD_EXC("transiverDeInit");
};

void GWThread::transiverInit(){
	// Предварительная настройка трансивера
	
	// Остановим раздачу сети если она включена
	if (is_network_seed()){
		network_seed(false);
		SPDLOG_INFO("Transiver was seeding. Seed stoped.");
	};

#define CHK_BD(msg) \
		if (!db_res){\
			SPDLOG_ERROR("Data load from database failed. {}", msg);\
			throw(std::runtime_error(msg));\
		}	

#define CHK_VAL() \
	if (ch_res.error){\
		SPDLOG_ERROR("{}", ch_res.msg.c_str());\
		throw(std::runtime_error(ch_res.msg));\
	}

	// Загрузим все парметры из базы данных
	DataBase db;
	bool db_res;
	unsigned char sys_channel;
	unsigned char sync_channel;
	unsigned char pan_id;
	unsigned char tx_power;
	std::vector<unsigned char> stream_iv;
	std::vector<unsigned char> stream_key;
	std::vector<unsigned char> data_iv;
	std::vector<unsigned char> data_key;

	db_res = db.get_SYS_CHANNEL(sys_channel);
	CHK_BD("SYS_CHANNEL");
	db_res = db.get_SYNC_CHANNEL(sync_channel);
	CHK_BD("SYNC_CHANNEL");
	db_res = db.get_PAN_ID(pan_id);
	CHK_BD("PAN_ID");
	db_res = db.get_TX_POWER(tx_power);
	CHK_BD("TX_POWER");
	db_res = db.get_STREAM_IV(stream_iv);
	CHK_BD("STREAM_IV");
	db_res = db.get_STREAM_KEY(stream_key);
	CHK_BD("STREAM_KEY");
	db_res = db.get_DATA_IV(data_iv);
	CHK_BD("DATA_IV");
	db_res = db.get_DATA_KEY(data_key);
	CHK_BD("DATA_KEY");
// TODO не все параметры реализованны. Нужно еще tx_power

	//Нужно проверить значения параметров
	checker::CheckRes ch_res;
	ch_res = checker::pan_id(pan_id);
	CHK_VAL();
	ch_res = checker::sync_channel(sync_channel);
	CHK_VAL();
	ch_res = checker::sys_channel(sys_channel);
	CHK_VAL();
	ch_res = checker::tx_power(tx_power);
	CHK_VAL();
	
	// TODO перегрузить черкер на вектор.
	// Проверить что числа хорошо распределены.
	//	ch_res = checker::encrypt_iv_key();

	// Теперь инициализируем передатчик
	set_panid(pan_id);
	set_tx_power(tx_power);
	load_streem_iv(stream_iv);
	load_streem_key(stream_key);
	//set_rtc(..)
	
	clearPools();
	network_seed(true);
	open_slot(0x01, sys_channel);

	// Шлюз имеет предопределенные частотные каналы и временые слоты
	// для связи. {временой слот, канал}. Узлы используют такуюже таблицу 
	std::vector<pair<unsigned char, unsigned char>> gw_slots = {
  		{49, 11}, {48, 13}, {47, 15}, {46, 17}, {45, 19},
  		{44, 20}, {43, 21}, {42, 22}, {41, 23}, {40, 25}
	};

	for (auto it : gw_slots)
		open_slot(it.first, it.second);

	SPDLOG_INFO("Transiver inited");
}

void GWThread::taskRegistrate(){
	this->mutexTask.lock();
	for( GWTask task : this->taskPool)
		task.registrated = true;
	this->mutexTask.unlock();
}
void GWThread::taskClear(){
	this->mutexTask.lock();
	for(std::vector<GWTask>::iterator task = this->taskPool.begin();
		task != this->taskPool.end();
		task++){
		if(task->registrated)
			this->taskPool.erase(task);
	}
	this->mutexTask.unlock();
}

void GWThread::transiverDeInit(){
	if (Transiver::is_network_seed())
		Transiver::network_seed(false);
	SPDLOG_INFO("Transiver deinited");
	clearPools();
}

/**
 * @brief Добавляет задачу потока GWThread
 * 
 * @param task указатель на задачу
 */
void GWThread::addGWTask(GWTask& task){
	this->mutexTask.lock();
	this->taskPool.push_back(task);
	this->mutexTask.unlock();
};	

void GWThread::clearPools(){
	tx_pool.clear();
	rx_pool.clear();
};


/**
 * @brief Сбор принятых пакетов трансивером
 *
 * За один раз забирается ограниченное количество пакетов
 */
void GWThread::serveRXPool(){
	#define MAX_RX_FRAME 5
	int rx_cnt = rx_frames();
	int rx_cnt_save = rx_cnt;

	if (!rx_cnt)
		return;
	// Ограничем количество забираемых пакетов за 1 раз
	if (rx_cnt > MAX_RX_FRAME) 
		rx_cnt = MAX_RX_FRAME;

	Frame frame;
	while (rx_cnt > 0 ){
		boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
		pop_rx(frame);
		rx_pool.push_back(frame);
		rx_cnt--;
	};

	if (!rx_cnt_save)
		SPDLOG_TRACE("Received {} frames from transiver", rx_cnt_save);
};

/**
 * @brief Выбирает пакеты из очереди tx_pool и передает из трансиверу.
 *
 * Перед отправкой пакета трансиверу, производится проверка количества
 * пакетов, содержащихся в очереди.
 */
void GWThread::serveTXPool(){
	#define MAX_TX_FRAME 5 
	int tx_cnt = tx_frames();
	if (tx_cnt > MAX_TX_FRAME)
		return;

	if (!tx_pool.size())
		return;

	push_tx(tx_pool[0]);
	tx_pool.erase(tx_pool.begin());

	//SPDLOG_TRACE("Frame sended");
};

/**
 * @brief Считывает значения энергий по каналам из трансивера
 *
 * Значения сохраняет в базу данных. Период чтения фиксирован
 */
void GWThread::energyScan(){
	#define ENERGY_SCAN_INTERVAL (duration<double>)300
	using namespace boost::chrono;
	static system_clock::time_point last_pool = system_clock::now();
	
	// Проверяем интервал
	duration<double> sec_dur = system_clock::now() - last_pool;
	if (sec_dur < ENERGY_SCAN_INTERVAL)
		return;

	last_pool = system_clock::now();

	vector<signed char> energy;	
	Transiver::read_energy(energy);

	// Проверим что все значения корректны
	for (signed char it : energy)
		if (it == -127)
			return;

	DataBase db;
	db.set_ENERGYSCAN(energy);
	SPDLOG_TRACE("Energy scan complited");
};

/**
 * @brief Главный процедура потока
 *
 * Она обеспечивает обмен данными с трансивером и функционирование протокола
 */
void GWThread::threadActions(){
	serveRXPool();
	serveTXPool();
	neocore::process();
	energyScan();
	// energyScan() опрашивает трансивер.
	// neocoreStack(tx_pool, rx_pool); Разбор пакетов
	// dataCollection() - сбор данных. нет зависимостей
	// routeRibs() - сбор данных. нет зависимостей
	// statCollection() сбор данных. нет зависимостей
	// Авторизация - есть зависимость. Нужно отправлять ответы
	// Соседи - 
	boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
}


