#pragma once 

#define ETH_LAY_SIZE sizeof(struct ETH_LAY)
#define HEADER_ETH_VER 1 // Версия протокола
#define PID_IP  1
#define PID_NP  2
#define PID_AUTH 3
#define IPP_UDP 0
#define IPP_TCP 1 
#define IPP_AUTH 2
#define SYS_TS	1


enum AUTH_ETH_TYPES {AUTH_CMD_REQ = 0, AUTH_CMD_RESP = 1};
enum {NP_CMD_CARD = 0, NP_CMD_CARD_REQ = 1} NP_CMD_ENUM;

struct IP_H{
  char ETX;
  unsigned short int FDST;
  unsigned short int FSRC;
  unsigned char IPP;
} __attribute__((packed));

struct NP_CARD{
  unsigned char TS;
  unsigned char CH;
  unsigned char ETX;
} __attribute__((packed));


struct AUTH_ETH_RESP{
	unsigned  char mac[8]; // Для кого предназначен ответ
	bool access;
	unsigned short int ipaddr;
	unsigned char param[16];
} __attribute__((packed));

struct AUTH_ETH_REQ{
	unsigned char mac[8];
	unsigned char NODE_TYPE;
	unsigned char NODE_VER;
	unsigned char sensor_types[7];
  	unsigned char sensor_channels[7];
}__attribute__((packed));

struct ETH_LAY{  
  union{
    unsigned char value;
    struct {
      unsigned char PID:4;
      unsigned char UNUSED:1;
      unsigned char ETH_VER:3;
    } bits;
  } ETH_T;

  char NETID;
  unsigned short int NDST;
  unsigned short int NSRC;
  unsigned char NSRC_TS;
  unsigned char NSRC_CH;
  unsigned char XOR;
}__attribute__((packed));
