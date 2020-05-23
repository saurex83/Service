#include "udp_statistic.hpp"
#include "neocore_stack.hpp"
#include "database.hpp"
#include "debug.hpp"

/**
 * @brief Структура данных статистики
 */
struct statistic {
  float vbat;
  float rx_time;
  float tx_time;
  unsigned short int crc_error;
  unsigned short int cca_error;
  unsigned int uptime;
  unsigned short int reconect_cnt;
} __attribute((packed));

namespace udp_statistic{

/**
 * @brief Разбор udp данных с статистикой и сохранение в базе данных 
 *
 * @param frame
 */
void parse_statistic(Frame& frame){
	// Проверим размер
	if (frame.size() != sizeof(struct statistic)){
		SPDLOG_TRACE("Statistic data len {} != {}", 
				frame.size(), sizeof(struct statistic));
		return;
	};

//	struct AUTH_ETH_REQ& req =*(AUTH_ETH_REQ*)&frame.payload.front();		
	// Распаковываем данные
	struct statistic& st = *(struct statistic*)&frame.payload.front();
	// Отправитель статистики
	auto FSRC = frame.meta.meta.FSRC;
	SPDLOG_TRACE(
		"Statistic: FSRC={}, VBAT={}, RXT={}, TXT={}, CRC={}, CCA={}, UT={}, RCN={}",
			FSRC, st.vbat, st.rx_time, st.tx_time, st.crc_error, st.cca_error,
			st.uptime, st.reconect_cnt);

};
};
