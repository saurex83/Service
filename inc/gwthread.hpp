#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <mutex>

class GWTask{
	public:
		int cmd;
		int params;
		bool registrated;
};

class GWThread{
	public:
		GWThread();
		~GWThread();
		void start();
		void stop();
		bool isRunnig();
		std::string thread_error;
		void addGWTask(GWTask& task);
	private:
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
};
