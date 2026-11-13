#ifndef STOCKREQUEST_H
#define STOCKREQUEST_H

#include <string>
using namespace std;

// ============================================================================
// STOCK REQUEST STRUCTURE
// Used by the Queue to process supplier restocking orders in FIFO order.
// Easy to explain: "A token/ticket waiting in line to be served."
// ============================================================================
struct StockRequest {
    int requestId;       // Order ticket number (e.g. 1, 2, 3)
    int medicineId;      // Which medicine to restock
    string medicineName; // Name of medicine
    int quantity;        // How many units to order
};

#endif
