#pragma once
#include <string>

class ServiceConfig
{
  private:
    static ServiceConfig* p_instance;
	void debugOutput();
  protected:
    ServiceConfig();
    ServiceConfig(const ServiceConfig&);
    ServiceConfig& operator=(ServiceConfig&);
   ~ServiceConfig(){};
  public:
    static ServiceConfig& getInstance();
	std::string port;
	unsigned int speed;
	unsigned int db_port;
	std::string db_login;
	std::string db_password;
	std::string db_name;
	std::string db_ip;
};
