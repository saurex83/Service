#include "gwthread.hpp"
#include <boost/chrono.hpp>
#include <iostream>
#include "debug.hpp"
#include <exception>

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
	// Выключение раздачи сети
	//throw("TRANS DEINIT EXC");
	SPDLOG_INFO("Transiver deinited");
}

void GWThread::addGWTask(GWTask& task){
	this->mutexTask.lock();
	this->taskPool.push_back(task);
	this->mutexTask.unlock();
};	

void GWThread::threadActions(){
	boost::this_thread::sleep_for(boost::chrono::milliseconds(10000));
	throw (std::runtime_error("This is test exception!! Not a problem!"));
}


