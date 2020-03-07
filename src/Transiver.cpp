#include "SerialCom.hpp" 
#include "ServiceConfig.hpp"
#include "Transiver.hpp"
#include <boost/log/trivial.hpp>

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



bool Transiver::serial_inited  = false;
SerialCom Transiver::serial;
unsigned char Transiver::rx_buffer[READ_BUFF_SIZE];	
unsigned char Transiver::tx_buffer[WRITE_BUFF_SIZE];	

Transiver::Transiver(){
	if (!serial_inited)
		serial.connect();
};

Transiver::~Transiver(){
	///serial.disconnect();
};

void Transiver::connect(){
	if (!serial_inited){
		serial.connect();
		serial_inited = true;
	};
};
 


unsigned short Transiver::crc16(unsigned char *pcBlock, unsigned int len){
    unsigned short crc = 0xFFFF;
    unsigned char i;
 
    while (len--) {
        crc ^= *pcBlock++ << 8;
        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

void Transiver::serial_write(unsigned char *data, unsigned int len){
	unsigned int buf_len = len + 3;
	unsigned short crc = crc16(data, len);

	Transiver::tx_buffer[0] = len + 2;
	std::memcpy(&Transiver::tx_buffer[1], data, len);
	Transiver::tx_buffer[len+1] = ((unsigned char*)(&crc))[0];
	Transiver::tx_buffer[len+2] = ((unsigned char*)(&crc))[1];
	serial.write(Transiver::tx_buffer, buf_len);
};

int Transiver::serial_read(){
	unsigned int read_bytes = serial.read(Transiver::rx_buffer, READ_BUFF_SIZE);
	// Ничего не прочитали
	if (!read_bytes)
		return 0;	
	
	// Указаный размер передачи не соотетсвует фактическому
	if (Transiver::rx_buffer[0] != read_bytes - 1){
		BOOST_LOG_TRIVIAL(error) <<"rx_buffer[0]=" << Transiver::rx_buffer[0] << " .read_bytes=" << read_bytes;
		throw ParserError("Received size not match to specified");	
	};

	// Произошла ошибка парсера
	if (Transiver::rx_buffer[1] == ATYPE_PAR_ERR)
		switch(Transiver::rx_buffer[2]){
			case PAR_CRC16:
				BOOST_LOG_TRIVIAL(error) << "crc16";
				return -1;
			case PAR_NOCMD:
				BOOST_LOG_TRIVIAL(error) << "No CMD";
				throw ParserError("Command not found");
			default:
				BOOST_LOG_TRIVIAL(error) << "Unexpected error";
				throw ParserError("Unexpected parser error");
		}

	// Команда выдала ошибку при исполнении
	if (Transiver::rx_buffer[1] == ATYPE_CMD_ERR)
		switch(Transiver::rx_buffer[2]){
			case CMD_LEN:
				BOOST_LOG_TRIVIAL(error) << "Incorrect args length"; 
				throw CMDError("Incorrect args length");
			case CMD_ARG_VAL:
				BOOST_LOG_TRIVIAL(error) << "Incorrect args value"; 
				throw CMDError("Incorrect args value");
			case CMD_SEEDING:
				BOOST_LOG_TRIVIAL(error) << "Network seed must be stoped"; 
				throw CMDError("Network seed must be stopped");
			case CMD_NOSEEDING:
				BOOST_LOG_TRIVIAL(error) << "Network seed must be start"; 
				throw CMDError("Network seed must be start");
			case CMD_TX_FULL:
				BOOST_LOG_TRIVIAL(error) << "TX buffer full"; 
				throw CMDError("TX buffer full");
			case CMD_RX_EMPTY:
				BOOST_LOG_TRIVIAL(error) << "RX buffer empty"; 
				throw CMDError("RX buffer empty");
			default:
				BOOST_LOG_TRIVIAL(error) << "Unexpected cmd error answer"; 
				throw CMDError("Unexpected cmd error answer");
		}

	// Ожидаемы ответ ATYPE_CMD_OK
	if (Transiver::rx_buffer[1] != ATYPE_CMD_OK){
		BOOST_LOG_TRIVIAL(error) << "Expected ATYPE_CMD_OK, but something go wrong. rx_buffer[1] = " 
			<< unsigned(Transiver::rx_buffer[1]);
		throw ParserError("Expected ATYPE_CMD_OK");
	};
	return read_bytes;  	
};


int Transiver::send_cmd(unsigned char *cmd, int size){
	bool cmd_notsended = true;
	int read_bytes;
	while (cmd_notsended){
		serial_write(cmd, size);	
		read_bytes = serial_read();
		if (read_bytes == -1){
			BOOST_LOG_TRIVIAL(warning) << "CRC16 error"; 
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
		BOOST_LOG_TRIVIAL(error) << "cmd 0x00 incorrect answer length = " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x00 is not 5 bytes");
	;}

	if (Transiver::rx_buffer[2])
		return true;
	return false;
};

void Transiver::set_panid(unsigned char panid){
	unsigned char cmd[2];
	cmd[0] = 0x01;
	cmd[1] = panid;

	int read_bytes = send_cmd(cmd, sizeof(cmd));

	
	if (read_bytes != 4){
		BOOST_LOG_TRIVIAL(error) << "cmd 0x01 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x01 is not 4 bytes");
	;}
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
		BOOST_LOG_TRIVIAL(error) << "cmd 0x02 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x02 is not 4 bytes");
	;}
};

