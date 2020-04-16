#include "license.hpp"
#include "database.hpp"
#include <string>
#include "debug.hpp"
#include <vector>
#include <utility>

using namespace std;

// Базовые параметры лицензирования
static vector<pair<string, string>> LIC_BASIC {
	{"max_wnodes","20"}, {"max_virtsensors", "10"},
	{"gtest_float", "3.14"}, {"gtest_int", "314"},
	{"gtest_str", "hello"},
	{"gtest_bool", "true"}
};

License::License(){
	std::string lic_str;	
	std::string device_serial;
   	
	if (!loadDeviceSerial(device_serial)){
		addDefaultLic();
		SPDLOG_ERROR("Device serial number not loaded");
		return;
	}
	SPDLOG_INFO("Device serial number: {}",device_serial.c_str());

	if (!loadStrDb(lic_str)){
		addDefaultLic();
		SPDLOG_ERROR("License from database not loaded. Use default license");
		return;
	}

	if (!checkLicense(lic_str, device_serial)){
		SPDLOG_ERROR("License invalide. Use default license");
		addDefaultLic();
		return;
	}
	
	parseLic(lic_str);	
	addDefaultLic();
}

License::~License(){
};

bool License::loadStrDb(std::string& lic_str){
	DataBase db;
	if (!db.getLicense(lic_str))
		return false;
	return true;	
};

void License::parseLic(std::string& lic_str){
	Poco::JSON::Parser parser;
	try{	
		Poco::Dynamic::Var result = parser.parse(lic_str);
		license_data = result.extract<Poco::JSON::Object::Ptr>();
	}
	catch(Poco::Exception& exc){
		std::string message_error = exc.displayText();
		SPDLOG_ERROR("License file parse:{}", message_error);
		return;
	}
};

void License::addDefaultLic(){
	if (license_data.isNull())
		license_data = new Poco::JSON::Object;
	
	for (pair<string,string> value : LIC_BASIC){
		if (!license_data->has(value.first))
			license_data->set(value.first, value.second);
	};
};

bool License::loadDeviceSerial(string& serial){
	return false;
};

bool License::checkLicense(string& lic, string& serial){
	return true;
};

template<typename TYPE_PARAM>
bool License::getLicParam_templ(std::string param, TYPE_PARAM &val){
	if (!license_data->has(param))
		return false;

	try{
		Poco::Dynamic::Var v_val = license_data->get(param);
		val = v_val.convert<TYPE_PARAM>();
	}
	catch(Poco::Exception& exc){
		SPDLOG_ERROR("Get license param ex: {}", exc.displayText().c_str());
		return false;
	};
	return true;
};



bool License::getLicParam(std::string param, int &val){
	return getLicParam_templ(param, val);
};

bool License::getLicParam(std::string param, bool &val){
	return getLicParam_templ(param, val);
};

bool License::getLicParam(std::string param, float &val){
	return getLicParam_templ(param, val);
};

bool License::getLicParam(std::string param, std::string &val){
	return getLicParam_templ(param, val);
};
