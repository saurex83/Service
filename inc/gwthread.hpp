#include <boost/thread.hpp>

class GWThread{
	public:
		GWThread();
		~GWThread();
		void start();
		void stop();
	private:
		bool threadStarted;
		bool threadStopReq;
		boost::thread Thread;
		void gatewayThread();
		void threadActions();
	void tst();
};
