#ifndef PCIEPACKET_H
#define PCIEPACKET_H

#include <vector>
#include "request.h"
#include <iostream>
using namespace std;

class PCIePacket {

	public:
		long long int pciePacketID = 0;
		vector<Request*> request_vector;
		long long int next_dispatch; // the earliest time that we can dispatch it to Sieve.
		long long int next_return; // the earliest time that we can return this pPacket to CPU
		void printPPacket();
		
};

#endif
