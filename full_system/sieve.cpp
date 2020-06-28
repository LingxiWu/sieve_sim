#include <iostream>
#include "sieve.h"
using namespace std;

Sieve::Sieve()
{

}
void Sieve::update(long long int time_tick){

	// sync up the clock
	tick = time_tick;

	// try to dispatch pPacket by checking its top pPacket's dispatch time
	while(true){
		if(pcie_in_q.size()>0){ // make sure we have outstanding pPackets
			PCIePacket *pp = pcie_in_q.front();
			if(pp->next_dispatch <= tick){
				int removePPacket; 
				removePPacket = dispatch(pp);	// returns 1 if all reqs in a pPacket are dispatched
				if(removePPacket){
					pcie_in_q.pop();
				} else { // unable to distribute all reqs for current pPacket, will comback later
					break;
				}
			} else { // don't bother to check the rest of the queue, since the first one is not ready
				break;
			}
		} else { // if no outstanding pPackets
			break;
		}
	}

	// move reqs from bank buffer to ready_vec			
    for(int i=0;i<TOTAL_NUM_RANK;i++){
		Rank *r = rank_vector[i];
		for(int j=0;j<NUM_BANK;j++){
			Bank *b = r->bank_vec[j];
			if(b->request_buffer.size() < 1){continue;}
			else{
				if(b->request_buffer.front()->readyCycle <= tick && ready_vec.size() < READY_VEC_CAPACITY){
				    ready_vec.push_back(b->request_buffer.front());
//cout << "@ tick " << to_string(tick) << " released req " << " ";
//b->request_buffer.front()->printRequest();
					b->request_buffer.pop();
				}
			}
		}			
	}

	// move from ready_vec to out_q
	if(pcie_in_q.size()==0 && bankEmpty() && ready_vec.size()>0){ // scenario 1: everything else is done, just a few reqs in ready_vec need to be processed	
		if(pcie_out_q.size() < Q_FULL_CAPACITY){ // pcie_out_q is available
			long long int num_pp_return; // to record how many pPackets to return
			int quotient = ready_vec.size() / NUM_REQ_PER_PCKT;
			int remainder = ready_vec.size() % NUM_REQ_PER_PCKT;
			int num_pPackets;
			int out_q_available = Q_FULL_CAPACITY - pcie_out_q.size();
//cout << "here here quotient " << to_string(quotient) << " remainder " << to_string(remainder) << endl;
			if(remainder > 0){
				num_pPackets = min((quotient+1), out_q_available);
			} else {
				num_pPackets = min(quotient, out_q_available);
			}
			long long int n_r = tick += PCIE_OVERHEAD;
			for(int i=0; i<(num_pPackets-1); i++){
				PCIePacket *pPacket = new PCIePacket();
				pPacket->pciePacketID = num_pp_return;
				pPacket->next_return = n_r;
				n_r += PCIE_OVERHEAD;
				num_pp_return += 1;	
				for(int j=0;j<NUM_REQ_PER_PCKT;j++){
					pPacket->request_vector.push_back(ready_vec[j]);
					ready_vec.erase(ready_vec.begin());
				} 
				pcie_out_q.push(pPacket);
			} 
			if(remainder>0){
				PCIePacket *pPacket = new PCIePacket();
				pPacket->pciePacketID = num_pp_return;
				pPacket->next_return = n_r;
				n_r += PCIE_OVERHEAD;
				for(int k=0;k<remainder;k++){
					pPacket->request_vector.push_back(ready_vec[k]);
					ready_vec.erase(ready_vec.begin());
				} 
				pcie_out_q.push(pPacket);	
			}
		}
	} else if((pcie_in_q.size()>0 || !bankEmpty()) && 
			  (ready_vec.size() >= THRESHOLD && pcie_out_q.size() < Q_FULL_CAPACITY)){ // scenario 2: everything else is churning 

		long long int num_pp_return = 0;
	int out_q_available = Q_FULL_CAPACITY - pcie_out_q.size();
		int num_pPackets = min(THRESHOLD/NUM_REQ_PER_PCKT, out_q_available);
//cout << "@ tick: " << to_string(tick) << " out_q_available: " << to_string(out_q_available) << " num_pPackets: " << to_string(num_pPackets) << endl;
//cout << "pcie_in_q: " << to_string(pcie_in_q.size()) << " bankEmpty(): " << to_string(bankEmpty()) << " ready_vec.size(): " << to_string(ready_vec.size()) << " pcie_out_q.size(): " << to_string(pcie_out_q.size()) << endl; 
		long long int n_r = tick += PCIE_OVERHEAD;
        for(int i=0;i<num_pPackets;i++){
            PCIePacket *pPacket = new PCIePacket();
            pPacket->pciePacketID = num_pp_return;
            pPacket->next_return = n_r;
			n_r += PCIE_OVERHEAD;
            num_pp_return += 1;
            for(int j=0;j<NUM_REQ_PER_PCKT;j++){
                pPacket->request_vector.push_back(ready_vec[j]);
                ready_vec.erase(ready_vec.begin());
            }
            pcie_out_q.push(pPacket);
        }
	}
	
	// drain pPackets from out_q				
	if(pcie_out_q.size() > 0){
		while(!pcie_out_q.empty()){
			PCIePacket *pp = pcie_out_q.front();

			if(pp->next_return <= tick){
cout << "@ tick: " << to_string(tick) << " drained a pPacket " << endl;
for(int i=0;i<pp->request_vector.size();i++){
			how_many ++;
}			pcie_out_q.pop();					
			} else {
				break;
			}
		}
	}

	
}


