// ============================================================================
// common/StockRequest.h
// ============================================================================
// CONCEPT: Queue Item Record
// This structure holds a pending restocking request made by dispensary staff.
// It is processed strictly via FIFO (First-In, First-Out) using our Circular
// Queue (StockQueue).
// ============================================================================

#ifndef STOCKREQUEST_H
#define STOCKREQUEST_H

#include <string>
using namespace std;

struct StockRequest {
    string requestID;          // e.g., "REQ001", "REQ002"
    string medicineID;         // Target medicine ID, e.g., "MED101"
    int    requestedQuantity;  // Number of units to add to stock
    string requestDate;        // Request placement date (YYYY-MM-DD)
    string status;             // "Pending" or "Completed"
};

#endif // STOCKREQUEST_H
