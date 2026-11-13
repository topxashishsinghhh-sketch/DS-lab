// InventoryList.h - MEMBER 4 : Singly Linked List (owns the real medicine records)
#ifndef INVENTORYLIST_H
#define INVENTORYLIST_H

#include "../common/Medicine.h"

struct Node {
    Medicine data;   // the medicine stored in this node
    Node* next;      // pointer to the next node
};

class InventoryList {
private:
    Node* head;      // first node of the list
    int total;       // number of medicines

public:
    InventoryList();
    ~InventoryList();

    void addMedicine(const Medicine& m);
    bool deleteMedicine(const string& id);
    bool updateMedicine(const string& id, const Medicine& newData);
    Medicine* findMedicine(const string& id);
    void displayInventory();

    bool stockIn(const string& id, int qty);
    bool stockOut(const string& id, int qty);
    void displayLowStock();

    int countMedicines();
    int totalQuantity();
    Node* getHead();
};

#endif
