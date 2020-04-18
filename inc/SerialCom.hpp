#pragma once
#include "boost/asio.hpp"
#include <vector>

using namespace std;

namespace comm{
	void connect();
	bool isConnected();
	void write(vector<unsigned char>& data);
	void read(vector<unsigned char>& data);
};
