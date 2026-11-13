// ============================================================================
// modules/HashTable.cpp
// ============================================================================
// Implementation of the Hash Table with Chaining
// ============================================================================

#include "HashTable.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor: Initializes all 11 buckets to nullptr (empty chains)
HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

// Destructor: Iterates through each bucket, deallocating each HashNode chain
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

// Hash Function: Converts alphanumeric ID to bucket index [0 .. 10]
// Algorithm: Sum of ASCII codes of all characters, then modulo 11
int HashTable::hashFunction(const string& id) const {
    int sum = 0;
    for (char ch : id) {
        sum += static_cast<unsigned char>(ch);
    }
    return sum % TABLE_SIZE;
}

// Returns the calculated bucket index for an ID
int HashTable::getBucketIndex(const string& id) const {
    return hashFunction(id);
}

// Inserts a key and master-record pointer into the table
// If a collision occurs, new node is prepended to the head of the chain (O(1))
bool HashTable::insert(const string& id, Medicine* record) {
    // Check if ID already exists to prevent duplicate keys
    if (search(id) != nullptr) {
        return false;
    }

    int index = hashFunction(id);

    HashNode* newNode = new HashNode;
    newNode->id     = id;
    newNode->record = record;
    newNode->next   = table[index]; // Insert at head of the bucket list
    table[index]    = newNode;

    return true;
}

// Searches for a record by ID:
// 1. Calculates the bucket index in O(1) via hashFunction
// 2. Traverses only the short linked-list chain in that bucket (average O(1))
Medicine* HashTable::search(const string& id) {
    int index = hashFunction(id);
    HashNode* current = table[index];

    while (current != nullptr) {
        if (current->id == id) {
            return current->record; // Found match
        }
        current = current->next;
    }
    return nullptr; // Key not found
}

// Updates the record pointer for an existing ID
bool HashTable::update(const string& id, Medicine* record) {
    int index = hashFunction(id);
    HashNode* current = table[index];

    while (current != nullptr) {
        if (current->id == id) {
            current->record = record;
            return true;
        }
        current = current->next;
    }
    return false;
}

// Deletes a node from its bucket chain
bool HashTable::remove(const string& id) {
    int index = hashFunction(id);
    HashNode* current = table[index];
    HashNode* previous = nullptr;

    while (current != nullptr) {
        if (current->id == id) {
            if (previous == nullptr) {
                // Removing head of bucket chain
                table[index] = current->next;
            } else {
                // Removing intermediate or tail node of chain
                previous->next = current->next;
            }
            delete current;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false; // Key not found
}

// Displays the entire hash table structure with collision chains
// Perfect for presenting in front of the teacher!
void HashTable::displayTable() const {
    cout << "\n  +-------------------------------------------------------------------+\n";
    cout << "  |          HASH TABLE INTERNAL ARCHITECTURE (SIZE = 11)             |\n";
    cout << "  +--------+----------------------------------------------------------+\n";
    cout << "  | Bucket | Collision Chain (Keys -> Linked Records)                 |\n";
    cout << "  +--------+----------------------------------------------------------+\n";

    for (int i = 0; i < TABLE_SIZE; i++) {
        cout << "  | [" << setw(2) << right << i << "]   | ";
        HashNode* current = table[i];
        if (current == nullptr) {
            cout << "(empty)\n";
        } else {
            while (current != nullptr) {
                cout << "[" << current->id;
                if (current->record != nullptr) {
                    cout << " : " << current->record->name;
                }
                cout << "]";
                if (current->next != nullptr) {
                    cout << " -> ";
                }
                current = current->next;
            }
            cout << " -> NULL\n";
        }
    }
    cout << "  +--------+----------------------------------------------------------+\n";
    cout << "  * Notice: When two IDs map to the same bucket (e.g. MED101 & MED110),\n";
    cout << "    they form a linked list chain. This solves hash collisions!\n";
}

// Step-by-step mathematical explanation of the hash function for viva
void HashTable::explainHash(const string& id) const {
    int sum = 0;
    cout << "\n  --- HASH CALCULATION STEP-BY-STEP FOR: \"" << id << "\" ---\n";
    cout << "  Characters & ASCII values:\n  ";
    for (size_t i = 0; i < id.length(); i++) {
        char ch = id[i];
        int asciiVal = static_cast<unsigned char>(ch);
        sum += asciiVal;
        cout << "'" << ch << "'(" << asciiVal << ")";
        if (i + 1 < id.length()) cout << " + ";
    }
    cout << "\n  Sum of ASCII values = " << sum << "\n";
    cout << "  Bucket Formula      = (Sum % " << TABLE_SIZE << ") = ("
         << sum << " % " << TABLE_SIZE << ") = " << (sum % TABLE_SIZE) << "\n";
    cout << "  => Assigned to Bucket [" << (sum % TABLE_SIZE) << "]\n";
}
