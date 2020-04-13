
#pragma once
#include "API_prototype.hpp"

class API_transiver_config_param : public API_prototype{
public:
	API_transiver_config_param(vector<string> params, GWThread *gw)
		:API_prototype(params, gw){};

	void getHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();
		DataBase db;

		// Извлекаем имя параметра переданного в URL
		string param_name = params[2];
		// Допустимые параметры
	   	string templ = "sys_channel sync_channel stream_iv stream_key data_iv data_key pan_id";	
		string search_result;
		if(!StringInTemplate(param_name, templ, search_result)){
			ApiSetAnswer(true, string("URL config parameter not found:") + param_name);
			JSON_ANSWER.stringify(ostr);
			return;
		};

		if (search_result.compare("sys_channel") ==0){
			unsigned char sys_channel;
			if (db.get_SYS_CHANNEL(sys_channel)){
				JSON_ANSWER.set("value", sys_channel);
				ApiSetAnswer(false, "");
				JSON_ANSWER.stringify(ostr);
				return;
			} 
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}

		if (search_result.compare("sync_channel") ==0){
			unsigned char sync_channel;
			if (db.get_SYNC_CHANNEL(sync_channel)){
				JSON_ANSWER.set("value", sync_channel);
				ApiSetAnswer(false, "");
				JSON_ANSWER.stringify(ostr);
				return;
			} 
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}

		if (search_result.compare("pan_id") ==0){
			unsigned char pan_id;
			if (db.get_PAN_ID(pan_id)){
				JSON_ANSWER.set("value", pan_id);
				ApiSetAnswer(false, "");
				JSON_ANSWER.stringify(ostr);
				return;
			} 
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}

		if (search_result.compare("stream_iv") ==0){
			vector<unsigned char> stream_iv;
			if (db.get_STREAM_IV(stream_iv)){
				JSON_ANSWER.set("value", VectorToStr(stream_iv));
				ApiSetAnswer(false, "");
				JSON_ANSWER.stringify(ostr);
				return;
			} 
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}
		
		if (search_result.compare("stream_key") ==0){
			vector<unsigned char> stream_key;
			if (db.get_STREAM_KEY(stream_key)){
				JSON_ANSWER.set("value", VectorToStr(stream_key));
				ApiSetAnswer(false, "");
				JSON_ANSWER.stringify(ostr);
				return;
			} 
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}
		
		if (search_result.compare("data_iv") ==0){
			vector<unsigned char> data_iv;
			if (db.get_DATA_IV(data_iv)){
				JSON_ANSWER.set("value", VectorToStr(data_iv));
				ApiSetAnswer(false, "");
				JSON_ANSWER.stringify(ostr);
				return;
			} 
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}
		
		if (search_result.compare("data_key") ==0){
			vector<unsigned char> data_key;
			if (db.get_DATA_KEY(data_key)){
				JSON_ANSWER.set("value", VectorToStr(data_key));
				ApiSetAnswer(false, "");
				JSON_ANSWER.stringify(ostr);
				return;
			} 
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}

