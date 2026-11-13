// ============================================================================
// modules/StockQueue.cpp
// ============================================================================
// Implementation of the Circular Array Queue
// ============================================================================

#include "StockQueue.h"
#include <iostream>
#include <iomanip>
#include <cstdio>

using namespace std;

// Constructor: Initializes empty circular queue
StockQueue::StockQueue() {
    frontIndex = 0;
    rearIndex  = -1;
    count      = 0;
    nextNumber = 1;
}

bool StockQueue::isEmpty() const {
    return count == 0;
}

bool StockQueue::isFull() const {
    return count == MAX_REQUESTS;
}

int StockQueue::size() const {
    return count;
}

// ENQUEUE: Inserts a new request at the REAR of the circular queue
// Time Complexity: O(1)
bool StockQueue::enqueue(const StockRequest& r) {
    if (isFull()) {
        return false; // Queue Overflow
    }

    // Wrap around using modulo arithmetic
    rearIndex = (rearIndex + 1) % MAX_REQUESTS;
    items[rearIndex] = r;
    count++;
    return true;
}

// DEQUEUE: Removes the request at the FRONT of the circular queue
// Time Complexity: O(1)
bool StockQueue::dequeue(StockRequest& out) {
    if (isEmpty()) {
        return false; // Queue Underflow
    }

    out = items[frontIndex];
    // Advance front index with modulo wrap-around
    frontIndex = (frontIndex + 1) % MAX_REQUESTS;
    count--;
    return true;
}

// PEEK / FRONT: Inspects the front request without removing it
// Time Complexity: O(1)
bool StockQueue::front(StockRequest& out) const {
    if (isEmpty()) {
        return false;
    }
    out = items[frontIndex];
    return true;
}

// Displays all pending requests in queue order (oldest to newest)
void StockQueue::display() const {
    if (isEmpty()) {
        cout << "  [!] No pending stock requests in the queue.\n";
        return;
    }

    cout << "  +-------------+---------+----------+------------+------------+--------------------+\n";
    cout << "  | Pos in Queue| Req ID  | Med ID   | Req Qty    | Req Date   | Status             |\n";
    cout << "  +-------------+---------+----------+------------+------------+--------------------+\n";

    for (int i = 0; i < count; i++) {
        StockRequest r = getAt(i);
        cout << "  | " << left << setw(12) << (to_string(i + 1) + (i == 0 ? " (NEXT)" : ""))
             << "| " << left << setw(8)  << r.requestID
             << "| " << left << setw(9)  << r.medicineID
             << "| " << right << setw(7) << r.requestedQuantity << "  "
             << "| " << left << setw(11) << r.requestDate
             << "| " << left << setw(19) << r.status << "|\n";
    }

    cout << "  +-------------+---------+----------+------------+------------+--------------------+\n";
    cout << "  (Capacity used: " << count << " / " << MAX_REQUESTS << " slots)\n";
}

// Visual diagram showing the circular array slots and front/rear pointers
void StockQueue::displayVisualQueue() const {
    cout << "\n  [ CIRCULAR ARRAY QUEUE BUFFER VISUALIZATION ]\n";
    cout << "  Slot Indices: ";
    for (int i = 0; i < MAX_REQUESTS; i++) {
        cout << "[" << setw(2) << i << "] ";
    }
    cout << "\n  Slot State  : ";

    for (int i = 0; i < MAX_REQUESTS; i++) {
        bool occupied = false;
        // Check if index 'i' falls within the active window from front to rear
        for (int k = 0; k < count; k++) {
            if ((frontIndex + k) % MAX_REQUESTS == i) {
                occupied = true;
                break;
            }
        }
        if (occupied) {
            if (i == frontIndex && i == rearIndex) cout << "[F/R] ";
            else if (i == frontIndex)             cout << "[ F ] ";
            else if (i == rearIndex)              cout << "[ R ] ";
            else                                  cout << "[REQ] ";
        } else {
            cout << "[ . ] ";
        }
    }
    cout << "\n";
    cout << "  Pointers: Front = " << frontIndex << ", Rear = " << rearIndex
         << " | Count = " << count << " / " << MAX_REQUESTS << "\n";
    cout << "  Wrap-around formula: next_slot = (current_slot + 1) % " << MAX_REQUESTS << "\n";
}

// Generates padded IDs like "REQ001", "REQ002", etc.
string StockQueue::makeRequestId() {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "REQ%03d", nextNumber);
    nextNumber++;
    return string(buffer);
}

int StockQueue::getNextNumber() const {
    return nextNumber;
}

void StockQueue::setNextNumber(int n) {
    nextNumber = n;
}

// Retrieves item at relative position (0 = front/oldest)
StockRequest StockQueue::getAt(int position) const {
    int index = (frontIndex + position) % MAX_REQUESTS;
    return items[index];
}
