#include "gtest/gtest.h"
#include "Transiver.hpp"
#include "ServiceConfig.hpp"

TEST(Transiver, cmd_is_network_seed){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(true);
	bool seed = Transiver::is_network_seed();
	ASSERT_EQ(seed, true);
	Transiver::network_seed(false);
	seed = Transiver::is_network_seed();
	ASSERT_EQ(seed, false);
}

TEST(Transiver, cmd_set_panid){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(false);
	Transiver::set_panid(0xaa);
}

TEST(Transiver, cmd_set_rtc){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(false);
	Transiver::set_rtc(0x11223344);
}

TEST(Transiver, cmd_load_streem_iv){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(false);
	unsigned char iv[16] = {10,11,12,13,14,15,16,17,18,19,20,21,23,24,25,26};
	Transiver::load_streem_iv(iv);
}

TEST(Transiver, cmd_load_streem_key){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(false);
	unsigned char key[16] = {10,11,12,13,14,15,16,17,18,19,20,21,23,24,25,26};
	Transiver::load_streem_key(key);
}

TEST(Transiver, cmd_open_slot){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(true);
	Transiver::open_slot(2, 11);
	Transiver::open_slot(49, 26);
}

TEST(Transiver, cmd_close_slot){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(true);
	Transiver::close_slot(2);
	Transiver::close_slot(49);
}

TEST(Transiver, cmd_network_seed_start){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(false);
	Transiver::network_seed(true);
	bool seed = Transiver::is_network_seed();
	ASSERT_EQ(seed, true);
}

TEST(Transiver, cmd_network_seed_stop){
	ServiceConfig::Load();
	Transiver::connect();
	Transiver::network_seed(true);
	Transiver::network_seed(false);
	bool seed = Transiver::is_network_seed();
	ASSERT_EQ(seed, false);
}
