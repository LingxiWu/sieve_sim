#include "bank.h"
#include <iostream>
#include <string>
using namespace std;

Bank::Bank(string bank_id, int next_act_cycle):
	bankID(bank_id){
		
}
void Bank::printBank(){
	cout << " " << endl;
	cout << "Bank " << bankID 
	<< " receives " << to_string(request_buffer.size()) << " requests. " << endl;
//	<< to_string(ready_queue.size()) << " request ready." << endl;
}
