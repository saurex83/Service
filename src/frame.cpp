#include "frame.hpp"
#include <cstring>
#include "debug.hpp"
#include <stdexcept>

Frame::Frame(){
}

/**
 * @brief Добавить заголовок
 *
 * Заголовок вставляется перед в начало существующих данных.
 * Если размер превышет максимальный, то будет выбрашено исключение
 * runtime_error
 * @param head указатель на вставляемые данные
 */
void Frame::addHeader(vector<unsigned char> &head){
  int new_len = payload.size() + head.size();
  if (new_len > MAX_PAYLOAD_SIZE)
    throw(runtime_error("Add header data more then MAX_PAYLOAD_SIZE"));
 
 	payload.reserve(new_len);
	payload.insert(payload.begin(), head.begin(), head.end());	
}

/**
 * @brief Удаление начала заголовка
 *
 * Удаляет данные с начала заголовка. Если размер удаляемых данных 
 * превышает размер заголовка, то будет выброшенно исключение runtime_error
 * @param len размер удаляемых данных
 */
void Frame::delHeader(size_t len){
	if (!len){
		SPDLOG_WARN("Request delete zero len");
		return;
	}

	if (len > payload.size()){
		SPDLOG_ERROR("Payload size less then requested delete size");
		throw(runtime_error("Payload size less then requested delete size"));
	}

	payload.erase(payload.begin(), payload.begin() + len);
}

/**
 * @brief Разбирает сырые данные
 *
 * Сырые данные состоят из метаданных, байта длинны и полезной нагрузки.
 * @param raw указатель на сырые данные
 */
void Frame::parseRaw(vector<unsigned char>& raw){
	// Проверим размер. Как минимум дожны быть метаданные и байт длинны
	if (raw.size() <= meta.size() + 1){
		SPDLOG_ERROR("Raw data size is too small: {}", raw.size());	
		throw(runtime_error("Raw data size is too small"));	
	};
	
	vector<unsigned char> raw_meta;
	raw_meta.reserve(meta.size());
	
	raw_meta.insert(raw_meta.begin(),
		   	raw.begin(), 
			raw.begin() + meta.size());

	unsigned char len_byte = raw[meta.size()];
	size_t calc_size = meta.size() + 1 + len_byte;
	if (raw.size() !=calc_size){
		SPDLOG_ERROR("Raw data has invalid size:{}. Calculated size:{}",
				raw.size(), calc_size );
		throw(runtime_error("Calculated size not equal real size"));
	};

	payload.clear();
	payload.reserve(len_byte);
	payload.insert(payload.begin(), raw.begin() + meta.size() + 1, raw.end());

	// Копируем метаданные
	meta.parseRaw(raw_meta);
};

/**
 * @brief Размер полезной нагрузки
 *
 * @return размер в байтах
 */
size_t Frame::size(){
	return payload.size();
};


/**
 * @brief Преобразует кадр в вектор байт
 *
 * @param raw указатель на вектор байт
 */
void Frame::convertRaw(vector<unsigned char>& raw){
	raw.clear();
	meta.convertRaw(raw);

	raw.push_back(payload.size());

	raw.insert(raw.end(),
		   	payload.begin(), 
			payload.end());
};
