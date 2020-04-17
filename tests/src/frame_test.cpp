#include "gtest/gtest.h"
#include "frame.hpp"
#include <iostream>

TEST(Frame, frame_create){
	Frame frame;
	ASSERT_EQ(frame.meta.meta.TIMESTAMP , 0);
	ASSERT_EQ(frame.meta.meta.RSSI_SIG, 0);
	ASSERT_EQ(frame.meta.meta.LIQ, 0);
	ASSERT_EQ(frame.meta.meta.TS, 0);
	ASSERT_EQ(frame.meta.meta.CH, 0);
	ASSERT_EQ(frame.meta.meta.PID, 0);
	ASSERT_EQ(frame.meta.meta.NDST, 0);
	ASSERT_EQ(frame.meta.meta.NSRC, 0);
	ASSERT_EQ(frame.meta.meta.FSRC, 0);
	ASSERT_EQ(frame.meta.meta.IPP, 0);
	ASSERT_EQ(frame.meta.meta.tx_attempts, 0);	
}

TEST(Frame, header_operations){
	Frame frame;
	vector<unsigned char> tst_header = {1,2,3};

	frame.addHeader(tst_header);
	ASSERT_EQ(frame.payload == tst_header, true);	
	ASSERT_EQ(frame.size(), 3);

	vector<unsigned char> tst_add_vec = {5,6,7};
	vector<unsigned char> result = {5,6,7,1,2,3};
	frame.addHeader(tst_add_vec);

//	for (vector<unsigned char>::iterator it = frame.payload.begin();
//			it != frame.payload.end(); it++)
//		cout << unsigned(*it) << " ";
//	cout << endl;

	ASSERT_EQ(frame.size(), 6);
	ASSERT_EQ(frame.payload == result, true);

	frame.delHeader(2);
	result = {7,1,2,3};	
	ASSERT_EQ(frame.size(), 4);
	ASSERT_EQ(frame.payload == result, true);
	
	frame.delHeader(3);
	result = {3};	
	ASSERT_EQ(frame.size(), 1);
	ASSERT_EQ(frame.payload == result, true);
	
	frame.delHeader(1);
	ASSERT_EQ(frame.size(), 0);
}


TEST(Frame, parser){
	Frame frame;
	vector<unsigned char> raw ={
		0,0x1a, //TIMESTAMP
		9,		//RSSI_SIG
		10,     //LIQ
		11,     //TS
		12,     //CH
		13,     //PID
		0,0x11, //NDST
		0,0x12, //NSRC
		14,     //ETX
		0,0x13, //FDST
		0,0x14, //FSRC
		15,     //IPP
		16,     //tx_attempts
		5,		//LEN
		1,2,3,4,5 //PAYALOAD
	};
//	cout << "META SIZE " << frame.meta.size() << endl;
	frame.parseRaw(raw);

	ASSERT_EQ(frame.size(), 5);
	vector<unsigned char> result = {1,2,3,4,5};
	ASSERT_EQ(frame.payload == result, true);

	vector<unsigned char> conv_res;
	frame.convertRaw(conv_res);
	ASSERT_EQ(conv_res == raw, true);
	ASSERT_EQ(conv_res.size() == raw.size(), true);
}


