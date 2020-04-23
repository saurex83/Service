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
	static void eth_processRxPool();
	static bool eth_frame_filter(Frame& frame);
	static void eth_fill_metadata(Frame& frame);
	static void RP_Receive(Frame& frame);
	static void IP_Receive(Frame& frame);
	static void NP_Receive(Frame& frame);
	static void AUTH_ETH_Receive(Frame& frame);
	static void eth_send(Frame& frame);
};
