#include <vector>
#include <debug.hpp>
#include "frame.hpp"

using namespace std;

/**
 * @brief Стек протокола
 */
namespace neocore{
	void init(vector<Frame> *tx, vector<Frame> *rx);
	void process();
};
