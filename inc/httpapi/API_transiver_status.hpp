#pragma once
#include "API_prototype.hpp"

class API_transiver_status : public API_prototype{
public:
	API_transiver_status(vector<string> params, GWThread *gw)
		:API_prototype(params, gw){};

	void getHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();
	
		bool gw_running = gwthread->isRunnig();
		if (gw_running)
			JSON_ANSWER.set("run", true);
		else
			JSON_ANSWER.set("run", false);
		
		ApiSetAnswer(false, "");
		JSON_ANSWER.set("thread_error", gwthread->thread_error);		
		JSON_ANSWER.stringify(ostr);
	};

	void putHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();

//***************************************************
//Разбираем входную строку команды PUT
//***************************************************
		// Выбираемые параметры из json	
		bool run;
	
		bool parse_error = false;
		string message_error = "";
		Parser parser;
		try{	
			Var result = parser.parse(req.stream());
			Object::Ptr object = result.extract<Object::Ptr>();
			Var arg = object->get("run");
			run = arg.convert<bool>();
		}
	   	catch(Poco::Exception& exc){
			parse_error = true;
			message_error = exc.displayText();
		}
	
		if (parse_error){	
			ApiSetAnswer(true, message_error.c_str());
			JSON_ANSWER.stringify(ostr);
			return;
		}

//***************************************************
//Проверим параметры запроса
//***************************************************
		bool gw_running = gwthread->isRunnig();
		if (run == true && gw_running == true){
			ApiSetAnswer(true, "Gateway allready running");
			JSON_ANSWER.stringify(ostr);
			return;
		}
		else if (run == false && gw_running == false){
			ApiSetAnswer(true, "Gateway allready stoped");
			JSON_ANSWER.stringify(ostr);
			return;
		}

		
		if (run){
			SPDLOG_INFO("Restapi start gateway thread");
			ApiSetAnswer(false, "Gateway requested to start");
			gwthread->start();
		}
		else{
			SPDLOG_INFO("Restapi stop gateway thread");
			ApiSetAnswer(false, "Gateway requested to stop");
			gwthread->stop();
		}

			
		JSON_ANSWER.stringify(ostr);
	};
};
