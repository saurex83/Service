#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "ServiceConfig.hpp"
#include "debug.hpp"

using namespace std;
using namespace boost;

#define CONFIG_FILE "config.ini"

//string ServiceConfig::port;
//unsigned int ServiceConfig::speed;

ServiceConfig* ServiceConfig::p_instance = 0;
  
ServiceConfig& ServiceConfig::getInstance() {
    if(!p_instance)     {
        p_instance = new ServiceConfig();
    }
    return *p_instance;
}

void ServiceConfig::debugOutput(){
		SPDLOG_INFO("Config file. Serial port: ", ServiceConfig::port);
		SPDLOG_INFO("Config file. Serial speed: ", ServiceConfig::speed);
		SPDLOG_INFO("Config file. Database IP: ", ServiceConfig::db_ip);
		SPDLOG_INFO("Config file. Database login: ", ServiceConfig::db_login);
		SPDLOG_INFO("Config file. Database password: ", ServiceConfig::db_password);
		SPDLOG_INFO("Config file. Database db_name: ", ServiceConfig::db_name);
		SPDLOG_INFO("Config file. Database port: ", ServiceConfig::db_port);
}

ServiceConfig::ServiceConfig(){
	try{
		property_tree::ptree pt;
		property_tree::ini_parser::read_ini(CONFIG_FILE, pt);
		ServiceConfig::port =pt.get<std::string>("serial.port");
		ServiceConfig::speed = pt.get<unsigned int>("serial.speed");	
		ServiceConfig::db_ip = pt.get<std::string>("db.ip");	
		ServiceConfig::db_login = pt.get<std::string>("db.login");	
		ServiceConfig::db_password = pt.get<std::string>("db.password");	
		ServiceConfig::db_name = pt.get<std::string>("db.db_name");	
		ServiceConfig::db_port = pt.get<unsigned int>("db.port");

		debugOutput();	
	}
	catch(boost::property_tree::ptree_bad_data &e){
		//если строку пытается представить в виде числа
		SPDLOG_ERROR("Wrong data format. ", e.what());
	}
	catch(property_tree::ptree_bad_path &e){
		// Если нету узла в ini
		SPDLOG_ERROR("Ini node wrong.", e.what()); 
	}
	catch(property_tree::ptree_error &e){
		// Если файл неудалось загрузить
		SPDLOG_ERROR("File error. ", e.what()); 
	}
};
