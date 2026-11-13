// ============================================================================
// modules/InventoryList.h
// ============================================================================
// DATA STRUCTURE: Singly Linked List
// ROLE: Master Inventory Owner
//
// Key Concepts for Viva:
// 1. Dynamic Size: Memory is allocated on the heap as nodes arrive (no fixed limit).
// 2. Ownership: This linked list holds the ACTUAL master records. All other
//    structures (Hash Table, BST) merely reference or index nodes in this list.
// 3. Pointer Manipulation: Insertion at tail traverses `current->next`;
//    deletion updates `previous->next = current->next` before calling `delete current`.
//
// Time Complexity:
// - Append (tail insert) : O(n) traversal (or O(1) with tail pointer)
// - Search by ID         : O(n) linear search
// - Deletion by ID       : O(n) search + O(1) pointer adjustment
// ============================================================================

#ifndef INVENTORYLIST_H
#define INVENTORYLIST_H

#include "../common/Medicine.h"

// Each Node contains a Medicine record and a pointer to the next Node
struct Node {
    Medicine data;   // Real Medicine payload
    Node* next;      // Pointer to subsequent node, or nullptr at tail
};

class InventoryList {
private:
    Node* head;      // First node in the linked list
    int total;       // Total count of medicines currently stored

public:
    InventoryList();
    ~InventoryList();

    // Core Linked List operations
    void addMedicine(const Medicine& m);
    bool deleteMedicine(const string& id);
    bool updateMedicine(const string& id, const Medicine& newData);
    Medicine* findMedicine(const string& id);

    // Stock adjustments
    bool stockIn(const string& id, int qty);
    bool stockOut(const string& id, int qty);

    // Displays
    void displayInventory();
    void displayLowStock();
    void displayVisualList(); // Graphic ASCII diagram showing pointer links

    // Metrics
    int countMedicines() const;
    int totalQuantity() const;
    Node* getHead() const;
};

#endif // INVENTORYLIST_H
