#include "database.hpp"
#include "ServiceConfig.hpp"
#include "debug.hpp"
#include "db_tables_def.hpp"
#include <sstream>
#include <boost/format.hpp>
#include <iostream>
#include  <iomanip>

DataBase::DataBase(){
	ServiceConfig& cfg = ServiceConfig::getInstance();	
    
	std::string m_dbhost = cfg.db_ip;
    int         m_dbport = cfg.db_port;
    std::string m_dbname = cfg.db_name;
    std::string m_dbuser = cfg.db_login;
    std::string m_dbpass = cfg.db_password;

	m_connection.reset(
			PQsetdbLogin(m_dbhost.c_str(),
			   	std::to_string(m_dbport).c_str(),			   	
				nullptr, nullptr, 
				m_dbname.c_str(), 
				m_dbuser.c_str(),
			   	m_dbpass.c_str()),
		   		&PQfinish );

    if (PQstatus( m_connection.get() ) != CONNECTION_OK && PQsetnonblocking(m_connection.get(), 1) != 0 )
    {
		SPDLOG_INFO(PQerrorMessage(m_connection.get()));  
		throw std::runtime_error(PQerrorMessage(m_connection.get()));
    }

//	SPDLOG_INFO("Connected to dababase");
}

DataBase::~DataBase(){
//	SPDLOG_INFO("Connection with database closed");
}



bool DataBase::tableActions(const char* sqlcmd){
	PGresult   *res;

	res = PQexec(this->m_connection.get(), sqlcmd);
  
	if (PQresultStatus(res) != PGRES_COMMAND_OK){
		SPDLOG_ERROR("Create table sqlcmd faild: {}", PQerrorMessage(this->m_connection.get()));
		PQclear(res);
		return false;
	}

	PQclear(res);
	return true;
}

void DataBase::dropTables(){
	if (tableActions("DROP TABLE IF EXISTS CONFIG"))
		SPDLOG_INFO("Table CONFIG droped");
	else
		SPDLOG_ERROR("Error while table CONFIG droping");

	if (tableActions("DROP TABLE IF EXISTS JOURNAL"))
		SPDLOG_INFO("Table JOURNAL droped");
	else
		SPDLOG_ERROR("Error while table JOURNAL droping");
}

void DataBase::createTables(){
	if (tableActions(DB_CONFIG))
		SPDLOG_INFO("Table CONFIG created");
	else
		SPDLOG_ERROR("Error while table CONFIG creating");
	
	if (tableActions(DB_JOURNAL))
		SPDLOG_INFO("Table JOURNAL created");
	else
		SPDLOG_ERROR("Error while table JOURNAL creating");
	
	if (tableActions(DB_NODELIST))
		SPDLOG_INFO("Table NODELIST created");
	else
		SPDLOG_ERROR("Error while table NODELIST creating");
	
	if (tableActions(DB_NODE_STATISTIC))
		SPDLOG_INFO("Table NODE_STATISTIC created");
	else
		SPDLOG_ERROR("Error while table NODE_STATISTIC creating");
	
	if (tableActions(DB_MEASURING_POINT))
		SPDLOG_INFO("Table MEASURING_POINT created");
	else
		SPDLOG_ERROR("Error while table MEASURING_POINT creating");
	
	if (tableActions(DB_DATA_FLOAT))
		SPDLOG_INFO("Table DATA_FLOAT created");
	else
		SPDLOG_ERROR("Error while table DATA_FLOAT creating");
	
	if (tableActions(DB_DATA_INT))
		SPDLOG_INFO("Table DATA_INT created");
	else
		SPDLOG_ERROR("Error while table DATA_INT creating");
	
	if (tableActions(DB_DATA_BOOL))
		SPDLOG_INFO("Table DATA_BOOL created");
	else
		SPDLOG_ERROR("Error while table DATA_BOOL creating");
	
	if (tableActions(DB_NODE_LAST_COMM))
		SPDLOG_INFO("Table NODE_LAST_COMM created");
	else
		SPDLOG_ERROR("Error while table NODE_LAST_COMM creating");
	
	if (tableActions(DB_LICENSE))
		SPDLOG_INFO("Table LICENSE created");
	else
		SPDLOG_ERROR("Error while table LICENSE creating");
	
	if (tableActions(DB_VIRTUALSENSOR))
		SPDLOG_INFO("Table VIRTUALSENSOR created");
	else
		SPDLOG_ERROR("Error while table VIRTUALSENSOR creating");
	
	if (tableActions(DB_VIRT_DATA_FLOAT))
		SPDLOG_INFO("Table VIRT_DATA_FLOAT created");
	else
		SPDLOG_ERROR("Error while table VIRT_DATA_FLOAT creating");
	
	if (tableActions(DB_VIRT_DATA_INT))
		SPDLOG_INFO("Table VIRT_DATA_INT created");
	else
		SPDLOG_ERROR("Error while table VIRT_DATA_INT creating");
	
	if (tableActions(DB_VIRT_DATA_BOOL))
		SPDLOG_INFO("Table VIRT_DATA_BOOL created");
	else
		SPDLOG_ERROR("Error while table DATA_BOOL creating");

	if (tableActions(DB_ENERGYSCAN))
		SPDLOG_INFO("Table VIRT_DATA_BOOL created");
	else
		SPDLOG_ERROR("Error while table DATA_BOOL creating");
	
};

