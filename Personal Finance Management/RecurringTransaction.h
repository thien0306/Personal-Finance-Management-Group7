#pragma once

#include <string>
#include "Date.h"
#include "Transaction.h"
using namespace std;


class RecurringTransaction {
public:
	TransactionType type;
	int CategoryId;
	int walletId;
	double amount;
	Date startDate; 
	Date endDate; 
	string description;

	Date nextDueDate;
	bool isFinished;

	RecurringTransaction();
	RecurringTransaction(TransactionType _type, int cateId, int WId, double _amount,
		Date start, Date end, string desc);

	friend ostream& operator<<(ostream& os, const RecurringTransaction& rt);
};
