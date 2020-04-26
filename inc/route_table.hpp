#pragma once

#include "exptimer.hpp"
#include <vector>
#include <map>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "debug.hpp"

using namespace std;
using namespace boost::posix_time;


/**
 * @brief Время жизни пути в секундах
 */
#define ROUTE_LIVE_TIME 300

struct RRec{
	unsigned short int nsrc;
	ptime live_time;
	unsigned char NSRC_TS;
	unsigned char NSRC_CH;
};

class RouteTable{
public:

void Clear(){
	ROUTE_TABLE.clear();
};
/**
 * @brief Анализ и удаление старых узлов 
 */
void analyseTable(){
	// Проверям таблицу раз в 60 секунд
	static ExpTimer exp(60);
	if (!exp.Expired())
		return;

	exp.Start();
	
	ptime now = second_clock::local_time();
    time_duration diff;
	time_duration LT = seconds(ROUTE_LIVE_TIME);

	// Элементы для удаления
	vector<map<unsigned int, struct RRec>::iterator> del_rec;

	// Перебираем таблицу в поисках просроченых записей
	for (auto it = ROUTE_TABLE.begin(); it!= ROUTE_TABLE.end(); it++){
		diff = now - it->second.live_time;
		if (diff > LT)
			del_rec.push_back(it);
	};

	// Удаляем записи
	for (auto it : del_rec){
		SPDLOG_TRACE("Route to {} deleted", it->second.nsrc);
		ROUTE_TABLE.erase(it);
	};

};

/**
 * @brief Создает или добавляет новый маршрут в таблицу
 *
 * @param nsrc адрес узла от которого принят пакет
 * @param fsrc адрес узла создавшего пакет
 * @param ts слот для связи с узлом от которого приняли пакет
 * @param ch канал для связи с узлом от которого приняли пакет
 */
void addRoute(unsigned short int nsrc, unsigned short int fsrc, 
			unsigned char ts, unsigned char ch){
	RRec rec;
	rec.nsrc = nsrc;
	rec.NSRC_TS = ts;
	rec.NSRC_CH = ch;
	rec.live_time = second_clock::local_time();

	ROUTE_TABLE.insert(pair<unsigned int, struct RRec>(fsrc, rec));
	SPDLOG_TRACE("Route {} updated", fsrc);
};

/**
 * @brief Получить информацию о промежуточном узел через который 
 * происходит свзяь с узлом fsrc
 *
 * @param fsrc конечный получатель ip пакета
 * @param nsrc указатель на промежуточный узел
 * @param ts временной канал связи с промежуточным узлом
 * @param ch частота связи с промежуточным узлом
 *
 * @return 
 */
bool getRoute(unsigned short int fsrc, unsigned short int *nsrc,
		unsigned char *ts, unsigned char *ch){
	// Ищем по ключу
	auto search = ROUTE_TABLE.find(fsrc);
	
	if (search == ROUTE_TABLE.end())
		return false;

	RRec *rec =  &search->second;
	*nsrc = rec->nsrc;
	*ts = rec->NSRC_TS;
	*ch = rec->NSRC_CH;
	return true;
};

private:
	map<unsigned int, struct RRec> ROUTE_TABLE;
};
