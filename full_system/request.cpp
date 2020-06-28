#include <iostream>
#include "request.h"
using namespace std;

Request::Request(int request_id, int rank_id, int bank_id, int seq_num, int start_cycle, int end_cycle, int duration, int pcie_packet_id):
requestID(request_id),
rankID(rank_id),
bankID(bank_id),
seqNum(seq_num),
startCycle(start_cycle),
endCycle(end_cycle),
duration(duration),
pciePacketID(pcie_packet_id)
{}

void Request::printRequest(){
	cout << "RequestID: " << to_string(requestID) <<
	" rankID: " << to_string(rankID) <<
	" bankID: " << to_string(bankID) <<
	" seqNum: " << to_string(seqNum) <<
//	" startCycle: " << to_string(startCycle) <<
//	" endCycle: " << to_string(endCycle) <<
	" duration: " << to_string(duration) <<
	" dispatched: " << to_string(dispatched) <<
	" pciePacketID: " << to_string(pciePacketID) << endl;
}
