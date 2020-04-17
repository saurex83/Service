#include "gtest/gtest.h"
#include "meta.hpp"

using namespace std;

TEST(Meta, create){
	Meta mt;
	vector<unsigned char> vec;

	mt.meta.TIMESTAMP = 1;
	mt.meta.RSSI_SIG = 2;
	mt.meta.LIQ = 3;
	mt.meta.TS = 4;
	mt.meta.CH = 5;
	mt.meta.PID = 6;
	mt.meta.NDST = 0x1122;
	mt.meta.NSRC = 0x3344;
	mt.meta.ETX = 7;
	mt.meta.FDST = 0x5566;
	mt.meta.FSRC = 0x7788;
	mt.meta.IPP = 8;
	mt.meta.tx_attempts = 9;

	mt.convertRaw(vec);
	ASSERT_EQ(vec.size() , mt.size());
	
	Meta mt_in;
	mt_in.parseRaw(vec);

	ASSERT_EQ(mt_in.meta.TIMESTAMP, 1);
	ASSERT_EQ(mt_in.meta.RSSI_SIG, 2);
	ASSERT_EQ(mt_in.meta.LIQ, 3);
	ASSERT_EQ(mt_in.meta.TS, 4);
	ASSERT_EQ(mt_in.meta.CH, 5);
	ASSERT_EQ(mt_in.meta.PID, 6);
	ASSERT_EQ(mt_in.meta.NDST, 0x1122);
	ASSERT_EQ(mt_in.meta.NSRC, 0x3344);
	ASSERT_EQ(mt_in.meta.ETX, 7);
	ASSERT_EQ(mt_in.meta.FDST, 0x5566);
	ASSERT_EQ(mt_in.meta.FSRC, 0x7788);
	ASSERT_EQ(mt_in.meta.IPP, 8);
	ASSERT_EQ(mt_in.meta.tx_attempts, 9);
};
