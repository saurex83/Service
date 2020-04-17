#include "meta.hpp"
#include <stdexcept>
#include <cstring>

Meta::Meta(){
	memset(&meta, 0x00, sizeof(meta));
};


void Meta::convertRaw(vector<unsigned char>& raw){
	unsigned char* ptr = (unsigned char*)&meta;
	for (size_t i = 0; i < sizeof(meta); i++)
		raw.push_back(*ptr++);
};

/**
 * @brief Заполняет внутреннию структуру на основе сырых данных.
 *
 * Если размер данных не соответсвует размеру структуры, 
 * будет выброшенно исключечние runtime_error
 * @param raw указатель на входные данные 
 */
void Meta::parseRaw(vector<unsigned char>& raw){
	if (raw.size() != sizeof(meta))
		throw(runtime_error("Size of raw data not equal size of meta"));
	memcpy(&meta, &raw.front(), sizeof(meta));
};

size_t Meta::size(){
	return sizeof(meta);
};
