#pragma once
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/URI.h>
#include <Poco/Foundation.h>
#include <Poco/RegularExpression.h>
#include "debug.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "gwthread.hpp"

using namespace Poco::Net;
using namespace Poco;
using namespace std;

class API_default : public HTTPRequestHandler{
public:
	API_default(vector<string> params, GWThread *gw){
		this->params = params;
		this->gwthread =gw;
	};

	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("text/html"); 
		std::ostream& ostr = resp.send();
		ostr << "<b1>HTTPTimeServer powered by <\\b1>";
	};

private:
	GWThread* gwthread;
	vector<string> params;
};