// returns 1 if all reqs are dispatched. returns 0 if some reqs are not able to dispatch
char Sieve::dispatch(PCIePacket *pPacket){

	int result = 1; // 0 do not delete the pPacket. 1 delete the pPacket.
	
	// loop through all reqs in the pPacket, try to dispatch them to target banks
	for(int i=0;i<pPacket->request_vector.size();i++){
		Request *r = pPacket->request_vector[i];
//cout << "here pPacketID: " << to_string(pPacket->pciePacketID) << " requestID: " << to_string(r->requestID) << " dispatched? " << to_string(r->dispatched) << endl;
		if(r->dispatched == 0){ // found one request ready to go
			if(rank_vector[r->rankID]->bank_vec[r->bankID]->request_buffer.size() <= rank_vector[r->rankID]->bank_vec[r->bankID]->queue_depth){
				// figure out the ready cycle
				long long int last_ready;
				if(rank_vector[r->rankID]->bank_vec[r->bankID]->request_buffer.size()>0){
					last_ready = rank_vector[r->rankID]->bank_vec[r->bankID]->request_buffer.back()->readyCycle;
				} else {
					last_ready = tick;
				}	
				r->readyCycle = last_ready + r->duration;
//cout << "req: " << to_string(r->requestID) << " finishes at: " << to_string(r->readyCycle) << endl; 
				rank_vector[r->rankID]->bank_vec[r->bankID]->request_buffer.push(r);
				// mark the req as d ispatched.
//cout << "marking request " << to_string(pPacket->request_vector[i]->requestID) << " dispatched" << endl;
				pPacket->request_vector[i]->dispatched = 1;
			} else { // no space in the target buffer
				continue;
			}
		} else if (r->dispatched == 1) { // the req was already dispatched, fetch the next one
			continue;
		}
	}

	// determine if we need to remove pPacket
	for(int i=0;i<pPacket->request_vector.size();i++){	
		if(pPacket->request_vector[i]->dispatched == 0){
			result = 0;
		}
	}

	return result;
}

// set up chip parameters
void Sieve::chipConfig(int num_bank, int num_row, int num_col, int device_width){
	NUM_BANK = num_bank;
	NUM_ROW = num_row;
	NUM_COL = num_col;
	DEVICE_WIDTH = device_width;
}

// set up system parameters
void Sieve::sieveConfig(int total_megs_of_mem, int num_chan, int trans_size, int bus_width){
    TOTAL_MEGS_OF_MEM = total_megs_of_mem;
	NUM_CHAN = num_chan;
	TRANS_SIZE = trans_size;
	BUS_WIDTH = bus_width;

	STORAGE_PER_DEVICE = NUM_ROW * (NUM_COL * DEVICE_WIDTH) * NUM_BANK / 8 * (pow(2, -20));
	MEGS_OF_STORAGE_PER_RANK = (int)((STORAGE_PER_DEVICE * (BUS_WIDTH/DEVICE_WIDTH)));
	TOTAL_NUM_RANK = (int)(TOTAL_MEGS_OF_MEM / MEGS_OF_STORAGE_PER_RANK);
	RANK_PER_CHAN = (int)(TOTAL_NUM_RANK/NUM_CHAN);
}

