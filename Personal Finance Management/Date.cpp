#define _CRT_SECURE_NO_WARNINGS
#include "Date.h" 
#include <cstdio> 
#include <ctime>   
#include <iomanip> 


Date::Date() {
    time_t t = time(0);
    tm* now = localtime(&t);
    day = now->tm_mday;
    month = now->tm_mon + 1;
    year = now->tm_year + 1900;
}

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }

int Date::numberOfDaysInMonth() const{
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        return isLeap ? 29 : 28;
    }
    return 31;
}

void Date::addOneMonth() {
    month++;
    if (month > 12) {
        year++;
        month = 1;
    }
    if (day > numberOfDaysInMonth()) {
        day = numberOfDaysInMonth();
    }
}

string Date::toString() const {
    char buffer[12];
    sprintf(buffer, "%04d-%02d-%02d", year, month, day);
    return string(buffer);
}

Date Date::fromString(string dateStr) {
    int d, m, y;
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d) == 3) {
        return Date(d, m, y);
    }
    return Date();
}

void Date::setIndefinite() {
    day = 0;
    month = 0;
    year = 0;
}

bool Date::isIndefinite() const {
    return (day == 0 && month == 0 && year == 0);
}

bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }

    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) return false;
    }
    return true;
}


bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const { return other < *this; }
bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}
bool Date::operator>=(const Date& other) const { return !(*this < other); }
bool Date::operator<=(const Date& other) const { return !(*this > other); }

istream& operator>>(istream& is, Date& date) {
    is >> date.day >> date.month >> date.year;
    return is;
}

ostream& operator<<(ostream& os, const Date& date) {
    os << right <<setfill('0') << setw(2) << date.day << "/"
        << right << setfill('0') << setw(2) << date.month << "/"
        << date.year;
    os << setfill(' ');
    return os;
}