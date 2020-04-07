#include "gwthread.hpp"
#include <boost/chrono.hpp>
#include <iostream>
#include "debug.hpp"

GWThread::GWThread(){
	this->threadStarted = false;
	this->threadStopReq = false;
}

GWThread::~GWThread(){
}

void GWThread::start(){
	SPDLOG_INFO("Gateway thread prepared to start");
	this->threadStarted = false;
	this->threadStopReq = false;
	Thread = boost::thread(&GWThread::tst, this);
	boost::this_thread::sleep_for(boost::chrono::seconds{10});
}
void GWThread::tst(){
	SPDLOG_INFO("TST Gateway thread started");
};

void GWThread::stop(){
	SPDLOG_INFO("Gateway thread prepared to stop");
	this->threadStopReq = true;
	this->Thread.join();
	this->threadStarted = false;
	SPDLOG_INFO("Gateway thread stoped");
}

void GWThread::gatewayThread(){
//	this->threadStarted = true;
	std::cout<<"hello";
	SPDLOG_INFO("Gateway thread started");
	while(!this->threadStopReq)
		threadActions();
};

void GWThread::threadActions(){
	boost::this_thread::sleep_for(boost::chrono::seconds{2});
	SPDLOG_INFO("Hello from thread!");
}

