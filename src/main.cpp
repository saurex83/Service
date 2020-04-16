#include "debug.hpp"
#include "gwthread.hpp"
#include "httpserver.hpp"
#include "agrthread.hpp"
#include "dbdecthread.hpp"
#include "sysconfig.hpp"
#include <boost/chrono.hpp>
#include "license.hpp"

void initialisation();

void TESTER(){
	License& lic = License::instance();
	int max_wnodes;
	bool res = lic.getLicParam(std::string("max_wnodes"), max_wnodes);
	SPDLOG_INFO("max_wnodes {}", max_wnodes);
};

int main(int, char*[])
{
	// Службу не нужно стартовать если : 
	// - ошибка загрузки файла конфигурации
	// - ошибка связи с трансивером
	// - ошибка подключения к БД
	// WEB интерфейс если не может подключиться к службе,
	// смотрит информацию в логах системы.

		
	initialisation();
	bool autostart_network = false;
	
	TESTER();

	DbDecThread 	dbdecthread;
	AgrThread 		agrthread;
	GWThread  		gwthread(&agrthread);
	HTTP_Server_set_GWThread(&gwthread);

	dbdecthread.start();
	agrthread.start();
	if (autostart_network)
		gwthread.start();	
	HTTP_Server_run();
	

	while (true){
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

		if (!dbdecthread.isRunnig()){
			dbdecthread.start();
		}	

		if (!agrthread.isRunnig()){
			agrthread.start();
		}
	};
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


