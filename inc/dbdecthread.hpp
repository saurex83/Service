#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <mutex>

class DbDecThread{
	public:
		DbDecThread();
		~DbDecThread();
		void start();
		void stop();
		bool isRunnig();
		std::string thread_error;
	private:
		void decThread();
		bool threadException;
		bool threadStarted;
		bool threadStopReq;
		void threadActions();
		boost::thread Thread;
};
