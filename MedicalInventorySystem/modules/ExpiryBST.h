// ExpiryBST.h - MEMBER 2 : Binary Search Tree keyed by expiry date
// Each node = one expiry date + a list of all medicine IDs with that date.
#ifndef EXPIRYBST_H
#define EXPIRYBST_H

#include <vector>
#include "../common/Medicine.h"
#include "HashTable.h"     // used only to look up medicine names for display

struct BSTNode {
    string expiryDate;         // the key (YYYY-MM-DD)
    vector<string> ids;        // all medicines expiring on this date
    BSTNode* left;             // earlier dates
    BSTNode* right;            // later dates
};

class ExpiryBST {
private:
    BSTNode* root;

    // helper functions (recursive)
    BSTNode* insertNode(BSTNode* node, const string& date, const string& id);
    BSTNode* searchNode(BSTNode* node, const string& date);
    void inorderPrint(BSTNode* node, HashTable& ht);
    void expiredPrint(BSTNode* node, const string& today, HashTable& ht, int& count);
    void expiringPrint(BSTNode* node, int todayDays, int days, HashTable& ht, int& count);
    void printNode(BSTNode* node, HashTable& ht, int& count);
    void freeTree(BSTNode* node);

public:
    ExpiryBST();
    ~ExpiryBST();

    void insert(const string& date, const string& id);
    bool remove(const string& date, const string& id);
    BSTNode* search(const string& date);
    void inorder(HashTable& ht);
    void displayExpired(const string& today, HashTable& ht);
    void displayExpiring(const string& today, int days, HashTable& ht);
};

#endif
