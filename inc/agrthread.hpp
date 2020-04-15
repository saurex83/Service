#pragma once
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <mutex>
#include <string>
#include <Poco/Dynamic/Var.h>
#include <Poco/Exception.h>
#include <mutex>

enum dataT_e {Temp, Hum};

class DRecord{
public:
	std::string mac;
	dataT_e data_type;
	unsigned int channel;
	Poco::Dynamic::Var value;
	bool registrated;
};

class AgrThread{
	public:
		AgrThread();
		~AgrThread();
		void start();
		void stop();
		bool isRunnig();
		std::string thread_error;
		void pushData(DRecord drec);
	private:
		void DataRecordsRegistrate(); 
		void DataRecordsClear();
		std::vector<DRecord> DataRecordList;
		std::mutex mutexDataRecordList;
		void agrThread();
		bool threadException;
		bool threadStarted;
		bool threadStopReq;
		void threadActions();
		boost::thread Thread;
};
