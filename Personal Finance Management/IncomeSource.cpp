#include "IncomeSource.h"
#include <iomanip>

using namespace std;

Income::Income() {
	id = 0;
	name = "";
}

Income::Income( int _id, string _name) {
	id = _id;
	name = _name;
}

ostream& operator<<(ostream& os, const Income& inc) {
	os << left << setw(5) << inc.id
		<< "[Nguon Thu] " << inc.name;
	return os;
}