#include "rank.h"
#include <iostream>

using namespace std;


// constructor setup banks
Rank::Rank(char rank_id, int num_bank_per_rank){
	rankID = rank_id;
	// make a group of banks for this rank
	for(char i=0;i<num_bank_per_rank;i++){
		string bank_id = "";
		bank_id += to_string(rankID);
		bank_id += "-";
		bank_id += to_string(i);
		Bank *bank = new Bank(bank_id, 0);
//		bank->printBank();
		bank_vec.push_back(bank);	
	}/*
	for(vector<Bank*>::iterator it=bank_vec.begin();it!=bank_vec.end();++it){
		it.printBank(); 		
	}*/	
}

