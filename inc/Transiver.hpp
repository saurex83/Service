#pragma once

#include "SerialCom.hpp" 
#include <cstdint> 
#include <exception>
#include "frame.hpp"

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

namespace Transiver{
	bool is_network_seed();
	void set_panid(unsigned char panid);
	void network_seed(bool status);
	void set_rtc(uint32_t rtc);
	void load_streem_iv(unsigned char *iv);
	void load_streem_key(unsigned char *key);
	void open_slot(unsigned char ts, unsigned char ch);
	void close_slot(unsigned char ts);
	int rx_frames();
	int tx_frames();
	void push_tx(Frame *frame);
	unsigned int pop_rx(unsigned char *frame);
}

