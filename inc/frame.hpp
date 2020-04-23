#pragma once
#include <exception>
#include <cstdint> 
#include <string>
#include <vector>
#include "meta.hpp"

using namespace std;

/**
 * @brief Максимальный размер полезной нагрузки
 */
#define MAX_PAYLOAD_SIZE 127


class Frame{
	public:
		Meta meta;
		vector<unsigned char> payload;

		Frame();
		void addHeader(vector<unsigned char> &head);
		void addHeader(unsigned char *head, size_t len);
		void parseRaw(vector<unsigned char>& raw);
		void convertRaw(vector<unsigned char>& raw);
		void delHeader(size_t len);
		size_t size();
};
