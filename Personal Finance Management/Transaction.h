#pragma once
#include "Utils.h"
#include <string>

using namespace std;

struct Transaction {
	int id;
	long long amount;
	string category;
	string wallet;
	string description;
	Date date;
};