void Transiver::network_seed(bool status){
	unsigned char cmd[2];
	cmd[0] = 0x03;
	cmd[1] = status;

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		BOOST_LOG_TRIVIAL(error) << "cmd 0x03 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x03 is not 4 bytes");
	;}
	
	BOOST_LOG_TRIVIAL(info) << "cmd 0x03. Set network status =  " << (status? "run":"stop"); 
};

void Transiver::load_streem_iv(unsigned char *iv){
	unsigned char cmd[17];
	cmd[0] = 0x04;
	for (int i = 0; i < 16; i++)
		cmd[i + 1] = iv[i];

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		BOOST_LOG_TRIVIAL(error) << "cmd 0x04 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x04 is not 4 bytes");
	;}
	
	BOOST_LOG_TRIVIAL(info) << "cmd 0x04. New IV loaded."; 
};

void Transiver::load_streem_key(unsigned char *key){
	unsigned char cmd[17];
	cmd[0] = 0x05;
	for (int i = 0; i < 16; i++)
		cmd[i + 1] = key[i];

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		BOOST_LOG_TRIVIAL(error) << "cmd 0x05 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x05 is not 4 bytes");
	};
	
	BOOST_LOG_TRIVIAL(info) << "cmd 0x05. New KEY loaded."; 
};

void Transiver::open_slot(unsigned char ts, unsigned char ch){
	unsigned char cmd[3];
	cmd[0] = 0x07;
	cmd[1] = ts;
	cmd[2] = ch;

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		BOOST_LOG_TRIVIAL(error) << "cmd 0x07 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x07 is not 4 bytes");
	};

	BOOST_LOG_TRIVIAL(info) << "cmd 0x07. Opened slot: " << unsigned(ts) << " on ch: " << unsigned(ch) ; 
};

void Transiver::close_slot(unsigned char ts){
	unsigned char cmd[2];
	cmd[0] = 0x08;
	cmd[1] = ts;

	int read_bytes = send_cmd(cmd, sizeof(cmd));
	
	if (read_bytes != 4){
		BOOST_LOG_TRIVIAL(error) << "cmd 0x08 incorrect answer length " << read_bytes; 
		throw std::runtime_error("Answer length for cmd 0x08 is not 4 bytes");
	};

	BOOST_LOG_TRIVIAL(info) << "cmd 0x08. Closed slot: " << unsigned(ts) ; 
};
