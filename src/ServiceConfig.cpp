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

static	src::severity_logger<severity_level> lg;

ServiceConfig* ServiceConfig::p_instance = 0;
  
ServiceConfig& ServiceConfig::getInstance() {
    if(!p_instance)     {
        p_instance = new ServiceConfig();
    }
    return *p_instance;
}

ServiceConfig::ServiceConfig(){
	try{
		property_tree::ptree pt;
		property_tree::ini_parser::read_ini(CONFIG_FILE, pt);
		ServiceConfig::port =pt.get<std::string>("serial.port");
		ServiceConfig::speed = pt.get<unsigned int>("serial.speed");	
	}
	catch(boost::property_tree::ptree_bad_data &e){
		//если строку пытается представить в виде числа
		LOGGER(lg, error) <<"Wrong data format. "<< e.what();
	}
	catch(property_tree::ptree_bad_path &e){
		// Если нету узла в ini
		LOGGER(lg, error) <<"Ini node wrong. "<< e.what(); 
	}
	catch(property_tree::ptree_error &e){
		// Если файл неудалось загрузить
		LOGGER(lg, error) <<"File error. "<< e.what(); 
	}
};
