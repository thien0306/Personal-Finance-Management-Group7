#include "Wallet.h"
#include <iomanip>

using namespace std;

Wallet::Wallet() {
	id = 0;
	name = "";
	balance = 0;
}

Wallet::Wallet(int _id, string _name, double _balance) {
	id = _id;
	name = _name;
	balance = _balance;
}

ostream& operator<<(ostream& os, const Wallet& w) {
	os << left << setw(5) << w.id
		<< setw(20) << w.name
		<< fixed << setprecision(0) << w.balance << " VND";
	return os;
}