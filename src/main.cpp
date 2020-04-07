//#include <iostream>
#include "ServiceConfig.hpp"
#include "SerialCom.hpp"
#include "debug.hpp"
#include "Transiver.hpp"
#include "database.hpp"
#include "gwthread.hpp"

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
	SPDLOG_INFO("Debug inited");

//******************************************************************	
// Загрузка конфигурации
//******************************************************************
//	try{
		ServiceConfig& cfg = ServiceConfig::getInstance();	
//	}
//	catch(){
//	};

	SPDLOG_INFO("ServiceConfig loaded");

//******************************************************************	
// Инициализация трансивера
//******************************************************************	
	SerialCom& com = SerialCom::getInstance();
	SPDLOG_INFO("SerialCom open port");

	Transiver::network_seed(false);	
	bool status = Transiver::is_network_seed();
	
	if (status){
		SPDLOG_INFO("Transiver communication error");
		return 1;
	}
	
	SPDLOG_INFO("Transiver network seed stoped");

//******************************************************************	
// Подключение к базе данных 
//******************************************************************	
	DataBase db;

//******************************************************************	
// Подключение к базе данных 
//******************************************************************	
	GWThread th;
	th.start();
	th.stop();
}
