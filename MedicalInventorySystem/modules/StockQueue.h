#ifndef STOCKQUEUE_H
#define STOCKQUEUE_H

#include <iostream>
#include "../common/StockRequest.h"
using namespace std;

// ============================================================================
// DATA STRUCTURE 4: CIRCULAR ARRAY QUEUE
// Concept: First-In, First-Out (FIFO) queue implemented using a fixed array.
// Why used: Restock orders must be processed in the order they arrived.
//
// Wrap-around formula: next_index = (current_index + 1) % MAX
// This reuses freed spaces without shifting array elements!
// ============================================================================

const int MAX_QUEUE = 10;

class StockQueue {
private:
    StockRequest items[MAX_QUEUE]; // Array storage
    int front;                     // Index of oldest request (dequeue point)
    int rear;                      // Index of newest request (enqueue point)
    int count;                     // Total items currently in queue
    int nextId;                    // Auto-increment ticket counter

public:
    StockQueue();

    bool isFull() const { return count == MAX_QUEUE; }
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    bool enqueue(int medId, const string& medName, int qty);
    bool dequeue(StockRequest& result);
    void display() const;
    StockRequest getAt(int i) const { return items[(front + i) % MAX_QUEUE]; }
};

#endif
