#include "gtest/gtest.h"
#include "ServiceConfig.hpp"


TEST(ServiceConfig, Check_loaded){
	ServiceConfig& cfg = ServiceConfig::getInstance();
	ASSERT_EQ(cfg.port,"/dev/ttyUSB0");
	ASSERT_EQ(cfg.speed, 115200);
}

