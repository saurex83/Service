#include "httpserver.hpp"
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/URI.h>
#include <Poco/Foundation.h>
#include <Poco/RegularExpression.h>
#include "debug.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "httpapi/httpapi.hpp"

static HTTPServer* http_server;
static GWThread* gwthread;

void HTTP_Server_set_GWThread(GWThread* gw){
	gwthread = gw;
}

class RequestHandlerFactory : public HTTPRequestHandlerFactory{
public:
	virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &req){
		URI uri(req.getURI());
		vector<string> params;

		if (parseURI("(?i)(/api/transiver/status$)", uri, params))
			return new API_transiver_status(params, gwthread);
	
//		if (parseURI("(?i)(/api/transiver/config/)(\\w+)$", uri, params))
//			return new API_transiver_config(params, gwthread);
	
		if (parseURI("(?i)(/api/transiver/config$)", uri, params))
			return new API_transiver_config(params, gwthread);
	
		// Ответ по умолчанию	
		return new API_default(params, gwthread);
	}

	int parseURI(const char* regexp, URI& uri, vector<string>& params){
		string uri_path(uri.getPath());
		Poco::RegularExpression regex(regexp);
		Poco::RegularExpression::MatchVec mvec;
		int matches = regex.match(uri_path, 0, mvec);	

		for (Poco::RegularExpression::Match M : mvec)
			params.push_back(uri_path.substr(M.offset, M.length));

		return matches;
	}
};

void HTTP_Server_run(){
	if (gwthread == NULL){
		SPDLOG_ERROR("Pointer to gwthread not set. Http server start abort");
		throw(std::runtime_error("Pointer to gwthread not set. Http server start abort"));
	}

    http_server = new HTTPServer 
		(
			new RequestHandlerFactory, 
			ServerSocket(9090), 
			new HTTPServerParams);
    http_server->start();
    SPDLOG_INFO("HTTP server started");
}

void HTTP_Server_stop(){
    SPDLOG_INFO("HTTP server shutting down...");
    http_server->stop();
}
