#pragma once


#include <iostream>
#include <string>
#include <cstdio>  
#include <iomanip> 
#include <ctime>  

using namespace std;

class Date {
public:
    int day;
    int month;
    int year;
    Date();
    Date(int d, int m, int y);
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int numberOfDaysInMonth() const;
    void addOneMonth();
    string toString() const;
    static Date fromString(string dateStr);
    void setIndefinite();
    bool isIndefinite() const;
    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool isValid() const;
    friend istream& operator>>(istream& is, Date& date);
    friend ostream& operator<<(ostream& os, const Date& date);
};

