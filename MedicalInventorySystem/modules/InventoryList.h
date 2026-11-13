#ifndef INVENTORYLIST_H
#define INVENTORYLIST_H

#include "../common/Medicine.h"

// ============================================================================
// DATA STRUCTURE 1: SINGLY LINKED LIST
// Concept: Dynamic memory storage where each node points to the next.
// Why used: Unlike static arrays, we can add or delete medicines at any time
//           without wasting memory or shifting elements.
// ============================================================================

// A single node in the linked list
struct Node {
    Medicine data;   // Medicine details stored here
    Node* next;      // Pointer to the next node in heap memory
};

class InventoryList {
private:
    Node* head;      // Points to the first node (nullptr if empty)
    int totalCount;  // Total medicines currently in the list

public:
    InventoryList();
    ~InventoryList();

    // 4 Core Operations to explain:
    void addMedicine(const Medicine& m);      // Insert at the end
    bool deleteMedicine(int id);              // Remove node by ID
    Medicine* searchMedicine(int id);         // Linear search by ID
    void display();                           // Print all medicines
    void displayVisualChain();                // Visual: [101] -> [102] -> NULL

    int getCount() const { return totalCount; }
    Node* getHead() const { return head; }
};

#endif
