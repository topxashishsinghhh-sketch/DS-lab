#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <string>
using namespace std;

// In ISO format "YYYY-MM", alphabetical string comparison '<' 
// naturally gives chronological order ("2026-05" < "2027-01" is true).
inline bool isExpired(const string& expiry, const string& today = "2026-11") {
    return expiry < today;
}

#endif
