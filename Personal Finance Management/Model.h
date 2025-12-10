#pragma once
#include <iostream>
#include <string>

using namespace std;

enum TransactionType { INCOME, EXPENSE };

struct Date {
    int day;
    int month;
    int year;
    Date() : day(1), month(1), year(2025) {}
    Date(int _day, int _month, int _year) : day(_day), month(_month), year(_year) {}
    bool operator < (const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    string toString() {
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }
};

struct Wallet {
    int id;
    string name;
    long long balance;
    Wallet() : id(0), name(""), balance(0) {};
    Wallet(int _id, string _name, long long _balance) : id(_id), name(_name), balance(_balance) {};
};

struct IncomeCategory {
    int id;
    string name;
    IncomeCategory(int _id = 0, string _name = "") : id(_id), name(_name) {}
};

struct ExpenseCategory {
    int id;
    string name;
    ExpenseCategory(int _id = 0, string _name = "") : id(_id), name(_name) {}
};

struct Transaction {
    int id;
    TransactionType type;
    long long amount;
    string description;
    Date date;
    int walletId;   
    int categoryId; 

    Transaction() : id(0), type(INCOME), amount(0), description(""), walletId(0), categoryId(0) {}
    Transaction(int _id, TransactionType _type, long long _amt, string _desc,
        Date _date, int _wId, int _cId)
        : id(_id), type(_type), amount(_amt), description(_desc),
        date(_date), walletId(_wId), categoryId(_cId) {
    }
};


