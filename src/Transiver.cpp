#include "SerialCom.hpp" 
#include "ServiceConfig.hpp"
#include "Transiver.hpp"
#include "debug.hpp"
#include <iostream>

#define ATYPE_PAR_ERR  0
#define ATYPE_CMD_ERR  1
#define ATYPE_CMD_OK  2

#define PAR_CRC16  1
#define PAR_NOCMD  2


#define CMD_LEN 1
#define CMD_ARG_VAL 2
#define CMD_SEEDING 3
#define CMD_NOSEEDING 4
#define CMD_TX_FULL 5
#define CMD_RX_EMPTY 6 


static unsigned char rx_buffer[READ_BUFF_SIZE];	
static unsigned char tx_buffer[WRITE_BUFF_SIZE];	

static src::severity_logger<severity_level> lg;

static unsigned short crc16(unsigned char *pcBlock, unsigned int len){
    unsigned short crc = 0xFFFF;
    unsigned char i;
 
    while (len--) {
        crc ^= *pcBlock++ << 8;
        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

static void serial_write(unsigned char *data, unsigned int len){
	unsigned int buf_len = len + 3;
	unsigned short crc = crc16(data, len);

	tx_buffer[0] = len + 2;
	std::memcpy(&tx_buffer[1], data, len);
	tx_buffer[len+1] = ((unsigned char*)(&crc))[0];
	tx_buffer[len+2] = ((unsigned char*)(&crc))[1];
	SerialCom& serial = SerialCom::getInstance();
	serial.write(tx_buffer, buf_len);
};

static int serial_read(){
	SerialCom& serial = SerialCom::getInstance();
	unsigned int read_bytes = serial.read(rx_buffer, READ_BUFF_SIZE);
	// Ничего не прочитали
	if (!read_bytes)
		return 0;	
	
	// Указаный размер передачи не соотетсвует фактическому
	if (rx_buffer[0] != read_bytes - 1){
		LOGGER(lg, error) << "rx_buffer[0]=" << rx_buffer[0] << " .read_bytes=" << read_bytes;
		throw ParserError("Received size not match to specified");	
	};

	// Произошла ошибка парсера
	if (rx_buffer[1] == ATYPE_PAR_ERR)
		switch(rx_buffer[2]){
			case PAR_CRC16:
				LOGGER(lg, error) << "crc16";
				return -1;
			case PAR_NOCMD:
				LOGGER(lg, error) << "No CMD";
				throw ParserError("Command not found");
			default:
				LOGGER(lg, error) << "Unexpected error";
				throw ParserError("Unexpected parser error");
		}

	// Команда выдала ошибку при исполнении
	if (rx_buffer[1] == ATYPE_CMD_ERR)
		switch(rx_buffer[2]){
			case CMD_LEN:
				LOGGER(lg, error) << "Incorrect args length"; 
				throw CMDError("Incorrect args length");
			case CMD_ARG_VAL:
				LOGGER(lg, error) << "Incorrect args value"; 
				throw CMDError("Incorrect args value");
			case CMD_SEEDING:
				LOGGER(lg, error) << "Network seed must be stoped"; 
				throw CMDError("Network seed must be stopped");
			case CMD_NOSEEDING:
				LOGGER(lg, error) << "Network seed must be start"; 
				throw CMDError("Network seed must be start");
			case CMD_TX_FULL:
				LOGGER(lg, error) << "TX buffer full"; 
				throw CMDError("TX buffer full");
			case CMD_RX_EMPTY:
				BOOST_LOG_SEV(lg, error) << "RX buffer empty"; 
				throw CMDError("RX buffer empty");
			default:
				LOGGER(lg, error) << "Unexpected cmd error answer"; 
				throw CMDError("Unexpected cmd error answer");
		}

	// Ожидаемы ответ ATYPE_CMD_OK
	if (rx_buffer[1] != ATYPE_CMD_OK){
		LOGGER(lg, error) << "Expected ATYPE_CMD_OK, but something go wrong. rx_buffer[1] = " 
			<< unsigned(rx_buffer[1]);
		throw ParserError("Expected ATYPE_CMD_OK");
	};
	return read_bytes;  	
};


static int send_cmd(unsigned char *cmd, int size){
	bool cmd_notsended = true;
	int read_bytes;
	while (cmd_notsended){
		serial_write(cmd, size);	
		read_bytes = serial_read();
		if (read_bytes == -1){
			LOGGER(lg, error) << "CRC16 error"; 
			continue;	
		}
		cmd_notsended = false;
	}
	return read_bytes;
}

bool Transiver::is_network_seed(){
	unsigned char cmd = 0x00;
		
	int read_bytes = send_cmd(&cmd, sizeof(unsigned char));
	
	if (read_bytes != 5){
		LOGGER(lg, error) << "cmd 0x00 incorrect answer length = " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x00 is not 5 bytes");
	;}

	LOGGER(lg, trace) << "CMD 0x00. Network " << ((rx_buffer[2]? "seeding":"not seeding")); 
	
	if (rx_buffer[2])
		return true;
	return false;
};

void Transiver::set_panid(unsigned char panid){
	unsigned char cmd[2];
	cmd[0] = 0x01;
	cmd[1] = panid;

	int read_bytes = send_cmd(cmd, sizeof(cmd));

	
	if (read_bytes != 4){
		LOGGER(lg, error) << "cmd 0x01 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x01 is not 4 bytes");
	;}
	
	LOGGER(lg, trace) << "CMD 0x01. Network set panid " << panid; 
};

void Transiver::set_rtc(uint32_t rtc){
	unsigned char cmd[5];
	cmd[0] = 0x02;
	cmd[1] = ((unsigned char*)&rtc)[0];
	cmd[2] = ((unsigned char*)&rtc)[1];
	cmd[3] = ((unsigned char*)&rtc)[2];
	cmd[4] = ((unsigned char*)&rtc)[3];

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		LOGGER(lg, error) << "cmd 0x02 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x02 is not 4 bytes");
	;}

	LOGGER(lg, trace) << "CMD 0x02. Set RTC =  " << rtc; 
};

