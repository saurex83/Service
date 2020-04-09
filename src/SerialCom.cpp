#include "ServiceConfig.hpp"
#include "SerialCom.hpp"
#include <iostream>
#include <string>
#include "blocking_reader.hpp"
#include "debug.hpp"

using namespace boost;
using namespace asio;

#define READ_TIMEOUT_US 200

boost::asio::serial_port* SerialCom::m_Port;
boost::asio::io_service SerialCom::m_Io;

SerialCom* SerialCom::p_instance = 0;
  
SerialCom& SerialCom::getInstance() {
    if(!p_instance)     {
        p_instance = new SerialCom();
    }
    return *p_instance;
}

SerialCom::SerialCom() {
	connect();
};

SerialCom::~SerialCom(){
	SPDLOG_WARN("Destructor must close com port");
}

void SerialCom::connect(){
	ServiceConfig& config = ServiceConfig::getInstance(); 
	SerialCom::m_Port = new boost::asio::serial_port(m_Io);
	SerialCom::m_Port->open(config.port);
	
    SerialCom::m_Port->set_option(serial_port_base::baud_rate(config.speed));
   	SerialCom::m_Port->set_option(serial_port_base::character_size(8));
	SerialCom::m_Port->set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
    SerialCom::m_Port->set_option(serial_port_base::parity(serial_port_base::parity::none));
    SerialCom::m_Port->set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
};


void SerialCom::flush_serial_port(
	boost::asio::serial_port& serial_port,
  	flush_type what,
    boost::system::error_code& error){
    if (0 == ::tcflush(serial_port.native_handle(), what)){
      error = boost::system::error_code();
    }
    else{
      error = boost::system::error_code(errno,
          boost::asio::error::get_system_category());
    }
}

void SerialCom::write(const unsigned char *transmitData, int writeSize){
	boost::asio::write(*SerialCom::m_Port, buffer(transmitData, writeSize));
};


unsigned int SerialCom::read(unsigned char *transmitData, unsigned int readSize){
	char data;
	unsigned char *ptr = transmitData;
	unsigned int cnt = 0;
	boost::system::error_code error;	

	flush_serial_port(*SerialCom::m_Port, flush_receive, error);
	blocking_reader reader(*SerialCom::m_Port, READ_TIMEOUT_US);
	   
  	while (reader.read_char(data) && readSize > 0){
		*ptr = data;
		ptr++;
		cnt++;
		readSize--;	
	}
	return cnt;
}
