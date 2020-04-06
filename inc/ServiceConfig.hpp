#pragma once
#include <string>

class ServiceConfig
{
  private:
    static ServiceConfig* p_instance;
  protected:
    ServiceConfig();
    ServiceConfig(const ServiceConfig&);
    ServiceConfig& operator=(ServiceConfig&);
   ~ServiceConfig(){};
  public:
    static ServiceConfig& getInstance();
	std::string port;
	unsigned int speed;
};
