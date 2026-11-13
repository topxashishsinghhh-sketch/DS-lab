#include "InventoryList.h"

// Constructor: Start with an empty list
InventoryList::InventoryList() {
    head = nullptr;
    totalCount = 0;
}

// Destructor: Clean up all nodes from heap memory
InventoryList::~InventoryList() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next; // Save pointer to next
        delete current;                 // Free current node
        current = nextNode;             // Move forward
    }
}

// INSERT AT END:
// Step 1: Create a new node on heap
// Step 2: If list is empty, new node becomes the head
// Step 3: Otherwise, walk to the last node and attach it
void InventoryList::addMedicine(const Medicine& m) {
    Node* newNode = new Node;
    newNode->data = m;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode; // List was empty
    } else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next; // Move to the last node
        }
        temp->next = newNode;  // Link old last node to new node
    }
    totalCount++;
}

// DELETE BY ID:
// Step 1: Check if the target is the very first node (head)
// Step 2: If not, find the node just BEFORE the target
// Step 3: Bypass target (prev->next = target->next) and delete target
bool InventoryList::deleteMedicine(int id) {
    if (head == nullptr) return false;

    // Case 1: Target is the first node
    if (head->data.id == id) {
        Node* temp = head;
        head = head->next;
        delete temp;
        totalCount--;
        return true;
    }

    // Case 2: Target is in the middle or end
    Node* prev = head;
    while (prev->next != nullptr && prev->next->data.id != id) {
        prev = prev->next;
    }

    if (prev->next != nullptr) {
        Node* target = prev->next;
        prev->next = target->next; // Skip over target
        delete target;             // Free memory
        totalCount--;
        return true;
    }

    return false; // ID not found
}

// SEARCH BY ID:
// Walks the list from head to tail until ID matches
Medicine* InventoryList::searchMedicine(int id) {
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.id == id) {
            return &(temp->data); // Return address of record
        }
        temp = temp->next;
    }
    return nullptr; // Not found
}

// DISPLAY ALL:
void InventoryList::display() {
    if (head == nullptr) {
        cout << "  (Inventory is empty)\n";
        return;
    }
    cout << "  ------------------------------------------------------------------------\n";
    Node* temp = head;
    while (temp != nullptr) {
        printMedicine(temp->data);
        temp = temp->next;
    }
    cout << "  ------------------------------------------------------------------------\n";
    cout << "  Total medicines: " << totalCount << "\n";
}

// VISUAL POINTER CHAIN:
void InventoryList::displayVisualChain() {
    cout << "  Head -> ";
    Node* temp = head;
    while (temp != nullptr) {
        cout << "[" << temp->data.id << ": " << temp->data.name << "] -> ";
        temp = temp->next;
    }
    cout << "NULL\n";
}
