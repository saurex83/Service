#pragma once

#include "SerialCom.hpp" 
#include <cstdint> 
#include <exception>

class ParserError: public std::exception
{
private:
	std::string m_error;
public:
	ParserError(std::string error)
		: m_error(error){
	};
	const char* what() const noexcept {
		return m_error.c_str();
	};
};



class CMDError: public std::exception
{
private:
	std::string m_error;
public:
	CMDError(std::string error)
		: m_error(error){
	};
	const char* what() const noexcept {
		return m_error.c_str();
	};
};



#define WRITE_BUFF_SIZE 1024
#define READ_BUFF_SIZE 1024

class Transiver{
public:
	Transiver();
	~Transiver();
	static void connect();
	static bool is_network_seed();
	static void set_panid(unsigned char panid);
	static void network_seed(bool status);
	static void set_rtc(uint32_t rtc);
	static void load_streem_iv(unsigned char *iv);
	static void load_streem_key(unsigned char *key);
	static void open_slot(unsigned char ts, unsigned char ch);
	static void close_slot(unsigned char ts);
private:
	static void serial_write(unsigned char *data, unsigned int len);
	static int send_cmd(unsigned char *cmd, int size);
	static unsigned short crc16(unsigned char *pcBlock, unsigned int len);
	static int serial_read();
	static SerialCom serial;
	static unsigned char rx_buffer[READ_BUFF_SIZE];	
	static unsigned char tx_buffer[WRITE_BUFF_SIZE];	
	static bool serial_inited;
};
