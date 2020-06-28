#ifndef REQUEST_H
#define REQUEST_H

class Request {
	public:
		int requestID;
		int rankID;
		int bankID; // which bank to send to
		int seqNum; 
		long long int startCycle; // FROM TRACE: when do this request start
	    long long int endCycle; // FROM TRACE: when do this request finish
		long long int readyCycle; // FROM SIEVE: when can we remove this req from bank's queue
		int duration;
		char dispatched = 0;		
		int pciePacketID; // which PCIe packet I belong to

		Request(int requestID, int rankID, int bankID, int seqNum, int startCycle, int endCycle, int duration, int pciePacketID);
		void printRequest();
};
#endif
