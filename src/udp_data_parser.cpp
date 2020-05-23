#include "udp_data_parser.hpp"
#include "neocore_stack.hpp"
#include "database.hpp"
#include "debug.hpp"

#define MAX_MEASURE_DATA_BUF 60 

typedef enum DTYPE {DT_INT = 0, DT_FLOAT = 1, DT_BOOL = 2} DTYPE;

typedef enum MTYPE {MT_INSTANT = 0, MT_MIDDLE = 1, MT_MAX = 2, MT_MIN = 3
} MTYPE;

typedef enum PARAM {
  TEMP = 0, HUM = 1, PREASURE = 2, DIFF_PREASURE = 3, VOLTAGE = 4, CURRENT = 5,
  DINPUT = 6, BUS_INP = 7
} PARAM;

struct data_code {
  union {
    struct{
      char dtype:3;
      char chn:5;
    };
    char val;
  } SENSOR_PARAM;
  
  union {
    struct{
      char mtype:3;
      char parameter:5;
    };
  char val;
  } SENSOR_TYPE;
} __attribute((packed));

struct d_aggr{
  struct data_code dc;
  char dta[4];
} __attribute((packed));

namespace udp_data_parser{

/**
 * @brief Разбор udp данных измерения 
 *
 * @param frame
 */
void parse_data(Frame& frame){
	// Проверим размер. Он не может быть меньше заголовка data_code
	if (frame.size() <= sizeof(struct data_code)){
		SPDLOG_TRACE("Data len {} <= {}", 
				frame.size(), sizeof(struct data_code));
		return;
	};
	
	// Узнаем кто был отправителем данных
	auto FSRC = frame.meta.meta.FSRC;

	struct d_aggr agr;
	char len;
	// Извлекаю данные 
	while (frame.size() >= sizeof(struct data_code)){
		// Первые два байта информация о данном
		agr.dc.SENSOR_PARAM.val = frame.payload[0];
		agr.dc.SENSOR_TYPE.val = frame.payload[1];
		
		// Определим размер данного исходя из типа
		len = 0;
		switch(agr.dc.SENSOR_PARAM.dtype){
			case (DT_INT): len = 2;   break;
			case (DT_FLOAT): len = 4; break;
			case (DT_BOOL): len = 1;  break;
 		default:
        	break;
		};

		if (!len){
			SPDLOG_ERROR("Incorrect dtype = {}", agr.dc.SENSOR_PARAM.dtype);
			break;
		};

		// Убедимся что в кадре достаточно данных
		if (frame.size() < len + sizeof(struct data_code)){
			SPDLOG_ERROR("Dtype={}, Dtype.len={}. Frame.size()={} < {}",
					agr.dc.SENSOR_PARAM.dtype, len, frame.size(), 
					frame.size() + len);
			return;
		};

		// Уберем байты из заголовка
		frame.delHeader(sizeof(struct data_code));
		
		// Копируем данное
		for (size_t i = 0 ; i < len ; i++)
			agr.dta[i] = frame.payload[i];
		
		// Удалим сами данные, теперь мы знаем их размер
		frame.delHeader(len);

		// Вывод на экран
		switch(agr.dc.SENSOR_PARAM.dtype){
			case (DT_INT): 
				SPDLOG_TRACE("node={}. int, ch={}, val={}",
						FSRC, unsigned(agr.dc.SENSOR_PARAM.chn), 
						*(unsigned short int*)agr.dta );
			 	break;
			case (DT_FLOAT):
				SPDLOG_TRACE("node={}. FLOAT, ch={}, val={}",
						FSRC, unsigned(agr.dc.SENSOR_PARAM.chn), 
						*(float*)agr.dta );
				break;
			case (DT_BOOL):
				SPDLOG_TRACE("node={}. BOOL, ch={}, val={}",
						FSRC, unsigned(agr.dc.SENSOR_PARAM.chn), 
						*(bool*)agr.dta);
				break;
 		default:
        	break;
		};
	};



};
};

