#pragma once
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <mutex>
#include "agrthread.hpp"
#include "frame.hpp"

class GWTask{
	public:
		int cmd;
		int params;
		bool registrated;
};

class GWThread{
	public:
		GWThread(AgrThread* agr);
		~GWThread();
		void start();
		void stop();
		bool isRunnig();
		std::string thread_error;
		void addGWTask(GWTask& task);
	private:
		AgrThread* agrthread;
		std::mutex mutexTask;
		std::vector<GWTask> taskPool;
		void taskRegistrate();
		void taskClear();
		bool threadException;
		bool threadStarted;
		bool threadStopReq;
		boost::thread Thread;
		void gatewayThread();
		void threadActions();
		void transiverInit();
		void transiverDeInit();
		void clearPools();
		void serveRXPool();
		void serveTXPool();
	
		std::vector<Frame> tx_pool;
		std::vector<Frame> rx_pool;
};
