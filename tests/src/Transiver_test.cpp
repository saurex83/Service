#include "gtest/gtest.h"
#include "Transiver.hpp"
#include "ServiceConfig.hpp"
#include <iostream>
#include "frame.hpp"

TEST(Transiver, cmd_is_network_seed){
	
	Transiver::network_seed(true);
	bool seed = Transiver::is_network_seed();

	ASSERT_EQ(seed, true);
	Transiver::network_seed(false);
	seed = Transiver::is_network_seed();
	ASSERT_EQ(seed, false);
}

TEST(Transiver, rx_frames){
	Transiver::network_seed(true);
	int rx = Transiver::rx_frames();
	ASSERT_EQ(rx , 0);
}

TEST(Transiver, tx_frames){
	Transiver::network_seed(true);
	int tx = Transiver::tx_frames();
	ASSERT_EQ(tx , 0);
}

TEST(Transiver, push_tx){
	Transiver::network_seed(true);
	int tx = Transiver::tx_frames();
	ASSERT_EQ(tx , 0);
	
	Frame frame;
	frame.meta.CH = 15;
	frame.meta.TS = 1;
	unsigned char test[3] = {1,2,3};
	frame.addHeader(test, sizeof(test));
	Transiver::push_tx(&frame);
	
	tx = Transiver::tx_frames();
	ASSERT_EQ(tx , 1);
}

TEST(Transiver, cmd_set_panid){
	Transiver::network_seed(false);
	Transiver::set_panid(0xaa);
}

TEST(Transiver, cmd_set_rtc){
	Transiver::network_seed(false);
	Transiver::set_rtc(0x11223344);
}

TEST(Transiver, cmd_load_streem_iv){
	Transiver::network_seed(false);
	unsigned char iv[16] = {10,11,12,13,14,15,16,17,18,19,20,21,23,24,25,26};
	Transiver::load_streem_iv(iv);
}

TEST(Transiver, cmd_load_streem_key){
	Transiver::network_seed(false);
	unsigned char key[16] = {10,11,12,13,14,15,16,17,18,19,20,21,23,24,25,26};
	Transiver::load_streem_key(key);
}

TEST(Transiver, cmd_open_slot){
	Transiver::network_seed(true);
	Transiver::open_slot(2, 11);
	Transiver::open_slot(49, 26);
}

TEST(Transiver, cmd_close_slot){
	Transiver::network_seed(true);
	Transiver::close_slot(2);
	Transiver::close_slot(49);
}

TEST(Transiver, cmd_network_seed_start){
	Transiver::network_seed(false);
	Transiver::network_seed(true);
	bool seed = Transiver::is_network_seed();
	ASSERT_EQ(seed, true);
}

TEST(Transiver, cmd_network_seed_stop){
	Transiver::network_seed(true);
	Transiver::network_seed(false);
	bool seed = Transiver::is_network_seed();
	ASSERT_EQ(seed, false);
}
