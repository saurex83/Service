#pragma once
#include "boost/asio.hpp"

enum flush_type{
	flush_receive = TCIFLUSH,
	flush_send = TCIOFLUSH,
	flush_both = TCIOFLUSH
};

class SerialCom{
public:
	SerialCom();
	~SerialCom();
	static void connect();
	static void disconnect();
	static void write(const unsigned char *transmitData, 
			int writeSize);
	static unsigned int read(unsigned char *transmitData,
		   	unsigned int readSize);
private:
	static boost::asio::serial_port *m_Port;
	static void flush_serial_port(
		boost::asio::serial_port& serial_port,
  		flush_type what,
    	boost::system::error_code& error);
	static boost::asio::io_service m_Io;
};
