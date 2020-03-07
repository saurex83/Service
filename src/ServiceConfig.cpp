#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "ServiceConfig.hpp"

using namespace std;

#define CONFIG_FILE "config.ini"

std::string ServiceConfig::port;
unsigned int ServiceConfig::speed;

ServiceConfig::ServiceConfig(){
};

bool ServiceConfig::Load(){
	bool res = true;

	try{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini(CONFIG_FILE, pt);
		ServiceConfig::port =pt.get<std::string>("serial.port");
		ServiceConfig::speed = pt.get<unsigned int>("serial.speed");	
	}
	catch(boost::property_tree::ptree_bad_data &e){
		//если строку пытается представить в виде числа
		BOOST_LOG_TRIVIAL(error) <<"Wrong data format. "<< e.what();
		res = false;
	}
	catch(boost::property_tree::ptree_bad_path &e){
		// Если нету узла в ini
		BOOST_LOG_TRIVIAL(error) <<"Ini node wrong. "<< e.what(); 
		res = false;
	}
	catch(boost::property_tree::ptree_error &e){
		// Если файл неудалось загрузить
		BOOST_LOG_TRIVIAL(error) <<"File error. "<< e.what(); 
		res = false;
	}

	return res;
};
