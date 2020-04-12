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
#include "database.hpp"
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>

using namespace Poco::Net;
using namespace Poco;
using namespace std;
using Poco::JSON::Stringifier;
using Poco::JSON::Object;
using Poco::JSON::Array;

class API_prototype : public HTTPRequestHandler{
public:
		API_prototype(vector<string> params, GWThread *gw){
		this->params = params;
		this->gwthread =gw;
	};
	
	GWThread* gwthread;
	vector<string> params;
	Object JSON_ERROR;
	Object JSON_ANSWER;
};

