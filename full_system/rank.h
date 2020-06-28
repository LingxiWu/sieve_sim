#ifndef RANK_H
#define RANK_H

#include <vector>
#include "bank.h"

class Rank{
	public:

		// fields
		char rankID;
		std::vector<Bank*> bank_vec; 

		// methods
		Rank(char rank_id, int num_banks);

};

#endif
