#include "pciePacket.h"
using namespace std;

void PCIePacket::printPPacket(){

	cout << "PCIePacketID: " << to_string(pciePacketID) << 
	" Number of requests: " << to_string(request_vector.size()) << 
	" Next_Dispatch: " << to_string(next_dispatch) << 
	" Next_Return: " << to_string(next_return) << endl;


}
