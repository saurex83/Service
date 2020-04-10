#include "debug.hpp"
#include "ServiceConfig.hpp"
#include "Transiver.hpp"
#include "database.hpp"
#include "SerialCom.hpp"
#include <exception>

static bool configured = false;

bool sysConfig_isOk(){
	return configured;
};

void sysConfig(){

	configured = false;

//******************************************************************	
// Загрузка конфигурации
//******************************************************************
	try{
		ServiceConfig& cfg = ServiceConfig::getInstance();	
	}
	catch(...){
	};
	SPDLOG_INFO("ServiceConfig loaded");


//******************************************************************	
// Инициализация трансивера
//******************************************************************	
	SerialCom& com = SerialCom::getInstance();
	SPDLOG_INFO("SerialCom open port");

	Transiver::network_seed(false);	
	bool status = Transiver::is_network_seed();
	
	if (status){
		SPDLOG_ERROR("Transiver communication error");
		throw(std::runtime_error("Comunication with transiver failed"));
	};

//******************************************************************	
// Подключение к базе данных 
//******************************************************************	
	try{
		DataBase db;
	} 
	catch(std::runtime_error& e){
		SPDLOG_ERROR("Database connection failed with msg :{}", e.what());
		throw(e);
	};

//******************************************************************	
// Подключение к базе данных 
//******************************************************************	
//	GWThread th;
//	th.start();
//	th.stop();
//SPDLOG_INFO("START WEB");
//******************************************************************	
// Запуск REST API сервера
//******************************************************************	
//	SPDLOG_INFO("obj server created");
//	HTTP_Server_run();
//	SPDLOG_INFO("server run");
///	while(true);

	configured = true;
}
