#pragma once

#include <string>
using namespace std;

class Expense {
public:
	int id;
	string name;
	Expense();
	Expense(int, string);
	friend ostream& operator<<(ostream& os, const Expense& exp);
};
