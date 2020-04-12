#pragma once
#include "API_prototype.hpp"

class API_transiver_config : public API_prototype{
public:
	API_transiver_config(vector<string> params, GWThread *gw)
		:API_prototype(params, gw){};

	virtual void getHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();
	
		JSON_ERROR.set("error", true);
		JSON_ERROR.set("message", "TRANSIVER CONFIG");
		JSON_ANSWER.set("result", JSON_ERROR);
		JSON_ANSWER.stringify(ostr);
	};
};

