#include <iostream>
#include "sieve.h"
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv){

	if (argc != 2){
		cout << "Missing trace filename" << endl;
		return 0;
	}

	// some variables
	Sieve *sieve = new Sieve();
	std::queue<Request*> requests_pool;
	
    long long int current_time_tick = 0;
	long long int num_pcie_packet = 0;

	// chip configs
	int NUM_BANK = 8;
	int NUM_ROW = 32768;
	int NUM_COL = 1024;
	int DEVICE_WIDTH = 16;
	// sieve configs
	int TOTAL_MEGS_OF_MEM = 32768;
	int NUM_CHAN = 1;
    int TRANS_SIZE = 64;
	int BUS_WIDTH = 64;

	// set up chip parameters
	sieve->chipConfig(NUM_BANK, NUM_ROW, NUM_COL, DEVICE_WIDTH);
	// set up sieve parameters
	sieve->sieveConfig(TOTAL_MEGS_OF_MEM, NUM_CHAN, TRANS_SIZE, BUS_WIDTH);

	sieve->printSystem();
	sieve->setupRanks();

	// read the trace file
	ifstream traceFile;
	traceFile.open(argv[1],ios::in);	
	if(traceFile.is_open()){
		// File format
	    // Request_ID, Rank, Bank, Arrive, Leave, Duration
		string line;
		string delimiter = ",";
		size_t pos = 0; // this to hold the index of the first occurrence of delimiter in the string.
		int counter = 0;
		int temp, requestID, rank, bank, arrive_cycle, leave_cycle, duration;
		while (getline(traceFile, line)){
			counter = 0;
			// parse line: RequestID, Rank, Bank, Arrive_cycle, Leave_cycle, Duration
			while((pos = line.find(delimiter)) != string::npos){
				string token = line.substr(0, pos);
				temp = stoi(token);
				if(counter == 0){
					requestID = temp;
				} else if (counter == 1){
					rank = temp;
				} else if (counter == 2){
					bank = temp;
				} else if (counter == 3){
					arrive_cycle = temp;
				} else if (counter == 4){
					leave_cycle = temp;
				} else if (counter == 5){
					duration = temp;
				}	
				counter += 1;
				line.erase(0, pos + delimiter.length());
				
			}
			// make an k-mer request
			Request *request = new Request(requestID, rank, bank, 0, arrive_cycle, leave_cycle, duration, 0);
//request->printRequest();
			requests_pool.push(request);
		}
	} else {
		cout << "Failed to open file " << argv[1] << endl; 
		return 0;
	}
	traceFile.close();

	/*** Simulation Start ***/
	while(true){
		// process at current time tick
		sieve->update(current_time_tick);
		
		// try to push k-mer requests into Sieve's Q
		if(sieve->inAvailable()>0){

   			queue<PCIePacket*> pcie_transaction;
			// making pcie packet(s).
//cout << "at time " << to_string(current_time_tick) << " available " << to_string(sieve->inAvailable()) << endl;
			for(int i=0;i<sieve->inAvailable();i++){

        		if(requests_pool.size() < 1){ // not enough requests, abort
//cout << "already depleted all reqs" << endl;
					break;
				}
				
 				int remaining_requests = requests_pool.size();
				int cnt = 0;
				if(remaining_requests < sieve->NUM_REQ_PER_PCKT){
					cnt = remaining_requests;
				} else {
					cnt = sieve->NUM_REQ_PER_PCKT;
				}
			
				PCIePacket *pciePacket = new PCIePacket();
				pciePacket->pciePacketID = num_pcie_packet;
				num_pcie_packet += 1;
	
				// lump a bunch of requests to one pcie packet, and deplete requests from the pool
				for(int i=0;i<cnt;i++){
					Request *r = requests_pool.front();	
					r->pciePacketID = pciePacket->pciePacketID;
					pciePacket->request_vector.push_back(r);
				//	r->printRequest();
					requests_pool.pop();
				}

				// add PCIE packets to transaction
				pcie_transaction.push(pciePacket);
				
			}
			
			// submit transactions
			if(pcie_transaction.size()>0){
				sieve->acceptTransaction(pcie_transaction);
			}
			
		}

		// terminate simulation ...		
		if(requests_pool.size() ==0 && sieve->finished()) {
			cout << "simulation finished ... " << endl;
			cout << "Total Cycles: " << to_string(current_time_tick) << endl;
			cout << "how many: " << to_string(sieve->how_many) << endl;
			break;
		}
		
		//if(current_time_tick == 7000){break;}
		
		// clock ticks
		current_time_tick += 1;
	}

	return 0;
}

