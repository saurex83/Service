#include "httpserver.hpp"
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include "debug.hpp"
#include <iostream>

using namespace Poco::Net;
using namespace std;

//static HTTPServer* http_server;

class RequestHandler : public HTTPRequestHandler{
public:
	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp){
		ostream& out = resp.send();
		out << "<h1>Hello world!</h1>";
		out.flush();
	};
};

class RequestHandlerFactory : public HTTPRequestHandlerFactory{
public:
	virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &){
		return new RequestHandler;
	}
};


void HTTP_Server_run(){
    HTTPServer s 
		(
			new RequestHandlerFactory, 
			ServerSocket(9090), 
			new HTTPServerParams);

    SPDLOG_INFO("REST API server started");
    s.start();

	while(true);

    SPDLOG_INFO("REST API server shutting down...");
    s.stop();
}
