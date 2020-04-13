#include "valuechecker.hpp"
#include "debug.hpp"
#include <Poco/Foundation.h>
#include <Poco/RegularExpression.h>

using namespace checker; 

static bool inRange(unsigned char min, unsigned char max,
	   	const unsigned char &val){
	if (val >=min && val<=max)
		return true;
	else 
		return false;
}

CheckRes pan_id(const unsigned char &val){
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

CheckRes sys_channel(const unsigned char &val){
	CheckRes res;

	if (!inRange(11,28, val)){
		res.error = true;
		res.msg = "sys_channel not in range [11..28]";
	   	return res;	
	}

	res.error = false;
	return res;
};

CheckRes sync_channel(const unsigned char &val){
	CheckRes res;

	if (!inRange(11,28, val)){
		res.error = true;
		res.msg = "sync_channel not in range [11..28]";
	   	return res;	
	}

	res.error = false;
	return res;
};


CheckRes encrypt_iv_key(std::string &val){
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
