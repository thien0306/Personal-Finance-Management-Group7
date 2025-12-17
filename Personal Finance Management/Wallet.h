#pragma once
#include <string>

using namespace std;

class Wallet {
public:
	int id;
	string name;
	double balance;
	Wallet();
	Wallet(int id, string name, double balance);
	friend ostream& operator<<(ostream& os, const Wallet& w);
};


