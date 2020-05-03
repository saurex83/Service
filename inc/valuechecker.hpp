#pragma once

#include <string>
#include <vector>


namespace checker{

struct CheckRes{
	bool error;
	std::string msg;
};

	CheckRes pan_id(const unsigned char &val);
	CheckRes tx_power(const unsigned char &val);
	CheckRes sync_channel(const unsigned char &val);
	CheckRes sys_channel(const unsigned char &val);
	CheckRes encrypt_iv_key(std::string &val);
	// TODO перегрузить черкер на вектор.
	// Проверить что числа хорошо распределены.
};