void Transiver::network_seed(bool status){
	unsigned char cmd[2];
	cmd[0] = 0x03;
	cmd[1] = status;

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		LOGGER(lg, error) << "cmd 0x03 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x03 is not 4 bytes");
	;}
	
	LOGGER(lg, trace) << "CMD 0x03. Set network seeding =  " << (status? "true":"false"); 
};

void Transiver::load_streem_iv(unsigned char *iv){
	unsigned char cmd[17];
	cmd[0] = 0x04;
	for (int i = 0; i < 16; i++)
		cmd[i + 1] = iv[i];

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		LOGGER(lg, error) << "cmd 0x04 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x04 is not 4 bytes");
	;}
	
	LOGGER(lg, trace) << "CMD 0x04. New IV loaded"; 
};

void Transiver::load_streem_key(unsigned char *key){
	unsigned char cmd[17];
	cmd[0] = 0x05;
	for (int i = 0; i < 16; i++)
		cmd[i + 1] = key[i];

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		LOGGER(lg, error) << "cmd 0x05 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x05 is not 4 bytes");
	};
	
	LOGGER(lg, trace) << "CMD 0x05. New KEY loaded"; 
};

void Transiver::open_slot(unsigned char ts, unsigned char ch){
	unsigned char cmd[3];
	cmd[0] = 0x07;
	cmd[1] = ts;
	cmd[2] = ch;

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		LOGGER(lg, error) << "cmd 0x07 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x07 is not 4 bytes");
	};

	LOGGER(lg, trace) << "CMD 0x07. Opened slot: " << unsigned(ts) << " on ch: " << unsigned(ch) ; 
};

void Transiver::close_slot(unsigned char ts){
	unsigned char cmd[2];
	cmd[0] = 0x08;
	cmd[1] = ts;

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		LOGGER(lg, error) << "cmd 0x08 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x08 is not 4 bytes");
	};

	LOGGER(lg, trace) << "CMD 0x08. Closed slot: " << unsigned(ts) ; 
};


int Transiver::rx_frames(){
	unsigned char cmd;
	cmd = 0x09;

	int read_bytes = send_cmd(&cmd, sizeof(unsigned char));
	
	if (read_bytes != 5){
		LOGGER(lg, error) << "cmd 0x09 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x09 is not 4 bytes");
	};
	
	int rx_frames = rx_buffer[2];
	LOGGER(lg, trace) << "CMD 0x09. Frames in RX buffer: " << rx_frames ;

	return rx_frames;
};


int Transiver::tx_frames(){
	unsigned char cmd;
	cmd = 0x0A;

	int read_bytes = send_cmd(&cmd, sizeof(unsigned char));
	
	if (read_bytes != 5){
		LOGGER(lg, error) << "cmd 0x0A incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x0A is not 4 bytes");
	};
	
	int tx_frames = rx_buffer[2];
	LOGGER(lg, trace) << "CMD 0x0A. Frames in TX buffer: " << tx_frames ;

	return tx_frames;
};


void Transiver::push_tx(Frame *frame){
	unsigned char cmd[256];
	unsigned char cmd_size = sizeof(meta) + 1 + frame->len + 1;

	cmd[0] = 0x0B;
	memcpy(&cmd[1], &frame->meta, sizeof(meta));
	cmd[sizeof(meta) + 1] = frame->len;
	memcpy(&cmd[sizeof(meta) + 2], frame->payload, frame->len);
	int read_bytes = send_cmd(cmd, cmd_size);
	
	if (read_bytes != 4){
		LOGGER(lg, error) << "cmd 0x0B incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x0B is not 4 bytes");
	};
	
	LOGGER(lg, trace) << "CMD 0x0B. Frame pushed in TX buffer";
};

unsigned int Transiver::pop_rx(unsigned char *frame){
	unsigned char cmd;
	cmd = 0x0C;

	int read_bytes = send_cmd(&cmd, sizeof(unsigned char));
	
	if (read_bytes < 4){
		LOGGER(lg, error) << "cmd 0x0C incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x0C less 4 bytes");
	};

	for (int i = 0; i < read_bytes; i++)
		frame[i] = rx_buffer[i+2];	
	
	LOGGER(lg, trace) << "cmd 0x0B. Frames pop from RX buffer (bytes): " << read_bytes;

	return read_bytes;
};