bool DataBase::setValueToCONFIG(const char* NAME, std::string& value){
	PGresult   *res;
	std::string sql_req ="UPDATE CONFIG SET VALUE='" + value + "' WHERE NAME = '" + std::string(NAME) + "';";
	res = PQexec(this->m_connection.get(), sql_req.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK){
		SPDLOG_ERROR("SQL REQUEST = {}. ERROR =  ", sql_req.c_str(), PQerrorMessage(this->m_connection.get()));	
		PQclear(res);
		return false;
	}
		PQclear(res);
		return true;
};

bool DataBase::getValueFromCONFIG(const char* NAME, std::string& result){
	PGresult   *res;
	
	std::string sql_req ="SELECT VALUE FROM CONFIG WHERE NAME ='" + std::string(NAME) + "';";

	res = PQexec(this->m_connection.get(), sql_req.c_str());
 
	if (PQresultStatus(res) != PGRES_TUPLES_OK){
		SPDLOG_ERROR("SQL REQUEST = {}. ERROR =  ", sql_req.c_str(), PQerrorMessage(this->m_connection.get()));	
		PQclear(res);
		return false;
	}

 	int nFields = PQnfields(res);
	int nTuples = PQntuples(res);
	
	if (nTuples != 1){
		SPDLOG_ERROR("nTuples = {}. Must be 1", nTuples);
		PQclear(res);
		return false;
	}

	if (nFields != 1){
		SPDLOG_ERROR("nFields = {}. Must be 1", nFields);
		PQclear(res);
		return false;
	}

	char* value = PQgetvalue(res, 0, 0);
	result = value;
	PQclear(res);
	return true;
}

bool DataBase::convert_to_uchar(std::string text,unsigned char& value){
	int res_val;
	try{
		res_val = std::stoi(text);
	}
	catch(std::invalid_argument){
		SPDLOG_ERROR("Invalid stoi from : {}", res_val);
		return false;
	}
	catch(std::out_of_range){
		SPDLOG_ERROR("Convertion to int out of range from : {}", res_val);
		return false;
	}
	if (res_val >=0 && res_val<=0xff){
		value = res_val;
		return true;
	}
	return false;
};

