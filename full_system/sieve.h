#ifndef SIEVE_H
#define SIEVE_H

#include <queue>
#include <vector>
#include <math.h>
#include "pciePacket.h"
#include "rank.h"
#include <algorithm>

class Sieve{
	public:

long long int how_many = 0;		
		// fields		
		int THRESHOLD = 1020; // we make pPacket(s) if we pass this threshold, stricly multiple of NUM_REQ_PER_PCKT 
		int PCIE_OVERHEAD = 463;
		int NUM_REQ_PER_PCKT = 340;
		int READY_VEC_CAPACITY = 1020; // temporary. holds 3 pPackets worth of requests
		int Q_FULL_CAPACITY = 12; // 12	(# ranks * # banks * queue_depth) / (pcie_payload_size / k-mer_request_size). for 32 GB Sieve, payload 4KB and 12B req, capacity is 12	
		std::vector<Rank*> rank_vector;
		std::queue<PCIePacket*> pcie_in_q;
		std::queue<PCIePacket*> pcie_out_q;
	
		long long int tick = 0;
		// ready_queue stores returned reqs.
		std::vector<Request*> ready_vec;  // it's worth exploring the impact of queue depth on performance
		long long int num_return = 0;

		// chip parameters
		long long int NUM_BANK;
		long long int NUM_ROW;
		long long int NUM_COL;
		long long int DEVICE_WIDTH;

		// system parameters
		long long int TOTAL_MEGS_OF_MEM;
		int NUM_CHAN;
		int TRANS_SIZE;
		int BUS_WIDTH;
		long long int STORAGE_PER_DEVICE; // calculate
		long long int MEGS_OF_STORAGE_PER_RANK; // calculate
		int TOTAL_NUM_RANK; // calculate
		int RANK_PER_CHAN; // calculate
		int NUM_SA;	// optional for type-3 

		// methods
		Sieve();
		void chipConfig(int NUM_BANK, int NUM_ROW, int NUM_COL, int DEVICE_WIDTH);
		void sieveConfig(int TOTAL_MEGS_OF_MEM, int NUM_CHAN, int TRANS_SIZE, int BUS_WIDTH);
		int setupRanks();
		void printSystem();	
		char inAvailable();
		char outAvailable();	
		char finished();
		char acceptTransaction(std::queue<PCIePacket*> transaction);
		char dispatchPPacket();
		void update(long long int time_tick);
		char dispatch(PCIePacket *pPacket);	
		char bankEmpty();
};

#endif
