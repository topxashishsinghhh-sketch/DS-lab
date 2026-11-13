// StockRequest.h - SHARED record used by the Queue module
#ifndef STOCKREQUEST_H
#define STOCKREQUEST_H

#include <string>
using namespace std;

struct StockRequest {
    string requestID;          // example: REQ001
    string medicineID;
    int requestedQuantity;
    string requestDate;        // YYYY-MM-DD
    string status;             // "Pending" or "Completed"
};

#endif
