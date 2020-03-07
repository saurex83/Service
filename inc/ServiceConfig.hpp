#pragma once
#include <string>

class ServiceConfig{
public:
	static	std::string port;
	static	unsigned int speed;		
	
	static bool Load();
	ServiceConfig();
};