bool DataBase::convert_to_vector_16(std::string text, std::vector<unsigned char>& vec){
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

//***********************************************************
//SYS_CHANNEL
//***********************************************************
bool DataBase::get_SYS_CHANNEL(unsigned char& SYS_CHANNEL){
	std::string answer; 
	bool res = getValueFromCONFIG("SYS_CHANNEL", answer);
	if (!res)
		return false;
	if (convert_to_uchar(answer, SYS_CHANNEL))
		return true;
	return false;
}

bool DataBase::set_SYS_CHANNEL(unsigned char& SYS_CHANNEL){
	std::string arg = std::to_string(SYS_CHANNEL);
	return setValueToCONFIG("SYS_CHANNEL", arg);
}

//***********************************************************
//SYNC_CHANNEL
//***********************************************************
bool DataBase::get_SYNC_CHANNEL(unsigned char& SYNC_CHANNEL){
	std::string answer; 
	bool res = getValueFromCONFIG("SYNC_CHANNEL", answer);
	if (!res)
		return false;
	if (convert_to_uchar(answer, SYNC_CHANNEL))
		return true;
	return false;
}

bool DataBase::set_SYNC_CHANNEL(unsigned char& SYNC_CHANNEL){
	std::string arg = std::to_string(SYNC_CHANNEL);
	return setValueToCONFIG("SYNC_CHANNEL", arg);
}

//***********************************************************
//PAN_ID
//***********************************************************
bool DataBase::get_PAN_ID(unsigned char& PAN_ID){
	std::string answer; 
	bool res = getValueFromCONFIG("PAN_ID", answer);
	if (!res)
		return false;
	if (convert_to_uchar(answer, PAN_ID))
		return true;
	return false;
}

bool DataBase::set_PAN_ID(unsigned char& PAN_ID){
	std::string arg = std::to_string(PAN_ID);
	return setValueToCONFIG("PAN_ID", arg);
}

//***********************************************************
//STREAM_IV
//***********************************************************
bool DataBase::get_STREAM_IV(std::vector<unsigned char>& STREAM_IV){
	std::string answer; 
	bool res = getValueFromCONFIG("STREAM_IV", answer);
	if (!res)
		return false;
	if (convert_to_vector_16(answer, STREAM_IV))
		return true;
	return false;
}

bool DataBase::set_STREAM_IV(std::vector<unsigned char>& STREAM_IV){
	if(STREAM_IV.size() != 16){
		SPDLOG_ERROR("Reference vector size is not 16");
		return false;
	}
	std::string value_str;
	std::stringstream ss;
	for (unsigned char item : STREAM_IV)
		ss << std::setfill('0') << std::setw(2) << std::hex << (unsigned)(item);
	
	ss >> value_str;
	
	if (value_str.size() != 32){
		SPDLOG_ERROR("Vector to hex string conversion error : {}, size = {}",
		value_str.c_str(), value_str.size());
		return false;
	}
	return setValueToCONFIG("STREAM_IV", value_str);
}

//***********************************************************
//STREAM_KEY
//***********************************************************
bool DataBase::get_STREAM_KEY(std::vector<unsigned char>& STREAM_KEY){
	std::string answer; 
	bool res = getValueFromCONFIG("STREAM_KEY", answer);
	if (!res)
		return false;
	if (convert_to_vector_16(answer, STREAM_KEY))
		return true;
	return false;
}

bool DataBase::set_STREAM_KEY(std::vector<unsigned char>& STREAM_KEY){
	if(STREAM_KEY.size() != 16){
		SPDLOG_ERROR("Reference vector size is not 16");
		return false;
	}
	std::string value_str;
	std::stringstream ss;
	for (unsigned char item : STREAM_KEY)
		ss << std::setfill('0') << std::setw(2) << std::hex << (unsigned)(item);
	
	ss >> value_str;
	
	if (value_str.size() != 32){
		SPDLOG_ERROR("Vector to hex string conversion error : {}, size = {}",
		value_str.c_str(), value_str.size());
		return false;
	}
	return setValueToCONFIG("STREAM_KEY", value_str);
}

//***********************************************************
//DATA_IV
//***********************************************************
bool DataBase::get_DATA_IV(std::vector<unsigned char>& DATA_IV){
	std::string answer; 
	bool res = getValueFromCONFIG("DATA_IV", answer);
	if (!res)
		return false;
	if (convert_to_vector_16(answer, DATA_IV))
		return true;
	return false;
}

bool DataBase::set_DATA_IV(std::vector<unsigned char>& DATA_IV){
	if(DATA_IV.size() != 16){
		SPDLOG_ERROR("Reference vector size is not 16");
		return false;
	}
	std::string value_str;
	std::stringstream ss;
	for (unsigned char item : DATA_IV)
		ss << std::setfill('0') << std::setw(2) << std::hex << (unsigned)(item);
	
	ss >> value_str;
	
	if (value_str.size() != 32){
		SPDLOG_ERROR("Vector to hex string conversion error : {}, size = {}",
		value_str.c_str(), value_str.size());
		return false;
	}
	return setValueToCONFIG("DATA_IV", value_str);
}

//***********************************************************
//DATA_KEY
//***********************************************************
bool DataBase::get_DATA_KEY(std::vector<unsigned char>& DATA_KEY){
	std::string answer; 
	bool res = getValueFromCONFIG("DATA_KEY", answer);
	if (!res)
		return false;
	if (convert_to_vector_16(answer, DATA_KEY))
		return true;
	return false;
}

bool DataBase::set_DATA_KEY(std::vector<unsigned char>& DATA_KEY){
	if(DATA_KEY.size() != 16){
		SPDLOG_ERROR("Reference vector size is not 16");
		return false;
	}
	std::string value_str;
	std::stringstream ss;
	for (unsigned char item : DATA_KEY)
		ss << std::setfill('0') << std::setw(2) << std::hex << (unsigned)(item);
	
	ss >> value_str;
	
	if (value_str.size() != 32){
		SPDLOG_ERROR("Vector to hex string conversion error : {}, size = {}",
		value_str.c_str(), value_str.size());
		return false;
	}
	return setValueToCONFIG("DATA_KEY", value_str);
}

//***********************************************************
//RECORD_JOURNAL
//***********************************************************
bool DataBase::record_JOURNAL(std::string lvl, std::string src, std::string msg){
	PGresult   *res;
	std::string sqlcmd;
	sqlcmd += "insert into journal(LEVEL, SOURCE, MESSAGE) VALUES (";
	sqlcmd +="'"+ lvl+"','"+ src+"','" + msg + "'";
	sqlcmd += ");";
	res = PQexec(this->m_connection.get(), sqlcmd.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK){
		SPDLOG_ERROR("Insert to JOURNAL faild: {}", PQerrorMessage(this->m_connection.get()));
		PQclear(res);
		return false;
	}

	PQclear(res);
	return true;
}

bool DataBase::getLastRecords(std::vector<JRecord> &j_vec, size_t n_rec){
	PGresult   *res;
	std::string sqlcmd;
	sqlcmd += "select timestampz, level, source, message  from journal  ORDER BY timestampz DESC limit ";
	sqlcmd += std::to_string(n_rec) + ";";
	res = PQexec(this->m_connection.get(), sqlcmd.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK){
		SPDLOG_ERROR("Select from JOURNAL faild: {}", PQerrorMessage(this->m_connection.get()));
		PQclear(res);
		return false;
	}

 	int nFields = PQnfields(res);
	int nTuples = PQntuples(res);
	
//	if (nTuples != 1){
//		SPDLOG_ERROR("nTuples = {}. Must be 1", nTuples);
//		PQclear(res);
//		return false;
//	}

	if (nFields != 4){
		SPDLOG_ERROR("nFields = {}. Must be 4", nFields);
		PQclear(res);
		return false;
	}

	JRecord j_rec;
	for (int i = 0; i < nTuples; i++){
		j_rec.tsz = PQgetvalue(res, i, 0); 
		j_rec.lvl = PQgetvalue(res, i, 1); 
		j_rec.src = PQgetvalue(res, i, 2); 
		j_rec.msg = PQgetvalue(res, i, 3); 
		j_vec.push_back(j_rec);
	}

	PQclear(res);
	return true;
};


//***********************************************************
//GET LICENSE
//***********************************************************
bool DataBase::getLicense(std::string &lic){
	PGresult   *res;
	std::string sqlcmd;
	sqlcmd += "select TEXT from license limit 1;";
	res = PQexec(this->m_connection.get(), sqlcmd.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK){
		SPDLOG_ERROR("Select from LICENSE faild: {}", PQerrorMessage(this->m_connection.get()));
		PQclear(res);
		return false;
	}
 	
	int nFields = PQnfields(res);
	int nTuples = PQntuples(res);
	
	if (nTuples != 1){
		SPDLOG_ERROR("nTuples = {}. Must be 1", nTuples);
		PQclear(res);
		return false;
	}

	if (nFields != 1){
		SPDLOG_ERROR("nFields = {}. Must be 1", nFields);
		PQclear(res);
		return false;
	}

	lic = PQgetvalue(res, 0, 0);
	return true;
};

//***********************************************************
//ENERGYSCAN
//***********************************************************
bool DataBase::set_ENERGYSCAN(std::vector<signed char>& energy){
	PGresult   *res;
	std::string sqlcmd;
	sqlcmd += "insert into ENERGYSCAN (CH11,CH12,CH13,CH14,CH15,CH16,CH17,";
	sqlcmd += "CH18,CH19,CH20,CH21,CH22,CH23,CH24,CH25,CH26,CH27,CH28) ";
	sqlcmd += "VALUES (";
	for (signed char it : energy){
		sqlcmd += std::to_string(signed(it)) + ",";
	};

	sqlcmd.resize(sqlcmd.size() - 1);
	sqlcmd += ");";

	res = PQexec(this->m_connection.get(), sqlcmd.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK){
		SPDLOG_ERROR("Insert to ENERGYSCAN faild: {}",
			   	PQerrorMessage(this->m_connection.get()));
		PQclear(res);
		return false;
	}

	PQclear(res);
	return true;
};

bool DataBase::get_NODELIST_by_MAC(std::vector<unsigned char>& mac,int& ipaddr,
	   std::string& name, std::string& comment, std::string& location,
	   bool& search_res){
	
	if (mac.size() != 8){
		SPDLOG_ERROR("Requested mac size is {}, but i need 8", mac.size());
		return false;
	};

	// Превращаем вектор с мак адресом в строку
	std::stringstream stream;
	std::string mac_str;
	for (unsigned char it : mac)
		stream << std::hex << unsigned(it);

	mac_str = stream.str();

	PGresult   *res;
	std::string sqlcmd;
	sqlcmd += "select mac, comment, location, ipaddr, name ";
	sqlcmd += "from nodelist where mac = '";
	sqlcmd += mac_str;
	sqlcmd += "'";
	
	res = PQexec(this->m_connection.get(), sqlcmd.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK){
		SPDLOG_ERROR("Get from NODELIST faild: {}",
			   	PQerrorMessage(this->m_connection.get()));
		PQclear(res);
		return false;
	}
	
	int nFields = PQnfields(res);
	int nTuples = PQntuples(res);
	
	if (nTuples != 1){
		PQclear(res);
		// Если нету нужного макадреса
		search_res = false;
		return true;
	}

	if (nFields != 5){
		SPDLOG_ERROR("nFields = {}. Must be 5", nFields);
		PQclear(res);
		return false;
	}

	ipaddr = std::stoi(PQgetvalue(res, 0, 3));
	name = PQgetvalue(res, 0, 4);
	comment = PQgetvalue(res, 0, 1);
	location = PQgetvalue(res, 0, 2);

	PQclear(res);
	search_res = true;
	return true;
};
