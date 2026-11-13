#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../common/Medicine.h"

// ============================================================================
// DATA STRUCTURE 2: HASH TABLE WITH SEPARATE CHAINING
// Concept: Maps a key (Medicine ID) to an array slot using a Hash Function.
// Why used: Searching a list takes O(n). Hash table gives instant O(1) search!
//
// Hash Function: hash(id) = id % 10
// Collisions: If two IDs give the same bucket (e.g. 101 and 111 both give 1),
//             they form a linked list chain at bucket 1.
// ============================================================================

const int TABLE_SIZE = 10;

// Node in the bucket's collision chain
struct HashNode {
    int id;              // Key
    Medicine* record;    // Pointer to the medicine in the Linked List
    HashNode* next;      // Next node in this bucket chain
};

class HashTable {
private:
    HashNode* table[TABLE_SIZE]; // Array of 10 bucket head pointers

public:
    HashTable();
    ~HashTable();

    int hashFunction(int id) const { return id % TABLE_SIZE; }

    void insert(int id, Medicine* record);
    Medicine* search(int id);
    bool remove(int id);
    void displayTable() const;
};

#endif
