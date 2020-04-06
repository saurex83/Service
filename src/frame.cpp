#include "frame.hpp"
#include <cstring>

Frame::Frame(){
	memset((void*)&this->meta, 0x00, sizeof(meta));
	memset((void*)this->payload, 0x00, MAX_PAYLOAD_SIZE);
	this->len = 0;
}

void Frame::addHeader(unsigned char *head, char len){
  int new_len = this->len + len;
  if (!(new_len < MAX_PAYLOAD_SIZE))
    throw(FrameError("Add header data more then MAX_PAYLOAD_SIZE"));
  
  // Сдвинем данные на размер вставки при необходимости
  if (this->len != 0)
    mem_move(&this->payload[len], this->payload, this->len);
	
  // Скопируем новые данные
  memcpy(this->payload, head, len);
  this->len = new_len;
}

void Frame::mem_move(unsigned char* dst, unsigned char* src, size_t len){
	for (size_t i = 0; i < len; i++)
		dst[len - i - 1] = src[len - i - 1];
}

void Frame::delHeader(char len){
  if (len > this->len)
    throw(FrameError("Payload in frame is less then len. Impossible delete"));
	memcpy(this->payload, &this->payload[len], len);
	memset(&this->payload[len], 0x00, this->len - len);
	this->len -= len;
}
