#include "HashTable.h"

// Constructor: Set all 10 buckets to empty (nullptr)
HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

// Destructor: Clean up all collision chains from memory
HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        table[i] = nullptr;
    }
}

// INSERT INTO HASH TABLE:
// Step 1: Calculate bucket index: index = id % 10
// Step 2: Insert new node at the front of that bucket's linked list
void HashTable::insert(int id, Medicine* record) {
    int index = hashFunction(id);

    HashNode* newNode = new HashNode;
    newNode->id = id;
    newNode->record = record;
    newNode->next = table[index]; // Point to current bucket head
    table[index] = newNode;       // New node becomes the new head
}

// SEARCH HASH TABLE - O(1) average:
// Step 1: Jump directly to bucket: index = id % 10
// Step 2: Traverse only that bucket's short chain
Medicine* HashTable::search(int id) {
    int index = hashFunction(id);
    HashNode* current = table[index];

    while (current != nullptr) {
        if (current->id == id) {
            return current->record; // Found match!
        }
        current = current->next;
    }
    return nullptr; // Not found in this bucket
}

// REMOVE FROM HASH TABLE:
bool HashTable::remove(int id) {
    int index = hashFunction(id);
    HashNode* current = table[index];
    HashNode* prev = nullptr;

    while (current != nullptr) {
        if (current->id == id) {
            if (prev == nullptr) {
                table[index] = current->next; // Deleting head of chain
            } else {
                prev->next = current->next;    // Bypass current
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// DISPLAY BUCKETS AND CHAINS (Demo for Teacher):
void HashTable::displayTable() const {
    cout << "\n  === HASH TABLE BUCKETS (Formula: ID % 10) ===\n";
    for (int i = 0; i < TABLE_SIZE; i++) {
        cout << "  Bucket [" << i << "]: ";
        HashNode* current = table[i];
        if (current == nullptr) {
            cout << "(empty)\n";
        } else {
            while (current != nullptr) {
                cout << "[" << current->id;
                if (current->record) cout << ": " << current->record->name;
                cout << "]";
                if (current->next != nullptr) cout << " -> ";
                current = current->next;
            }
            cout << " -> NULL\n";
        }
    }
}
