#include "agrthread.hpp"
#include "debug.hpp"
#include <boost/chrono.hpp>

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

AgrThread::AgrThread(){
	this->threadStarted = false;
	this->threadStopReq = false;
	this->thread_error = "";
	this->threadException = false;
}

AgrThread::~AgrThread(){
}

bool AgrThread::isRunnig(){
	return this->threadStarted;
};

void AgrThread::start(){
	SPDLOG_INFO("Agregation thread prepared to start");
	this->threadStarted = false;
	this->threadStopReq = false;
	Thread = boost::thread(&AgrThread::agrThread, this);
}

void AgrThread::stop(){
	SPDLOG_INFO("Agregation thread prepared to stop");
	this->threadStopReq = true;
	this->Thread.join();
	this->threadStarted = false;
	SPDLOG_INFO("Agregation thread stoped");
}

void AgrThread::agrThread(){
	this->threadStarted = true;
	SPDLOG_INFO("Agregation thread started");
	
	while(!this->threadStopReq){
		try{
			threadActions();
		} CATCH_THREAD_EXC("threadActions");
	};
};


void AgrThread::threadActions(){
	boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
}
