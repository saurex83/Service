#pragma once
#include "Poco/SingletonHolder.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Exception.h>
#include <Poco/AtomicCounter.h>	
class License{
public:
	License();
	~License();
	static License& instance(){
		static Poco::SingletonHolder<License> sh;
		return *sh.get();
	};
	bool getLicParam(std::string param, int &val);
	bool getLicParam(std::string param, bool &val);
	bool getLicParam(std::string param, float &val);
	bool getLicParam(std::string param, std::string &val);
private:
	Poco::JSON::Object::Ptr license_data;
	bool loadStrDb(std::string& lic_str);
	void parseLic(std::string& lic_str);
	bool loadDeviceSerial(std::string& serial);
	bool checkLicense(std::string& lic_str, std::string& serial);
	void addDefaultLic();
	template<typename TYPE_PARAM>
		bool getLicParam_templ(std::string param, TYPE_PARAM &val);
};
