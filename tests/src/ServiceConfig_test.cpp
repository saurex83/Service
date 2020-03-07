#include "gtest/gtest.h"
#include "ServiceConfig.hpp"

TEST(ServiceConfig, Load_config){
	bool res = ServiceConfig::Load();
	ASSERT_EQ(res, true);
}

TEST(ServiceConfig, Check_loaded){
	bool res = ServiceConfig::Load();
	ASSERT_EQ(ServiceConfig::port,"/dev/ttyUSB2");
	ASSERT_EQ(ServiceConfig::speed, 115200);
}

