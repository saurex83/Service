#include "database.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include "debug.hpp"

TEST(Database, db_create){
	DataBase db;
	db.createTables();
//	ASSERT_EQ(res , true);
}

TEST(Database, config_sys_channel){
	DataBase db;
	unsigned char sys_channel_old;
	unsigned char sys_channel = 10;
	bool res = db.get_SYS_CHANNEL(sys_channel_old);
	ASSERT_EQ(res , true);

	res = db.set_SYS_CHANNEL(sys_channel);
	ASSERT_EQ(res, true);
	
	res = db.get_SYS_CHANNEL(sys_channel);
	ASSERT_EQ(res, true);
	ASSERT_EQ(sys_channel, 10);

	res = db.set_SYS_CHANNEL(sys_channel_old);
	ASSERT_EQ(res, true);
}

TEST(Database, nodelist_get_by_mac){
	DataBase db;
	std::vector<unsigned char> mac =
   		{0x11, 0x22, 0x33, 0x44, 0x55, 0xaa, 0xee, 0xff};
	int ipaddr;
	std::string comment;
	std::string location;
	std::string name;
	bool search_res;
	bool res =db.get_NODELIST_by_MAC(mac, ipaddr, name, comment, location,
		   	search_res);


	ASSERT_EQ(res, true);
	ASSERT_EQ(ipaddr, 83);
	ASSERT_EQ(name, "NODE1");
	ASSERT_EQ(comment, "test node");
	ASSERT_EQ(location, "near notebook");
	ASSERT_EQ(search_res, true);


	std::vector<unsigned char> mac_not_av =
   		{0xee, 0x32, 0x33, 0x44, 0x55, 0xaa, 0xee, 0xff};
	
	res =db.get_NODELIST_by_MAC(mac_not_av, ipaddr, name, comment, location,
		   	search_res);

	ASSERT_EQ(res, true);
	ASSERT_EQ(search_res, false);
};

TEST(Database, config_sync_channel){
	DataBase db;
	unsigned char sync_channel_old;
	unsigned char sync_channel = 10;
	bool res = db.get_SYNC_CHANNEL(sync_channel_old);
	ASSERT_EQ(res , true);

	res = db.set_SYNC_CHANNEL(sync_channel);
	ASSERT_EQ(res, true);
	
	res = db.get_SYNC_CHANNEL(sync_channel);
	ASSERT_EQ(res, true);
	ASSERT_EQ(sync_channel, 10);

	res = db.set_SYNC_CHANNEL(sync_channel_old);
	ASSERT_EQ(res, true);
}


TEST(Database, config_pan_id){
	DataBase db;
	unsigned char pan_id_old;
	unsigned char pan_id = 10;
	bool res = db.get_PAN_ID(pan_id_old);
	ASSERT_EQ(res , true);

	res = db.set_PAN_ID(pan_id);
	ASSERT_EQ(res, true);
	
	res = db.get_PAN_ID(pan_id);
	ASSERT_EQ(res, true);
	ASSERT_EQ(pan_id, 10);

	res = db.set_PAN_ID(pan_id_old);
	ASSERT_EQ(res, true);
}


TEST(Database, config_stream_iv){
	DataBase db;
	std::vector<unsigned char> vec_old;
	
	bool res = db.get_STREAM_IV(vec_old);
	ASSERT_EQ(res, true);
	ASSERT_EQ(vec_old.size(), 16);

	std::vector<unsigned char> vec_new = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	res = db.set_STREAM_IV(vec_new);
	ASSERT_EQ(res, true);

	std::vector<unsigned char> vec_new_read;
	res = db.get_STREAM_IV(vec_new_read);
	ASSERT_EQ(res, true);

	ASSERT_EQ(vec_new, vec_new_read);

	res = db.set_STREAM_IV(vec_old);
	ASSERT_EQ(res, true);
}

TEST(Database, config_stream_key){
	DataBase db;
	std::vector<unsigned char> vec_old;
	
	bool res = db.get_STREAM_KEY(vec_old);
	ASSERT_EQ(res, true);
	ASSERT_EQ(vec_old.size(), 16);

	std::vector<unsigned char> vec_new = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	res = db.set_STREAM_KEY(vec_new);
	ASSERT_EQ(res, true);

	std::vector<unsigned char> vec_new_read;
	res = db.get_STREAM_KEY(vec_new_read);
	ASSERT_EQ(res, true);

	ASSERT_EQ(vec_new, vec_new_read);

	res = db.set_STREAM_KEY(vec_old);
	ASSERT_EQ(res, true);
}

TEST(Database, config_data_iv){
	DataBase db;
	std::vector<unsigned char> vec_old;
	
	bool res = db.get_DATA_IV(vec_old);
	ASSERT_EQ(res, true);
	ASSERT_EQ(vec_old.size(), 16);

	std::vector<unsigned char> vec_new = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	res = db.set_DATA_IV(vec_new);
	ASSERT_EQ(res, true);

	std::vector<unsigned char> vec_new_read;
	res = db.get_DATA_IV(vec_new_read);
	ASSERT_EQ(res, true);

	ASSERT_EQ(vec_new, vec_new_read);

	res = db.set_DATA_IV(vec_old);
	ASSERT_EQ(res, true);
}

TEST(Database, config_data_key){
	DataBase db;
	std::vector<unsigned char> vec_old;
	
	bool res = db.get_DATA_KEY(vec_old);
	ASSERT_EQ(res, true);
	ASSERT_EQ(vec_old.size(), 16);

	std::vector<unsigned char> vec_new = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	res = db.set_DATA_KEY(vec_new);
	ASSERT_EQ(res, true);

	std::vector<unsigned char> vec_new_read;
	res = db.get_DATA_KEY(vec_new_read);
	ASSERT_EQ(res, true);

	ASSERT_EQ(vec_new, vec_new_read);

	res = db.set_DATA_KEY(vec_old);
	ASSERT_EQ(res, true);
}

TEST(Database, journal_insert){
	DataBase db;
	bool res = db.record_JOURNAL("TEST","GTEST","TEST MESSAGE");
	ASSERT_EQ(res, true);

}
