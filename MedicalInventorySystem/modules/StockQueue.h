// StockQueue.h - MEMBER 3 : Queue (circular array) for stock requests, FIFO
#ifndef STOCKQUEUE_H
#define STOCKQUEUE_H

#include <iostream>
#include "../common/StockRequest.h"
using namespace std;

const int MAX_REQUESTS = 10;     // fixed size of the queue array

class StockQueue {
private:
    StockRequest items[MAX_REQUESTS];   // the array
    int frontIndex;                     // where the oldest request is
    int rearIndex;                      // where the newest request is
    int count;                          // how many requests are waiting
    int nextNumber;                     // used to create REQ001, REQ002 ...

public:
    StockQueue();

    bool isEmpty();
    bool isFull();
    bool enqueue(const StockRequest& r);      // false = overflow
    bool dequeue(StockRequest& out);          // false = underflow
    bool front(StockRequest& out);            // look at oldest, don't remove
    void display();

    string makeRequestId();
    int getNextNumber();
    void setNextNumber(int n);
    int size();
    StockRequest getAt(int position);         // position 0 = oldest
};

#endif
