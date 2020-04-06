//#include <iostream>
#include "ServiceConfig.hpp"
#include "SerialCom.hpp"
#include "debug.hpp"
#include "Transiver.hpp"

static src::severity_logger<severity_level> lg;

int initialisation();

int main(int, char*[])
{
//	ServiceConfig test_obj;
//	test_obj.Load();
	
//	std::cout << "aaa";
//	SerialCom com_test ;
//	std::cout << "fff";
   initialisation();
   	return 0;
}



int initialisation(){
	// Настройка модуля отладки
	debugInit();
	CUSTOM_LOG(lg, info) << "Debug inited";

//******************************************************************	
// Загрузка конфигурации
//******************************************************************
//	try{
		ServiceConfig& cfg = ServiceConfig::getInstance();	
//	}
//	catch(){
//	};

	CUSTOM_LOG(lg, info) << "ServiceConfig loaded";
	CUSTOM_LOG(lg, info) << "ServiceConfig.port = " << cfg.port;
	CUSTOM_LOG(lg, info) << "ServiceConfig.speed = " << cfg.speed;

//******************************************************************	
// Инициализация трансивера
//******************************************************************	
	SerialCom& com = SerialCom::getInstance();
	CUSTOM_LOG(lg, info) << "SerialCom open port";

	Transiver::network_seed(false);	
	bool status = Transiver::is_network_seed();
	
	if (status){
		CUSTOM_LOG(lg, error) << "Transiver communication error";
		return 1;
	}
	
	CUSTOM_LOG(lg, info) << "Transiver network seed stoped";

//******************************************************************	
// Подключение к базе данных 
//******************************************************************	



}
