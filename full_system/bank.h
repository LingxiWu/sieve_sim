#ifndef BANK_H
#define BANK_H

#include <queue>
#include "request.h"
#include <string>
using namespace std;
class Bank{
	public:

		// fields
		string bankID; // rank#-bank#
		char queue_depth = 32; // depth default to 32
		std::queue<Request*> request_buffer; // to hold k-mer requests

		// methods
		Bank(string bank_id, int next_activate_cycle);
		void printBank();

};

#endif
