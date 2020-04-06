#include "gtest/gtest.h"
#include "frame.hpp"

TEST(Frame, frame_create){
	Frame frame;
	ASSERT_EQ(frame.len , 0);
	ASSERT_EQ(frame.meta.TIMESTAMP , 0);
	ASSERT_EQ(frame.meta.RSSI_SIG, 0);
	ASSERT_EQ(frame.meta.LIQ, 0);
	ASSERT_EQ(frame.meta.TS, 0);
	ASSERT_EQ(frame.meta.CH, 0);
	ASSERT_EQ(frame.meta.PID, 0);
	ASSERT_EQ(frame.meta.NDST, 0);
	ASSERT_EQ(frame.meta.NSRC, 0);
	ASSERT_EQ(frame.meta.FSRC, 0);
	ASSERT_EQ(frame.meta.IPP, 0);
	ASSERT_EQ(frame.meta.tx_attempts, 0);	
}

TEST(Frame, addHeader){
	Frame frame;
	unsigned char arr[10] = {1,2,3,4,5,6,7,8,9,10};

	frame.addHeader(arr, sizeof(arr));

	ASSERT_EQ(frame.len, sizeof(arr));

	for (int i = 0; i < sizeof(arr); i++)
		ASSERT_EQ(frame.payload[i], arr[i]);
	
	unsigned char arr2[5] = {100,101,102,103,104};
	
//	std::copy(std::begin(frame.payload), 
//			std::end(frame.payload),
//		   	std::ostream_iterator<int>(std::cout, " "));	
	
	frame.addHeader(arr2, sizeof(arr2));

//	std::copy(std::begin(frame.payload), 
//			std::end(frame.payload),
//		   	std::ostream_iterator<int>(std::cout, " "));	

	ASSERT_EQ(frame.len, sizeof(arr)+sizeof(arr2));
	
	for (int i = 0; i < sizeof(arr2); i++)
		ASSERT_EQ(frame.payload[i], arr2[i]);

	for (int i = 0; i < sizeof(arr); i++)
		ASSERT_EQ(frame.payload[i+sizeof(arr2)], arr[i]);
}


TEST(Frame, delHeader){
	Frame frame;
	unsigned char arr[10] = {1,2,3,4,5,6,7,8,9,10};

	frame.addHeader(arr, sizeof(arr));

	ASSERT_EQ(frame.len, sizeof(arr));

	frame.delHeader(5);
	
	ASSERT_EQ(frame.len, sizeof(arr) - 5);

	for (int i = 0; i < sizeof(arr) - 5; i++)
		ASSERT_EQ(frame.payload[i], arr[i + 5]);

//	std::copy(std::begin(frame.payload), 
//			std::end(frame.payload),
//		   	std::ostream_iterator<int>(std::cout, " "));	
	
	frame.delHeader(5);
	ASSERT_EQ(frame.len, 0);

	for (int i = 0; i < sizeof(arr); i++)
		ASSERT_EQ(frame.payload[i], 0);
}


