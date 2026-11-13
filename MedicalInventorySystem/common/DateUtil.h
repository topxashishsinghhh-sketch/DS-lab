// ============================================================================
// common/DateUtil.h
// ============================================================================
// CONCEPT: Date Utilities & Lexicographical Comparison
// Why YYYY-MM-DD?
//   In standard ISO 8601 format (YYYY-MM-DD), alphabetical (lexicographical)
//   comparison string comparison ('<' and '>') produces EXACT chronological
//   ordering. This makes Binary Search Tree comparisons straightforward:
//   "2026-05-10" < "2027-01-15" evaluates to true naturally!
// ============================================================================

#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

// Returns today's system date in "YYYY-MM-DD" format
inline string todayDate() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buffer[16];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", t);
    return string(buffer);
}

// Leap year check for accurate day counting
inline bool isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Validates whether a string strictly matches "YYYY-MM-DD"
inline bool isValidDate(const string& d) {
    if (d.length() != 10) return false;
    if (d[4] != '-' || d[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (d[i] < '0' || d[i] > '9') return false;
    }
    try {
        int month = stoi(d.substr(5, 2));
        int day   = stoi(d.substr(8, 2));
        return (month >= 1 && month <= 12 && day >= 1 && day <= 31);
    } catch (...) {
        return false;
    }
}

// Converts a "YYYY-MM-DD" date into an absolute number of days since year 0
// Enables fast difference calculation: (date2Days - date1Days) = days remaining
inline int dateToDays(const string& d) {
    if (!isValidDate(d)) return 0;
    try {
        int year  = stoi(d.substr(0, 4));
        int month = stoi(d.substr(5, 2));
        int day   = stoi(d.substr(8, 2));
        int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        int total = year * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
        for (int i = 0; i < month - 1; i++) total += monthDays[i];
        if (month > 2 && isLeapYear(year)) total++;
        total += day;
        return total;
    } catch (...) {
        return 0;
    }
}

#endif // DATEUTIL_H
