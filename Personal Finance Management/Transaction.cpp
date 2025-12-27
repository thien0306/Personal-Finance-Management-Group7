#include "Transaction.h"
#include <iomanip>

Transaction::Transaction() {
	type = INCOME;
	date = Date();
	CategoryId = 0;
	walletId = 0;
	amount = 0;
	description = "";
}

ostream& operator<<(ostream& os, const Transaction& t) {
    string typeString = (t.type == INCOME) ? "Thu" : "Chi";
    os << left << setw(5) << t.id
        << setw(12) << typeString
        << setw(10) << t.date.toString()
        << right << setw(15) << (long long)t.amount << "  " 
        << left << t.description;
    return os;
}