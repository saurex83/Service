#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <mutex>

class AgrThread{
	public:
		AgrThread();
		~AgrThread();
		void start();
		void stop();
		bool isRunnig();
		std::string thread_error;
	private:
		void agrThread();
		bool threadException;
		bool threadStarted;
		bool threadStopReq;
		void threadActions();
		boost::thread Thread;
};
