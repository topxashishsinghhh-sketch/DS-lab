// StockQueue.cpp - MEMBER 3
#include "StockQueue.h"
#include <cstdio>

StockQueue::StockQueue() {
    frontIndex = 0;
    rearIndex = -1;
    count = 0;
    nextNumber = 1;
}

bool StockQueue::isEmpty() { return count == 0; }

bool StockQueue::isFull() { return count == MAX_REQUESTS; }

// Add at the REAR. The % makes the array "wrap around" (circular queue)
bool StockQueue::enqueue(const StockRequest& r) {
    if (isFull()) return false;                 // overflow
    rearIndex = (rearIndex + 1) % MAX_REQUESTS;
    items[rearIndex] = r;
    count++;
    return true;
}

// Remove from the FRONT (oldest request leaves first = FIFO)
bool StockQueue::dequeue(StockRequest& out) {
    if (isEmpty()) return false;                // underflow
    out = items[frontIndex];
    frontIndex = (frontIndex + 1) % MAX_REQUESTS;
    count--;
    return true;
}

// Peek at the oldest request without removing it
bool StockQueue::front(StockRequest& out) {
    if (isEmpty()) return false;
    out = items[frontIndex];
    return true;
}

// Print all pending requests, oldest first
void StockQueue::display() {
    if (isEmpty()) {
        cout << "  No pending requests." << endl;
        return;
    }
    for (int i = 0; i < count; i++) {
        StockRequest r = getAt(i);
        cout << "  " << (i + 1) << ". " << r.requestID << " | Medicine: " << r.medicineID
             << " | Qty: " << r.requestedQuantity << " | Date: " << r.requestDate
             << " | " << r.status << endl;
    }
    cout << "  (" << count << " of " << MAX_REQUESTS << " slots used)" << endl;
}

// Build an ID like REQ001
string StockQueue::makeRequestId() {
    char buffer[10];
    sprintf(buffer, "REQ%03d", nextNumber);
    nextNumber++;
    return string(buffer);
}

int StockQueue::getNextNumber() { return nextNumber; }
void StockQueue::setNextNumber(int n) { nextNumber = n; }
int StockQueue::size() { return count; }

StockRequest StockQueue::getAt(int position) {
    return items[(frontIndex + position) % MAX_REQUESTS];
}
