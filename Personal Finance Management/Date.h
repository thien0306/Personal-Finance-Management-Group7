#pragma once


#include <iostream>
#include <string>
#include <cstdio>  
#include <iomanip> 
#include <ctime>  

using namespace std;

class Date {
private:
    int day;
    int month;
    int year;
public:
    Date();
    Date(int d, int m, int y);
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int numberOfDaysInMonth() const;
    void addOneMonth();
    string toString() const;
    static Date fromString(string dateStr);
    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool isValid() const;
    friend inline istream& operator>>(istream& is, Date& date);
    friend inline ostream& operator<<(ostream& os, const Date& date);
};

