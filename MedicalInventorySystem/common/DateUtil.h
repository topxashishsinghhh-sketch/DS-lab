// DateUtil.h - date helpers. All dates are strings in YYYY-MM-DD format.
// Because of this format, comparing two date strings with < or > works correctly!
#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <string>
#include <ctime>
using namespace std;

// Today's date from the computer clock
inline string todayDate() {
    time_t now = time(0);
    tm* t = localtime(&now);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", t);
    return string(buffer);
}

inline bool isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Checks the text looks like YYYY-MM-DD with a sensible month and day
inline bool isValidDate(const string& d) {
    if (d.length() != 10) return false;
    if (d[4] != '-' || d[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (d[i] < '0' || d[i] > '9') return false;
    }
    int month = stoi(d.substr(5, 2));
    int day = stoi(d.substr(8, 2));
    return (month >= 1 && month <= 12 && day >= 1 && day <= 31);
}

// Converts a date into a day number so we can subtract two dates
inline int dateToDays(const string& d) {
    int year = stoi(d.substr(0, 4));
    int month = stoi(d.substr(5, 2));
    int day = stoi(d.substr(8, 2));
    int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int total = year * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
    for (int i = 0; i < month - 1; i++) total += monthDays[i];
    if (month > 2 && isLeapYear(year)) total++;
    total += day;
    return total;
}

#endif
