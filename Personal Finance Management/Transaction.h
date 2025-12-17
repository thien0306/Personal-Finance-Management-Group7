#pragma once

#include <string>
#include "Date.h"
using namespace std;
enum TransactionType { INCOME = 1, EXPENSE = 2 };

class Transaction {
public:
	int id;
	TransactionType type; 
	Date date; 
	int CategoryId;
	int walletId;
	double amount;
	string description;
	Transaction();
	friend ostream& operator<<(ostream& os, const Transaction& t);
};
