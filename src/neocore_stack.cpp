#include "neocore_stack.hpp"

namespace neocore{

static vector<Frame> *tx_pool;
static vector<Frame> *rx_pool;

/**
 * @brief Инициализация модуля
 *
 * @param tx указатель на вектор пакетов для передачи
 * @param rx указатель на вектор принятых пакетов
 */
void init(vector<Frame> *tx, vector<Frame> *rx){
	tx_pool = tx;
	rx_pool = rx;
};


/**
 * @brief Обрабатывает принятые пакеты согласно правилам протокола
 */
void process(){
};

};
