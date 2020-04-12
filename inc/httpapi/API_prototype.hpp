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
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Exception.h>

using namespace Poco::Net;
using namespace Poco;
using namespace std;
using Poco::JSON::Stringifier;
using Poco::JSON::Object;
using Poco::JSON::Array;
using Poco::Dynamic::Var;
using Poco::JSON::Parser;

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
	
	void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp){
		string req_method = req.getMethod();

		if (req_method == string("GET"))
			getHandler(req, resp);
		else if (req_method == string("PUT"))
			putHandler(req, resp);
		else if (req_method == string("POST"))
			postHandler(req, resp);
		else if (req_method == string("DELETE"))
			deleteHandler(req, resp);
		else
			SPDLOG_ERROR("HTTPServerRequest.getMethod undefined: {}", req_method);
	};

	virtual void getHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		SPDLOG_INFO("into get");	
		undefinedAnswer(req, resp);
	};

	virtual void putHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		undefinedAnswer(req, resp);
	};

	virtual void postHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		undefinedAnswer(req, resp);
	};
	
	virtual void deleteHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		undefinedAnswer(req, resp);
	};

	void ApiSetAnswer(bool err, string msg){
		JSON_ERROR.set("error", err);
		JSON_ERROR.set("message", msg);
		JSON_ANSWER.set("result", JSON_ERROR);
	}

	string HTTPRequestToString(HTTPServerRequest &req){
		auto & stream = req.stream();
		const size_t len = req.getContentLength();
		std::string buffer(len, 0);
		stream.read(buffer.data(), len);
		return buffer;
	};

private:
	void undefinedAnswer(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();
		JSON_ERROR.set("error", true);
		JSON_ERROR.set("message", "Method undefined");
		JSON_ANSWER.set("result", JSON_ERROR);
		JSON_ANSWER.stringify(ostr);
	};
};

