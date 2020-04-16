#include "gwthread.hpp"
#include <boost/chrono.hpp>
#include <iostream>
#include "debug.hpp"
#include <exception>
#include "Transiver.hpp"
#include "database.hpp"
#include "valuechecker.hpp"

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
	
	// TODO перегрузить черкер на вектор.
	// Проверить что числа хорошо распределены.
	//	ch_res = checker::encrypt_iv_key();

	// Теперь инициализируем передатчик
	set_panid(pan_id);
	load_streem_iv(&stream_iv.front());
	load_streem_key(&stream_key.front());
	//set_rtc(..)
	
	clearPools();
	network_seed(true);
	open_slot(0x01, sys_channel);

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

void GWThread::addGWTask(GWTask& task){
	this->mutexTask.lock();
	this->taskPool.push_back(task);
	this->mutexTask.unlock();
};	

void GWThread::clearPools(){
	tx_pool.clear();
	rx_pool.clear();
};


void GWThread::serveRXPool(){
	int rx_cnt = rx_frames();
	if (!rx_cnt)
		return;
	// Ограничем количество забираемых пакетов за 1 раз
	if (rx_cnt > 5) 
		rx_cnt = 5;

	Frame frame;
	while (rx_cnt > 0 ){
		pop_rx(frame);
		rx_pool.push_back(frame);
		rx_cnt--;
	};
};

void GWThread::serveTXPool(){
};

void GWThread::threadActions(){
	boost::this_thread::sleep_for(boost::chrono::milliseconds(10000));
	throw (std::runtime_error("This is test exception!! Not a problem!"));
}


