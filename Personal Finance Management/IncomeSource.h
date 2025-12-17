#pragma once

#include <string>

using namespace std;

class Income {
public:
	int id;
	string name;
	Income();
	Income(int, string);
	friend ostream& operator<<(ostream& os, const Income& inc);
};
