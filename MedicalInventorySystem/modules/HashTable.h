// ============================================================================
// modules/HashTable.h
// ============================================================================
// DATA STRUCTURE: Hash Table with Separate Chaining
// ROLE: High-Speed O(1) Index by Medicine ID
//
// Key Concepts for Viva:
// 1. Hash Function: Computes an array index from a string key (ID).
//    Formula: sum of ASCII values % TABLE_SIZE (11).
// 2. Collision Resolution: When two distinct keys hash to the same bucket
//    (e.g., MED101 and MED110 both produce bucket 8), they are chained in a
//    singly linked list at that bucket slot.
// 3. Pointer Referencing: The hash table does not store a duplicate copy
//    of the medicine record; it stores a pointer (`Medicine*`) directly
//    to the node residing in the master InventoryList.
//
// Time Complexity:
// - Search  : Average O(1), Worst O(n) (if all keys collide)
// - Insert  : Average O(1) (prepend to bucket chain)
// - Remove  : Average O(1)
// ============================================================================

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../common/Medicine.h"
#include <string>

// Table size chosen as 11 (a prime number minimizes clustering)
const int TABLE_SIZE = 11;

// Node in the collision chain
struct HashNode {
    string id;           // Search key, e.g., "MED101"
    Medicine* record;    // Direct pointer to the master record in InventoryList
    HashNode* next;      // Pointer to next node in the same bucket chain
};

class HashTable {
private:
    HashNode* table[TABLE_SIZE]; // Fixed-size array of bucket head pointers

    // Hash function: Sum of ASCII codes % TABLE_SIZE
    int hashFunction(const string& id) const;

public:
    HashTable();
    ~HashTable();

    // Core operations
    bool insert(const string& id, Medicine* record);
    Medicine* search(const string& id);
    bool update(const string& id, Medicine* record);
    bool remove(const string& id);

    // Demonstration & Visualizations for Viva
    void displayTable() const;
    void explainHash(const string& id) const;
    int getBucketIndex(const string& id) const;
};

#endif // HASHTABLE_H
