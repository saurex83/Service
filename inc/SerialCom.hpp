#pragma once
#include "boost/asio.hpp"

enum flush_type{
	flush_receive = TCIFLUSH,
	flush_send = TCIOFLUSH,
	flush_both = TCIOFLUSH
};

class SerialCom
{
  private:
    static SerialCom* p_instance;
	void connect();
  protected:
    SerialCom();
    SerialCom(const SerialCom&);
    SerialCom& operator=(SerialCom&);
	static boost::asio::serial_port *m_Port;
	static void flush_serial_port(
		boost::asio::serial_port& serial_port,
  		flush_type what,
    	boost::system::error_code& error);
	static boost::asio::io_service m_Io;
  public:
    static SerialCom& getInstance();
   ~SerialCom();
	void write(const unsigned char *transmitData, 
			int writeSize);
	unsigned int read(unsigned char *transmitData,
		   	unsigned int readSize);
};

