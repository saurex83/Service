#include "gtest/gtest.h"
#include "SerialCom.hpp"
#include "ServiceConfig.hpp"
#include <vector>

using namespace std;

TEST(SerialCom, Open_test){
	comm::connect();
};

TEST(SerialCom, DISABLED_Connect_test){
	comm::connect();
	vector<unsigned char> data_tx = {5,1,2,3,4};
	vector<unsigned char> data_rx ;
	comm::write(data_tx);
	comm::read(data_rx);
}

