// ============================================================================
// modules/InventoryList.cpp
// ============================================================================
// Implementation of the Singly Linked List for Inventory Management
// ============================================================================

#include "InventoryList.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor: Initializes an empty linked list
InventoryList::InventoryList() {
    head = nullptr;
    total = 0;
}

// Destructor: Walks through every node, deallocating heap memory
// Prevents memory leaks when the program terminates
InventoryList::~InventoryList() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next; // Save pointer to next node
        delete current;                 // Free current node from heap
        current = nextNode;             // Advance to saved next node
    }
    head = nullptr;
    total = 0;
}

// Appends a new node at the END (tail) of the list
// Preserves the natural order of insertion from the storage file
void InventoryList::addMedicine(const Medicine& m) {
    Node* newNode = new Node;
    newNode->data = m;
    newNode->next = nullptr; // New node is the new tail

    if (head == nullptr) {
        // List was previously empty: new node becomes the head
        head = newNode;
    } else {
        // Traverse to find the current last node
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode; // Link old tail to new node
    }
    total++;
}

// Deletes a node by Medicine ID
// Handles two cases:
// 1. Target is at the head (head pointer moves to head->next)
// 2. Target is in the middle or tail (previous->next skips over current)
bool InventoryList::deleteMedicine(const string& id) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->data.medicineID == id) {
            if (previous == nullptr) {
                // Case 1: Deleting the very first node (head)
                head = current->next;
            } else {
                // Case 2: Deleting middle or tail node
                previous->next = current->next; // Re-route around target
            }
            delete current; // Free heap memory of the unlinked node
            total--;
            return true;
        }
        previous = current;      // Advance previous pointer
        current = current->next; // Advance current pointer
    }
    return false; // Target ID not found
}

// Finds medicine and returns a POINTER directly to its record
// This pointer is passed to the Hash Table for O(1) direct updates
Medicine* InventoryList::findMedicine(const string& id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.medicineID == id) {
            return &(current->data); // Return address of the medicine in node
        }
        current = current->next;
    }
    return nullptr; // Not found
}

// Overwrite existing data fields without re-allocating nodes
bool InventoryList::updateMedicine(const string& id, const Medicine& newData) {
    Medicine* m = findMedicine(id);
    if (m == nullptr) return false;
    *m = newData; // Copy new fields over
    return true;
}

// Stock-In: Increase existing inventory quantity
bool InventoryList::stockIn(const string& id, int qty) {
    if (qty <= 0) return false;
    Medicine* m = findMedicine(id);
    if (m == nullptr) return false;
    m->quantity += qty;
    return true;
}

// Stock-Out: Decrease inventory quantity (validates available stock)
bool InventoryList::stockOut(const string& id, int qty) {
    if (qty <= 0) return false;
    Medicine* m = findMedicine(id);
    if (m == nullptr) return false;
    if (qty > m->quantity) return false; // Stock underflow check
    m->quantity -= qty;
    return true;
}

// Displays inventory in a clean table
void InventoryList::displayInventory() {
    if (head == nullptr) {
        cout << "  [!] Inventory is empty. No records to display.\n";
        return;
    }

    cout << "  +---------+-------------------+-------------+--------+----------+------------+-------+\n";
    cout << "  | ID      | Name              | Category    | Qty    | Price    | Expiry     | MinSt |\n";
    cout << "  +---------+-------------------+-------------+--------+----------+------------+-------+\n";

    Node* current = head;
    while (current != nullptr) {
        printMedicineRow(current->data);
        current = current->next;
    }

    cout << "  +---------+-------------------+-------------+--------+----------+------------+-------+\n";
    cout << "  Total distinct medicines: " << total << "\n";
}

// Displays only medicines where current quantity <= minStockLevel
void InventoryList::displayLowStock() {
    int count = 0;
    cout << "  +-------------------------------------------------------------------------+\n";
    cout << "  |                       LOW STOCK ALERTS REPORT                           |\n";
    cout << "  +---------+-------------------+-------------+--------+----------+---------+\n";
    cout << "  | ID      | Name              | Category    | Qty    | MinStk   | Status  |\n";
    cout << "  +---------+-------------------+-------------+--------+----------+---------+\n";

    Node* current = head;
    while (current != nullptr) {
        if (current->data.quantity <= current->data.minStockLevel) {
            cout << "  | " << left << setw(8) << current->data.medicineID
                 << "| " << left << setw(18) << (current->data.name.length() > 17 ? current->data.name.substr(0, 14) + "..." : current->data.name)
                 << "| " << left << setw(12) << current->data.category
                 << "| " << right << setw(6) << current->data.quantity
                 << " | " << right << setw(8) << current->data.minStockLevel
                 << " | " << left << setw(8) << (current->data.quantity == 0 ? "CRITICAL" : "LOW") << "|\n";
            count++;
        }
        current = current->next;
    }

    cout << "  +---------+-------------------+-------------+--------+----------+---------+\n";
    if (count == 0) {
        cout << "  [OK] All medicines are above minimum safe stock thresholds.\n";
    } else {
        cout << "  [!] Action Required: " << count << " medicine(s) need replenishment.\n";
    }
}

// Visual diagram showing how the nodes point to each other in memory
void InventoryList::displayVisualList() {
    cout << "\n  [ LINKED LIST MEMORY & POINTER VISUALIZATION ]\n";
    cout << "  Head -> ";
    if (head == nullptr) {
        cout << "[ NULL ]\n";
        return;
    }

    Node* current = head;
    int col = 10;
    while (current != nullptr) {
        cout << "[" << current->data.medicineID << " | " << current->data.quantity << " units]";
        if (current->next != nullptr) {
            cout << " -> ";
            col += 20;
            if (col > 70) {
                cout << "\n          -> ";
                col = 13;
            }
        } else {
            cout << " -> [ NULL / nullptr ]\n";
        }
        current = current->next;
    }
    cout << "  (Total Nodes: " << total << " linked dynamically on the heap)\n";
}

int InventoryList::countMedicines() const {
    return total;
}

int InventoryList::totalQuantity() const {
    int sum = 0;
    Node* current = head;
    while (current != nullptr) {
        sum += current->data.quantity;
        current = current->next;
    }
    return sum;
}

Node* InventoryList::getHead() const {
    return head;
}
