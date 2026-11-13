// InventoryList.cpp - MEMBER 4
#include "InventoryList.h"
#include <iomanip>

InventoryList::InventoryList() {
    head = nullptr;
    total = 0;
}

// Free every node when program ends
InventoryList::~InventoryList() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

// Add a new medicine at the END of the list
void InventoryList::addMedicine(const Medicine& m) {
    Node* newNode = new Node;       // dynamic memory allocation
    newNode->data = m;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;             // list was empty
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;   // walk to the last node
        }
        current->next = newNode;
    }
    total++;
}

// Delete a medicine by ID
bool InventoryList::deleteMedicine(const string& id) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->data.medicineID == id) {
            if (previous == nullptr) {
                head = current->next;           // deleting the first node
            } else {
                previous->next = current->next; // skip over this node
            }
            delete current;
            total--;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;   // ID not found
}

// Find a medicine and return a pointer to its data (nullptr if not found)
Medicine* InventoryList::findMedicine(const string& id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.medicineID == id) {
            return &(current->data);
        }
        current = current->next;
    }
    return nullptr;
}

// Overwrite an existing record (no duplicate is created)
bool InventoryList::updateMedicine(const string& id, const Medicine& newData) {
    Medicine* m = findMedicine(id);
    if (m == nullptr) return false;
    *m = newData;
    return true;
}

bool InventoryList::stockIn(const string& id, int qty) {
    Medicine* m = findMedicine(id);
    if (m == nullptr) return false;
    m->quantity = m->quantity + qty;
    return true;
}

bool InventoryList::stockOut(const string& id, int qty) {
    Medicine* m = findMedicine(id);
    if (m == nullptr) return false;
    if (qty > m->quantity) return false;   // not enough stock
    m->quantity = m->quantity - qty;
    return true;
}

// Print the whole list as a table
void InventoryList::displayInventory() {
    if (head == nullptr) {
        cout << "  Inventory is empty." << endl;
        return;
    }
    cout << left << setw(9) << "ID" << setw(18) << "Name" << setw(10) << "Category"
         << setw(8) << "Qty" << setw(9) << "Price" << setw(12) << "Expiry" << endl;
    cout << "------------------------------------------------------------" << endl;

    Node* current = head;
    while (current != nullptr) {
        cout << left << setw(9) << current->data.medicineID
             << setw(18) << current->data.name
             << setw(10) << current->data.category
             << setw(8) << current->data.quantity
             << setw(9) << current->data.price
             << setw(12) << current->data.expiryDate << endl;
        current = current->next;
    }
    cout << "------------------------------------------------------------" << endl;
    cout << "  Total medicines: " << total << endl;
}

// Medicines whose quantity is at or below minimum stock level
void InventoryList::displayLowStock() {
    int found = 0;
    Node* current = head;
    while (current != nullptr) {
        if (current->data.quantity <= current->data.minStockLevel) {
            cout << "  " << current->data.medicineID << " - " << current->data.name
                 << " | Qty: " << current->data.quantity
                 << " | Minimum: " << current->data.minStockLevel << endl;
            found++;
        }
        current = current->next;
    }
    if (found == 0) cout << "  No low stock medicines." << endl;
}

int InventoryList::countMedicines() { return total; }

int InventoryList::totalQuantity() {
    int sum = 0;
    Node* current = head;
    while (current != nullptr) {
        sum = sum + current->data.quantity;
        current = current->next;
    }
    return sum;
}

Node* InventoryList::getHead() { return head; }
