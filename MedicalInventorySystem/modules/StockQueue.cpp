#include "StockQueue.h"
#include <iomanip>

StockQueue::StockQueue() {
    front = 0;
    rear = -1;
    count = 0;
    nextId = 1;
}

// ENQUEUE (Add to rear):
// Step 1: Check for queue overflow (isFull)
// Step 2: Advance rear with wrap-around: rear = (rear + 1) % MAX
// Step 3: Put item in items[rear] and increment count
bool StockQueue::enqueue(int medId, const string& medName, int qty) {
    if (isFull()) {
        cout << "  [!] Queue is full (Overflow)! Cannot add request.\n";
        return false;
    }

    rear = (rear + 1) % MAX_QUEUE; // Wrap around to slot 0 if needed
    items[rear].requestId = nextId++;
    items[rear].medicineId = medId;
    items[rear].medicineName = medName;
    items[rear].quantity = qty;
    count++;

    return true;
}

// DEQUEUE (Remove from front - FIFO):
// Step 1: Check for queue underflow (isEmpty)
// Step 2: Grab item at items[front]
// Step 3: Advance front with wrap-around: front = (front + 1) % MAX, decrement count
bool StockQueue::dequeue(StockRequest& result) {
    if (isEmpty()) {
        cout << "  [!] Queue is empty (Underflow)! No requests waiting.\n";
        return false;
    }

    result = items[front];
    front = (front + 1) % MAX_QUEUE; // Wrap around to slot 0 if needed
    count--;

    return true;
}

// DISPLAY ALL REQUESTS (In arrival order):
void StockQueue::display() const {
    if (isEmpty()) {
        cout << "  (No pending requests in queue)\n";
        return;
    }

    cout << "  === PENDING RESTOCK REQUESTS (CIRCULAR QUEUE FIFO) ===\n";
    for (int i = 0; i < count; i++) {
        int index = (front + i) % MAX_QUEUE;
        cout << "  " << (i + 1) << ". Ticket #" << items[index].requestId 
             << " | Med ID: " << items[index].medicineId 
             << " (" << items[index].medicineName << ")"
             << " | Qty to Restock: " << items[index].quantity;
        if (i == 0) cout << "  <-- [FRONT / SERVED NEXT]";
        cout << "\n";
    }
    cout << "  Total waiting in line: " << count << " / " << MAX_QUEUE << " slots\n";
}
