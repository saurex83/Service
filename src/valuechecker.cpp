#include "valuechecker.hpp"
#include "debug.hpp"
#include <Poco/Foundation.h>
#include <Poco/RegularExpression.h>
#include <set>

using namespace checker; 



static bool inRange(unsigned char min, unsigned char max,
	   	const unsigned char &val){
	if (val >=min && val<=max)
		return true;
	else 
		return false;
}

CheckRes checker::pan_id(const unsigned char &val){
	CheckRes res;
	if (val == 0){
		res.error = true;
		res.msg = "pan_id can not be equal zero"; 
	} 
	else {
		res.error = false;
	}
	return res;
};

CheckRes checker::sys_channel(const unsigned char &val){
	CheckRes res;

	if (!inRange(11,28, val)){
		res.error = true;
		res.msg = "sys_channel not in range [11..28]";
	   	return res;	
	}

	res.error = false;
	return res;
};

CheckRes checker::sync_channel(const unsigned char &val){
	CheckRes res;

	if (!inRange(11,28, val)){
		res.error = true;
		res.msg = "sync_channel not in range [11..28]";
	   	return res;	
	}

	res.error = false;
	return res;
};

CheckRes checker::tx_power(const unsigned char &val){
	std::set<unsigned char> tx_power_const = {
 		 0xF5, 0xE5, 0xD5, 0xC5, 0xB5, 0xA5, 0x95, 0x85, 0x75, 0x65, 0x55,
		 0x45, 0x35, 0x15, 0x05};

	CheckRes res;
	
	if (!tx_power_const.count(val)){
		res.error = true;
		res.msg = "TX power not constant set 0xF5, 0xE5, 0xD5, 0xC5,\
			0xB5, 0xA5, 0x95, 0x85, 0x75, 0x65, 0x55, 0x45, 0x35,\
		   	0x15, 0x05";
	   	return res;	
	}

	res.error = false;
	return res;
};

CheckRes checker::encrypt_iv_key(std::string &val){
	CheckRes res;
	
	Poco::RegularExpression regex("(?i)^[0-9abcdef]{32}$");
	Poco::RegularExpression::MatchVec mvec;
	int matches = regex.match(val, 0, mvec);
	if (matches =! 1){
		res.error = true;
		res.msg = "Hex string is no response format :00112233445566778899aabbccddeeff";
	   	return res;	
	};	
	
	res.error = false;
	return res;
}
