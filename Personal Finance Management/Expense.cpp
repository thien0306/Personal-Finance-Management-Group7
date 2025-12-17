#include "Expense.h"
#include <iomanip>

Expense::Expense() {
	id = 0;
	name = "";
}

Expense::Expense(int _id, string _name) {
	id = _id;
	name = _name;
}

ostream& operator<<(ostream& os, const Expense& exp) {
	os << left << setw(5) << exp.id
		<< "[Muc Chi]   " << exp.name;
	return os;
}