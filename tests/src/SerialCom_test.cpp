#include "gtest/gtest.h"
#include "SerialCom.hpp"
#include "ServiceConfig.hpp"

TEST(SerialCom, Open_test){
	ServiceConfig cfg;
	cfg.Load();
	SerialCom test_obj;
	test_obj.connect();
	test_obj.disconnect();
};

TEST(SerialCom, Connect_test){
	SerialCom test_obj = SerialCom();
	
	unsigned char data_tx[5] = {5,1,2,3,4};
	unsigned char data_rx[10] = {0,0,0,0,0,0,0,0,0,0};
	test_obj.connect();
	test_obj.write(data_tx, sizeof(data_tx));
	test_obj.read(data_rx, 200);
	test_obj.disconnect();
}

