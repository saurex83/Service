#pragma once
#include "boost/date_time/posix_time/posix_time.hpp"


class ExpTimer{
	public:
		ExpTimer(unsigned int sec);
		void Start();
		bool Expired();
	private:
		boost::posix_time::ptime start_time;
		boost::posix_time::time_duration dur_time_sec;
};
