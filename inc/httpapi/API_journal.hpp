#pragma once
#include "API_prototype.hpp"

class API_journal : public API_prototype{
public:
	API_journal(vector<string> params, GWThread *gw)
		:API_prototype(params, gw){};

	void getHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();
		DataBase db;
//******************************************************** 
// Извлечем параметры запроса из URL
//********************************************************
		size_t limit = 5;
		
		URI uri(req.getURI());
		Poco::URI::QueryParameters QP = uri.getQueryParameters();
		for (std::pair<std::string,std::string> pair : QP){
			if (pair.first.compare("limit")==0)
				limit = std::stoi(pair.second);
		};

		std::vector<JRecord> j_rec;
		if(!db.getLastRecords(j_rec, limit)){
			ApiSetAnswer(true, "Database read error. Look for problem in log");
			JSON_ANSWER.stringify(ostr);
			return;
		}

		Array poco_arr;	
		Object poco_j_rec;

		for (JRecord rec : j_rec){
			poco_j_rec.set("lvl", rec.lvl);
			poco_j_rec.set("src", rec.src);
			poco_j_rec.set("msg", rec.msg);
			poco_j_rec.set("tsz", rec.tsz);
			poco_arr.add(poco_j_rec);
		}

		JSON_ANSWER.set("records",poco_arr);

		ApiSetAnswer(false, "");

		JSON_ANSWER.stringify(ostr);
	};	

	void putHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();
		DataBase db;

//***************************************************
//Разбираем входную строку команды PUT
//***************************************************
		Var msg;
		Var lvl;
		Var src;

		bool parse_error = false;
		string message_error = "";
		Parser parser;
		try{	
			Var result = parser.parse(req.stream());
			Object::Ptr object = result.extract<Object::Ptr>();
			msg = object->get("msg");
			lvl = object->get("lvl");
			src = object->get("src");
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

		string msg_str;
		string lvl_str;
		string src_str;

		try{
			msg_str = msg.convert<string>();
			lvl_str = lvl.convert<string>();
			src_str = src.convert<string>();
		}
		catch(Poco::Exception& exc){
			message_error = exc.displayText();
			ApiSetAnswer(true, message_error.c_str());
			JSON_ANSWER.stringify(ostr);
			return;
		};
		
		if(!db.record_JOURNAL(lvl_str, src_str, msg_str)){
			ApiSetAnswer(true, "Database read error. Look for problem in log");
			JSON_ANSWER.stringify(ostr);
			return;
		};

		ApiSetAnswer(false, "Record added to journal");
		JSON_ANSWER.stringify(ostr);
	}
};

