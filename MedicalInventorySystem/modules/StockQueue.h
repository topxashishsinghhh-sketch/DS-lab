// ============================================================================
// modules/StockQueue.h
// ============================================================================
// DATA STRUCTURE: Circular Array Queue
// ROLE: First-In, First-Out (FIFO) Order Processing
//
// Key Concepts for Viva:
// 1. FIFO Discipline: The earliest request created is the first to be processed.
// 2. Wrap-Around Logic (Modulo Arithmetic):
//    `rear = (rear + 1) % MAX_REQUESTS`
//    `front = (front + 1) % MAX_REQUESTS`
//    This allows freed array slots at the beginning of the buffer to be reused
//    without costly O(n) array element shifts!
// 3. Overflow & Underflow Protection:
//    - Overflow occurs when `count == MAX_REQUESTS`.
//    - Underflow occurs when `count == 0`.
//
// Time Complexity:
// - Enqueue : O(1)
// - Dequeue : O(1)
// - Front   : O(1)
// ============================================================================

#ifndef STOCKQUEUE_H
#define STOCKQUEUE_H

#include "../common/StockRequest.h"
#include <string>

const int MAX_REQUESTS = 15; // Queue buffer capacity

class StockQueue {
private:
    StockRequest items[MAX_REQUESTS]; // Static circular array buffer
    int frontIndex;                   // Points to the oldest request (dequeue point)
    int rearIndex;                    // Points to the latest request (enqueue point)
    int count;                        // Number of active requests currently queued
    int nextNumber;                   // Counter for generating ID strings ("REQ001")

public:
    StockQueue();

    // Queue status
    bool isEmpty() const;
    bool isFull() const;
    int size() const;

    // Core queue operations
    bool enqueue(const StockRequest& r);
    bool dequeue(StockRequest& out);
    bool front(StockRequest& out) const;

    // Display & Visualization
    void display() const;
    void displayVisualQueue() const;

    // ID generation & persistence helpers
    string makeRequestId();
    int getNextNumber() const;
    void setNextNumber(int n);
    StockRequest getAt(int position) const; // 0 = front (oldest), count-1 = newest
};

#endif // STOCKQUEUE_H
