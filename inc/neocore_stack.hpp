#include <vector>
#include <debug.hpp>
#include "frame.hpp"
#include "neocore_stack_def.hpp"

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
	static unsigned char calc_xor(struct ETH_LAY *eth);
	static void NP_TimeAlloc();
	static void NP_Send_gw_card();   
	static void NP_Periodic_Send_GWCard();
	static bool ip_frame_filter(Frame& frame); 
	static void IPP_Process(Frame& frame);
	static void TCP_Recive(Frame& frame);
	static void UDP_Recive(Frame& frame);
	static void AUTH_IP_Recive(Frame& frame);
	static void ROUTE_IPP_Receive(Frame& frame);

};
