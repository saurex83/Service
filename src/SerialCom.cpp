#include "ServiceConfig.hpp"
#include "SerialCom.hpp"
#include <iostream>
#include <string>
#include <boost/asio/serial_port.hpp> 
#include <boost/bind.hpp>
#include "debug.hpp"

using namespace boost;
using namespace asio;
using namespace boost::asio;

namespace comm{
#define READ_TIMEOUT_US 20

class blocking_reader
{
	boost::asio::serial_port& port;
	size_t timeout;
	char c;
	boost::asio::deadline_timer timer;
	bool read_error;
	// Called when an async read completes or has been cancelled
	void read_complete(const boost::system::error_code& error,
						size_t bytes_transferred) {		
		read_error = (error || bytes_transferred == 0);
		timer.cancel();
	}

	// Called when the timer's deadline expires.
	void time_out(const boost::system::error_code& error) {
		if (error) {
			return;
		}
		port.cancel();
	}

public:
	blocking_reader(boost::asio::serial_port& port, size_t timeout) :
												port(port), timeout(timeout),
												timer(port.get_io_service()),
												read_error(true) {
	}
	bool read_char(char& val) {
		val = c = '\0';
		port.get_io_service().reset();
		boost::asio::async_read(port, boost::asio::buffer(&c, 1), 
				boost::bind(&blocking_reader::read_complete, 
						this, 
						boost::asio::placeholders::error, 
						boost::asio::placeholders::bytes_transferred)); 

		timer.expires_from_now(boost::posix_time::milliseconds(timeout));
		timer.async_wait(boost::bind(&blocking_reader::time_out,
								this, boost::asio::placeholders::error));
		port.get_io_service().run();
		if (!read_error)
			val = c;
		return !read_error;
	}
};

enum flush_type{
	flush_receive = TCIFLUSH,
	flush_send = TCIOFLUSH,
	flush_both = TCIOFLUSH
};

static serial_port* m_Port;
static io_service m_Io;
static bool connected = false;

static void _connect(){
	if (connected)
		return;

	ServiceConfig& config = ServiceConfig::getInstance(); 
	m_Port = new boost::asio::serial_port(m_Io);
	m_Port->open(config.port);
	
#define SPO_1 serial_port_base::baud_rate(config.speed)
#define SPO_2 serial_port_base::character_size(8)
#define SPO_3 serial_port_base::flow_control(\
		serial_port_base::flow_control::none)
#define SPO_4 serial_port_base::parity(serial_port_base::parity::none)
#define SPO_5 serial_port_base::stop_bits(serial_port_base::stop_bits::one)

	m_Port->set_option(SPO_1);
   	m_Port->set_option(SPO_2);
	m_Port->set_option(SPO_3);
    m_Port->set_option(SPO_4);
    m_Port->set_option(SPO_5);

	connected = true;
};


static void flush_serial_port(
	serial_port& serial_port,
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

void connect(){
	_connect();
};

void write(vector<unsigned char>& data){
	_connect();
	boost::asio::write(*m_Port, buffer(&data.front(), data.size()));
};

void read(vector<unsigned char>& data){
	_connect();
	data.clear();
	
	char r_char;
	boost::system::error_code error;	
//	flush_serial_port(*m_Port, flush_receive, error);
//	if (error != boost::system::errc::success){
//		SPDLOG_ERROR("Flush error: {}", error.message().c_str());
//		throw(runtime_error(error.message()));
//	};

	blocking_reader reader(*m_Port, READ_TIMEOUT_US);
	   
  	while (reader.read_char(r_char))
		data.push_back((unsigned char)r_char);
}
};