		ApiSetAnswer(true, "URL parameter in template? but not handlel.");
		JSON_ANSWER.stringify(ostr);
	};

	void putHandler(HTTPServerRequest &req, HTTPServerResponse &resp){
		resp.setContentType("application/json"); 
		std::ostream& ostr = resp.send();
		DataBase db;

//***************************************************
// Извлекаем и проверяем параметры строкиГКД
//***************************************************
		string param_name = params[2];
		// Допустимые параметры
	   	string templ = "sys_channel sync_channel stream_iv stream_key data_iv data_key pan_id";	
		string search_result;
		if(!StringInTemplate(param_name, templ, search_result)){
			ApiSetAnswer(true, string("URL config parameter not found:") + param_name);
			JSON_ANSWER.stringify(ostr);
			return;
		};

//***************************************************
//Разбираем входную строку команды PUT
//***************************************************
		Var value;
	
		bool parse_error = false;
		string message_error = "";
		Parser parser;
		try{	
			Var result = parser.parse(req.stream());
			Object::Ptr object = result.extract<Object::Ptr>();
			value = object->get("value");
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
//Обработка запросов
//Запрос JSON {"value" : val}
//***************************************************
	
		if (search_result.compare("sys_channel") ==0){
			unsigned char sys_channel;
			try{	
				sys_channel = value.convert<unsigned char>();
			} 
			catch(Poco::Exception& exc){
				message_error = exc.displayText();
				ApiSetAnswer(true, message_error.c_str());
				JSON_ANSWER.stringify(ostr);
				return;
			}
			if (db.set_SYS_CHANNEL(sys_channel)){
				ApiSetAnswer(false, "Database parameter sys_channel changed");
				JSON_ANSWER.stringify(ostr);
				return;
			}
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}


		if (search_result.compare("sync_channel") ==0){
			unsigned char sync_channel;
			try{	
				sync_channel = value.convert<unsigned char>();
			} 
			catch(Poco::Exception& exc){
				message_error = exc.displayText();
				ApiSetAnswer(true, message_error.c_str());
				JSON_ANSWER.stringify(ostr);
				return;
			}
			if (db.set_SYNC_CHANNEL(sync_channel)){
				ApiSetAnswer(false, "Database parameter sync_channel changed");
				JSON_ANSWER.stringify(ostr);
				return;
			}
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}
		
		if (search_result.compare("pan_id") ==0){
			unsigned char pan_id;
			try{	
				pan_id = value.convert<unsigned char>();
			} 
			catch(Poco::Exception& exc){
				message_error = exc.displayText();
				ApiSetAnswer(true, message_error.c_str());
				JSON_ANSWER.stringify(ostr);
				return;
			}
			if (db.set_PAN_ID(pan_id)){
				ApiSetAnswer(false, "Database parameter pan_id changed");
				JSON_ANSWER.stringify(ostr);
				return;
			}
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}

		if (search_result.compare("stream_iv") ==0){
			vector<unsigned char> stream_iv;
			string stream_iv_str;
			try{	
				stream_iv_str = value.convert<string>();
			} 
			catch(Poco::Exception& exc){
				message_error = exc.displayText();
				ApiSetAnswer(true, message_error.c_str());
				JSON_ANSWER.stringify(ostr);
				return;
			}

			if (!convert_to_vector_16(stream_iv_str, stream_iv)){
				ApiSetAnswer(true, "Cant convert to vector. Error in string format.");
				JSON_ANSWER.stringify(ostr);
				return;
			};

			if (db.set_STREAM_IV(stream_iv)){
				ApiSetAnswer(false, "Database parameter stream changed");
				JSON_ANSWER.stringify(ostr);
				return;
			}
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}
		
		if (search_result.compare("stream_key") ==0){
			vector<unsigned char> stream_key;
			string stream_key_str;
			try{	
				stream_key_str = value.convert<string>();
			} 
			catch(Poco::Exception& exc){
				message_error = exc.displayText();
				ApiSetAnswer(true, message_error.c_str());
				JSON_ANSWER.stringify(ostr);
				return;
			}

			if (!convert_to_vector_16(stream_key_str, stream_key)){
				ApiSetAnswer(true, "Cant convert to vector. Error in string format.");
				JSON_ANSWER.stringify(ostr);
				return;
			};

			if (db.set_STREAM_KEY(stream_key)){
				ApiSetAnswer(false, "Database parameter stream changed");
				JSON_ANSWER.stringify(ostr);
				return;
			}
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}
	
	
		if (search_result.compare("data_key") ==0){
			vector<unsigned char> data_key;
			string data_key_str;
			try{	
				data_key_str = value.convert<string>();
			} 
			catch(Poco::Exception& exc){
				message_error = exc.displayText();
				ApiSetAnswer(true, message_error.c_str());
				JSON_ANSWER.stringify(ostr);
				return;
			}

			if (!convert_to_vector_16(data_key_str, data_key)){
				ApiSetAnswer(true, "Cant convert to vector. Error in string format.");
				JSON_ANSWER.stringify(ostr);
				return;
			};

			if (db.set_DATA_KEY(data_key)){
				ApiSetAnswer(false, "Database parameter stream changed");
				JSON_ANSWER.stringify(ostr);
				return;
			}
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}
	
		if (search_result.compare("data_iv") ==0){
			vector<unsigned char> data_iv;
			string data_iv_str;
			try{	
				data_iv_str = value.convert<string>();
			} 
			catch(Poco::Exception& exc){
				message_error = exc.displayText();
				ApiSetAnswer(true, message_error.c_str());
				JSON_ANSWER.stringify(ostr);
				return;
			}

			if (!convert_to_vector_16(data_iv_str, data_iv)){
				ApiSetAnswer(true, "Cant convert to vector. Error in string format.");
				JSON_ANSWER.stringify(ostr);
				return;
			};

			if (db.set_DATA_IV(data_iv)){
				ApiSetAnswer(false, "Database parameter stream changed");
				JSON_ANSWER.stringify(ostr);
				return;
			}
			else{
				ApiSetAnswer(true, "Database read error. Look for problem in log");
				JSON_ANSWER.stringify(ostr);
				return;
			}
		}


		ApiSetAnswer(true, "URL parameter in template? but not handlel.");
		JSON_ANSWER.stringify(ostr);
	};

bool convert_to_vector_16(std::string text, std::vector<unsigned char>& vec){
	if (text.size() != 32){ // Строка содержит 16 байт в 16ричной системе
		SPDLOG_ERROR("Expected size of text is 32, but size is : {}", text.size());
		return false;
	}
	size_t pos = 0;
	std::string symb = "0123456789ABCDEFabcdef";
	text.find_first_not_of(symb, pos);
	if (pos != 0){
		SPDLOG_ERROR("Text is not byte vector: {}, pos = {}", text.c_str(), pos);
		return false;
	}
	if(vec.size() != 0){
		SPDLOG_ERROR("Reference vector size is not 0");
		return false;
	}

	std::string sub;
	int value;
	for (int i = 0; i < 16; i++){ 
		sub = text.substr(2*i, 2);
		pos = 0;
		value = stoi(sub, &pos, 16);
	//	SPDLOG_INFO("sub {}, val = {}",sub.c_str(), std::to_string(value));
		vec.push_back(value);
	//	vec.insert(vec.begin(), value);
	}
	//SPDLOG_INFO("Vector conv res : {}", vec.size() );
	return true;
}
};
