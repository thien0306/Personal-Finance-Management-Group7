#pragma once

#include <string>
#include "Date.h"
#include "Transaction.h"
using namespace std;


class RecurringTransaction {
public:
	int id;
	TransactionType type;
	int categoryId;
	int walletId;
	double amount;
	Date startDate; 
	Date endDate; 
	string desc;
	Date lastProcessedDate;
	RecurringTransaction();
};
