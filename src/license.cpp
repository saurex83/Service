#include "license.hpp"
#include "database.hpp"
#include <string>
#include "debug.hpp"
#include <vector>
#include <utility>

using namespace std;

// Базовые параметры лицензирования
static vector<pair<string, string>> LIC_BASIC {
	{"1","2"}, {"7fef", "345"}
};

License::License(){
	std::string lic_str;	
	std::string device_serial;

   	if (!loadDeviceSerial(device_serial)){
		addDefaultLic();
		return;
	}

	if (!loadStrDb(lic_str)){
		addDefaultLic();
		return;
	}

	if (!checkLicense(lic_str, device_serial)){
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
	// TOSO license data может быть и не создана
	for (pair<string,string> value : LIC_BASIC){
		if (!license_data->has(value.first))
			license_data->set(value.first, value.second);
	};
};

bool License::loadDeviceSerial(string& serial){
};

bool License::checkLicense(string& lic, string& serial){
};
