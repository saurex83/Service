#include "gtest/gtest.h"
#include "license.hpp"
#include <string>

using namespace std;

TEST(License, read_params){
	License& lic = License::instance();

	int int_t;
	float float_t;
	bool bool_t;
	string str_t;


	bool res = lic.getLicParam(string("gtest_int"), int_t);
	ASSERT_EQ(res , true);

	res = lic.getLicParam(string("gtest_float"), float_t);
	ASSERT_EQ(res , true);

	res = lic.getLicParam(string("gtest_str"), str_t);
	ASSERT_EQ(res , true);
	
	res = lic.getLicParam(string("gtest_str"), bool_t);
	ASSERT_EQ(res , true);


	ASSERT_EQ(int_t, 314);
	EXPECT_FLOAT_EQ(float_t, 3.14);	
	ASSERT_EQ(str_t, "hello");	
	ASSERT_EQ(bool_t, true);	

	res = lic.getLicParam(string("gtes34f,.strfreeeeeeeeeee"), bool_t);
	ASSERT_EQ(res , false);
}
