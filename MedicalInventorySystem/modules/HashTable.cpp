// HashTable.cpp - MEMBER 1
#include "HashTable.h"

HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;      // every bucket starts empty
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
}

// Hash function: add all character codes, then take remainder by table size
int HashTable::hashFunction(const string& id) {
    int sum = 0;
    for (int i = 0; i < (int)id.length(); i++) {
        sum = sum + id[i];
    }
    return sum % TABLE_SIZE;
}

// Insert (returns false if the ID already exists -> no duplicates)
bool HashTable::insert(const string& id, Medicine* record) {
    if (search(id) != nullptr) return false;

    int index = hashFunction(id);
    HashNode* newNode = new HashNode;
    newNode->id = id;
    newNode->record = record;
    newNode->next = table[index];    // put new node at the front of the chain
    table[index] = newNode;          // (this is how collisions are handled)
    return true;
}

// Search: go to the bucket, then walk along its chain
Medicine* HashTable::search(const string& id) {
    int index = hashFunction(id);
    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->id == id) {
            return current->record;
        }
        current = current->next;
    }
    return nullptr;     // not found
}

// Change which record an ID points to
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

// Remove an ID from its chain
bool HashTable::remove(const string& id) {
    int index = hashFunction(id);
    HashNode* current = table[index];
    HashNode* previous = nullptr;

    while (current != nullptr) {
        if (current->id == id) {
            if (previous == nullptr) {
                table[index] = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

// Show every bucket and the IDs chained inside it
void HashTable::displayTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        cout << "  Bucket " << i << " : ";
        HashNode* current = table[i];
        if (current == nullptr) cout << "(empty)";
        while (current != nullptr) {
            cout << current->id;
            if (current->next != nullptr) cout << " -> ";
            current = current->next;
        }
        cout << endl;
    }
}
