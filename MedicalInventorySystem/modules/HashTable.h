// HashTable.h - MEMBER 1 : Hash Table with chaining (fast search by Medicine ID)
// It does NOT store its own copy of the medicine. It stores a POINTER to the
// record that lives in the Linked List.
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../common/Medicine.h"

const int TABLE_SIZE = 11;     // number of buckets (array size)

struct HashNode {
    string id;           // the key
    Medicine* record;    // points to the medicine inside the Linked List
    HashNode* next;      // next node in the same bucket (chaining)
};

class HashTable {
private:
    HashNode* table[TABLE_SIZE];     // array of linked chains
    int hashFunction(const string& id);

public:
    HashTable();
    ~HashTable();

    bool insert(const string& id, Medicine* record);
    Medicine* search(const string& id);
    bool update(const string& id, Medicine* record);
    bool remove(const string& id);
    void displayTable();     // shows buckets - great for explaining collisions
};

#endif