int Sieve::setupRanks(){

	if(TOTAL_NUM_RANK < 1){
		cout << "Rank number too small. Please retry. " << endl;
		return 0;
	}
	for(int i=0;i<TOTAL_NUM_RANK;i++){
		Rank *rank = new Rank(i, NUM_BANK);
		rank_vector.push_back(rank);
	}
//	cout << "in sieve::setupRanks(): " << rank_vector.size() << endl; 
	return 1;
}

void Sieve::printSystem(){
	cout << " " << endl;
	cout << " --- chip configs --- " << endl;
	cout << "NUM_BANK: " << NUM_BANK << endl;
	cout << "NUM_ROW: " << NUM_ROW << endl;
	cout << "NUM_COL: " << NUM_COL << endl;
	cout << "DEVICE_WIDTH: " << DEVICE_WIDTH << endl;
	cout << " " << endl;
	cout << " --- sieve configs --- " << endl;
 	cout << "TOTAL_MEGS_OF_MEM (MB):" << TOTAL_MEGS_OF_MEM << endl;
	cout << "NUM_CHAN: " << NUM_CHAN << endl;
	cout << "TRANS_SIZE: " << TRANS_SIZE << endl;
	cout << "BUS_WIDTH: " << BUS_WIDTH << endl;
	cout << "STORAGE_PER_DEVICE (MB):" << STORAGE_PER_DEVICE << endl;
	cout << "MEGS_OF_STORAGE_PER_RANK (MB): " << MEGS_OF_STORAGE_PER_RANK << endl;
    cout << "TOTAL_NUM_RANK: " << TOTAL_NUM_RANK << endl;
	cout << "RANK_PER_CHAN: " << RANK_PER_CHAN << endl;
}

char Sieve::inAvailable(){
	// return number of empty slots in in_q
	if(Q_FULL_CAPACITY - pcie_in_q.size() < 1){
		return 0;
	} else {
		return Q_FULL_CAPACITY - pcie_in_q.size();
	}
}

// adding pPacket into in_q
char Sieve::acceptTransaction(queue<PCIePacket*> transaction){
	// just to make sure we have enuf slots
	if(transaction.size()>(Q_FULL_CAPACITY-pcie_in_q.size())){
		cout << "In Sieve->acceptTransaction(): NOT ENOUGH SLOTS. ABORT. " << endl;
	}
	
	// Set pPackets' next_dispatch time and add pPackets to sieve's IN_Q		
	int num_pPackets = transaction.size();
	long long int n_p = tick += PCIE_OVERHEAD;

	for(int i=0;i<num_pPackets;i++){		
		PCIePacket *pPacket = transaction.front();
		pPacket->next_dispatch = n_p;
cout << "In acceptTransaction() @ tick " << to_string(tick) << endl;
pPacket->printPPacket();	
		pcie_in_q.push(pPacket);
		n_p += PCIE_OVERHEAD;
		transaction.pop();
	}
	
	return 0;
}

// finished = 1, unfinished = 0
char Sieve::finished(){
	// check IN_Q and OUT_Q
	if(pcie_in_q.size() > 0 || pcie_out_q.size() > 0){
		return 0;
	}
    for(int i=0;i<TOTAL_NUM_RANK;i++){
		Rank *r = rank_vector[i];
		for(int j=0;j<NUM_BANK;j++){
			Bank *b = r->bank_vec[j];
			if(b->request_buffer.size() > 0){
				return 0;
			}			
		}
	}
	return 1;
}

char Sieve::bankEmpty(){
    for(int i=0;i<TOTAL_NUM_RANK;i++){
		Rank *r = rank_vector[i];
		for(int j=0;j<NUM_BANK;j++){
			Bank *b = r->bank_vec[j];
			if(b->request_buffer.size() > 0){
				return 0;
			}			
		}
	}
	return 1;
}
