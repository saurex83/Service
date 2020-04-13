#pragma once
#include "API_prototype.hpp"

class API_transiver_config : public API_prototype{
public:
	API_transiver_config(vector<string> params, GWThread *gw)
		:API_prototype(params, gw){};

	virtual void getHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();
		DataBase db;
		
		unsigned char sys_channel;
		unsigned char sync_channel;
		unsigned char pan_id;
		vector<unsigned char> stream_iv;
		vector<unsigned char> stream_key;
		vector<unsigned char> data_iv;
		vector<unsigned char> data_key;

		bool get_res = true;
		get_res &= db.get_SYS_CHANNEL(sys_channel);	
		get_res &= db.get_SYNC_CHANNEL(sync_channel);	
		get_res &= db.get_PAN_ID(pan_id);
		get_res &= db.get_STREAM_IV(stream_iv);
		get_res &= db.get_STREAM_KEY(stream_key);
		get_res &= db.get_DATA_IV(data_iv);
		get_res &= db.get_DATA_KEY(data_key);

		if (!get_res){
			ApiSetAnswer(true, "Database read error. Look for problem in log");
			JSON_ANSWER.stringify(ostr);
			return;
		}

		ApiSetAnswer(false, "Database config table readed");
		JSON_ANSWER.set("sys_channel", sys_channel);
		JSON_ANSWER.set("sync_channel", sync_channel);
		JSON_ANSWER.set("pan_id", pan_id);
		JSON_ANSWER.set("stream_iv", VectorToStr(stream_iv));
		JSON_ANSWER.set("stream_key", VectorToStr(stream_key));
		JSON_ANSWER.set("data_iv", VectorToStr(data_iv));
		JSON_ANSWER.set("data_key", VectorToStr(data_key));
		JSON_ANSWER.stringify(ostr);
	};
	
};

