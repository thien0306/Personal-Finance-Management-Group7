#include "RecurringTransaction.h"

#include <iomanip>

RecurringTransaction::RecurringTransaction() {
    type = INCOME; 
    CategoryId = 0;
    walletId = 0;
    amount = 0;
    isFinished = false;
}

RecurringTransaction::RecurringTransaction(TransactionType _type, int cateId, int WId, double _amount,
    Date start, Date end, string desc) {
    type = _type;
    CategoryId = cateId;
    walletId = WId;
    amount = _amount;
    startDate = start;
    endDate = end;
    description = desc;
    nextDueDate = start;
    isFinished = false;
}

ostream& operator<<(ostream& os, const RecurringTransaction& rt) {
    string typeStr = (rt.type == INCOME) ? "Thu" : "Chi";
    string status = (rt.isFinished) ? "Ket thuc" : "Dang chay";
    os << left << setw(12) << status
        << setw(10) << typeStr
        << setw(12) << rt.nextDueDate.toString()
        << setw(12) << rt.endDate.toString()
        << right << setw(15) << (long long)rt.amount << "  "
        << left << rt.description;
    return os;
}