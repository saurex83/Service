#pragma once
#include <exception>
#include <cstdint> 
#include <string>

#define MAX_PAYLOAD_SIZE 127

class FrameError: public std::exception
{
private:
	std::string m_error;
public:
	FrameError(std::string error)
		: m_error(error){
	};
	const char* what() const noexcept {
		return m_error.c_str();
	};
};


struct meta {
	unsigned short int TIMESTAMP;
   	signed char RSSI_SIG; 
   	char LIQ;
   	char TS;
   	char CH;
   	char PID;
   	unsigned short int NDST;
   	unsigned short int NSRC;
   	char ETX;
   	unsigned short int FDST;
   	unsigned short int FSRC;
   	char IPP;
   	char tx_attempts;
} __attribute__((packed));

class Frame{
	public:
		struct meta meta;
		unsigned char len;
		unsigned char payload[MAX_PAYLOAD_SIZE];

		Frame();
		void addHeader(unsigned char *head, char len);
		void delHeader(char len);
	private:
	void mem_move(unsigned char* dst, unsigned char* src, size_t len);
};
