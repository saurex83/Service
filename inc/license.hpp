#pragma once
#include "Poco/SingletonHolder.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Exception.h>
	
class License{
public:
	License();
	~License();
	static License& instance(){
		static Poco::SingletonHolder<License> sh;
		return *sh.get();
	}
private:
	Poco::JSON::Object::Ptr license_data;
	bool loadStrDb(std::string& lic_str);
	void parseLic(std::string& lic_str);
	bool loadDeviceSerial(std::string& serial);
	bool checkLicense(std::string& lic_str, std::string& serial);
	void addDefaultLic();
};
