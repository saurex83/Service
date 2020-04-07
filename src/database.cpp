#include "database.hpp"
#include "ServiceConfig.hpp"
#include "debug.hpp"


DataBase::DataBase(){
	ServiceConfig& cfg = ServiceConfig::getInstance();	
    
	std::string m_dbhost = cfg.db_ip;
    int         m_dbport = cfg.db_port;
    std::string m_dbname = cfg.db_name;
    std::string m_dbuser = cfg.db_login;
    std::string m_dbpass = cfg.db_password;

	m_connection.reset(
			PQsetdbLogin(m_dbhost.c_str(),
			   	std::to_string(m_dbport).c_str(),			   	
				nullptr, nullptr, 
				m_dbname.c_str(), 
				m_dbuser.c_str(),
			   	m_dbpass.c_str()),
		   		&PQfinish );

    if (PQstatus( m_connection.get() ) != CONNECTION_OK && PQsetnonblocking(m_connection.get(), 1) != 0 )
    {
		SPDLOG_INFO(PQerrorMessage(m_connection.get()));  
		throw std::runtime_error(PQerrorMessage(m_connection.get()));
    }

	SPDLOG_INFO("Connected to dababase");
}

DataBase::~DataBase(){
	SPDLOG_INFO("Connection with database closed");
}
