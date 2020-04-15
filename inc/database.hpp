#pragma once
#include <libpq-fe.h>
#include <memory>
#include <string>
#include <vector>


struct JRecord{
	std::string lvl;
	std::string src;
	std::string msg;
	std::string tsz;
};

class DataBase {
	public:
		DataBase();
		~DataBase();
		void dropTables();
		void createTables();
		bool get_SYS_CHANNEL(unsigned char& SYS_CHANNEL);
		bool set_SYS_CHANNEL(unsigned char& SYS_CHANNEL);
		bool get_SYNC_CHANNEL(unsigned char& SYNC_CHANNEL);
		bool set_SYNC_CHANNEL(unsigned char& SYNC_CHANNEL);
		bool get_PAN_ID(unsigned char& PAN_ID);
		bool set_PAN_ID(unsigned char& PAN_ID);
		bool get_STREAM_IV(std::vector<unsigned char>& STREAM_IV);
		bool set_STREAM_IV(std::vector<unsigned char>& STREAM_IV);
		bool set_STREAM_KEY(std::vector<unsigned char>& STREAM_KEY);
		bool get_STREAM_KEY(std::vector<unsigned char>& STREAM_KEY);
		bool get_DATA_IV(std::vector<unsigned char>& DATA_IV);
		bool set_DATA_IV(std::vector<unsigned char>& DATA_IV);
		bool get_DATA_KEY(std::vector<unsigned char>& DATA_KEY);
		bool set_DATA_KEY(std::vector<unsigned char>& DATA_KEY);
		bool record_JOURNAL(std::string lvl, std::string src, std::string msg);
		bool getLastRecords(std::vector<JRecord> &j_vec, size_t n_rec);
		bool getLicense(std::string &lic);
	private:
		bool getValueFromCONFIG(const char* NAME, std::string& result);
		bool setValueToCONFIG(const char* NAME, std::string& value);
		std::shared_ptr<PGconn>  m_connection;
		bool tableActions(const char* sqlcmd);
		bool convert_to_uchar(std::string text, unsigned char& value);
		bool convert_to_vector_16(std::string text, std::vector<unsigned char>& vec);
};

