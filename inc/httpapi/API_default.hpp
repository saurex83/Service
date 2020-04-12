#pragma once
#include "API_prototype.hpp"

class API_default : public API_prototype{
public:
	API_default(vector<string> params, GWThread *gw)
		:API_prototype(params, gw){};

	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("text/html"); 
		std::ostream& ostr = resp.send();
		ostr << "<b1>HTTPTimeServer powered by <\\b1>";
	};
};
