#include "dbdecthread.hpp"
#include "debug.hpp"

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

DbDecThread::DbDecThread(){
	this->threadStarted = false;
	this->threadStopReq = false;
	this->thread_error = "";
	this->threadException = false;
}

DbDecThread::~DbDecThread(){
}


bool DbDecThread::isRunnig(){
	return this->threadStarted;
};

void DbDecThread::start(){
	SPDLOG_INFO("Database decimation thread prepared to start");
	this->threadStarted = false;
	this->threadStopReq = false;
	Thread = boost::thread(&DbDecThread::decThread, this);
}

void DbDecThread::stop(){
	SPDLOG_INFO("Database decimation thread prepared to stop");
	this->threadStopReq = true;
	this->Thread.join();
	this->threadStarted = false;
	SPDLOG_INFO("Database decimation thread stoped");
}

void DbDecThread::decThread(){
	this->threadStarted = true;
	SPDLOG_INFO("Database decimacion thread started");
	
	while(!this->threadStopReq){
		try{
			threadActions();
		} CATCH_THREAD_EXC("threadActions");
	};
};


void DbDecThread::threadActions(){
}
