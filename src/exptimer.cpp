#include "exptimer.hpp"




using namespace boost::posix_time;

ExpTimer::ExpTimer(unsigned int sec){
	dur_time_sec = seconds(sec);
	Start();
};


void ExpTimer::Start(){
	start_time = second_clock::local_time();
};

bool ExpTimer::Expired(){
	ptime now = second_clock::local_time();
    time_duration diff = now - start_time;
	if (diff > dur_time_sec)
	   return true;
	return false;	
};
