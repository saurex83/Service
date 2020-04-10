#include "debug.hpp"
#include "gwthread.hpp"
#include "httpserver.hpp"
#include "agrthread.hpp"
#include "dbdecthread.hpp"
#include "sysconfig.hpp"

void initialisation();

int main(int, char*[])
{
	initialisation();
   	return 0;
}

void initialisation(){
	debugInit();
	try {
		sysConfig();
	}
	catch(std::runtime_error& e){
		SPDLOG_INFO("sysConfig failed with msg :{}", e.what());
	};
}


