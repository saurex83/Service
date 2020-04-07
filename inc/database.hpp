#pragma once
#include <libpq-fe.h>
#include <memory>

class DataBase {
	public:
		DataBase();
		~DataBase();
	private:
		std::shared_ptr<PGconn>  m_connection;
};

