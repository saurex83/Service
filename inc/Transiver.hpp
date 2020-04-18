#pragma once
#include "SerialCom.hpp" 
#include <cstdint> 
#include <exception>
#include "frame.hpp"
#include <vector>

namespace Transiver{
	bool is_network_seed();
	void set_panid(unsigned char panid);
	void network_seed(bool status);
	void set_rtc(uint32_t rtc);
	void load_streem_iv(vector<unsigned char>& iv);
	void load_streem_key(vector<unsigned char>& key);
	void open_slot(unsigned char ts, unsigned char ch);
	void close_slot(unsigned char ts);
	void read_energy(vector<signed char>& energy);
	int rx_frames();
	int tx_frames();
	void push_tx(Frame& frame);
	void pop_rx(Frame& frame);
}

