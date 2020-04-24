#pragma once
#include <exception>
#include <cstdint> 
#include <string>
#include <vector>

using namespace std;

class Meta {
public:
	struct {
		unsigned short int TIMESTAMP;
		signed char RSSI_SIG; 
		unsigned char LIQ;
		unsigned char TS;
		unsigned char CH;
		unsigned char PID;
		unsigned short int NDST;
		unsigned short int NSRC;
		unsigned char NSRC_TS;
   		unsigned char NSRC_CH;
		unsigned char ETX;
		unsigned short int FDST;
		unsigned short int FSRC;
		unsigned char IPP;
		unsigned char tx_attempts;
	}__attribute__((packed)) meta;
	
	Meta();
	size_t size();
	void convertRaw(vector<unsigned char>& raw);
	void parseRaw(vector<unsigned char>& raw);
private:
};
