#ifndef EXPIRYBST_H
#define EXPIRYBST_H

#include <iostream>
#include <string>
using namespace std;

// ============================================================================
// DATA STRUCTURE 3: BINARY SEARCH TREE (BST)
// Concept: Hierarchical tree where:
//          - Left child  < Parent  (earlier expiry dates)
//          - Right child > Parent  (later expiry dates)
// Why used: Allows O(log n) search by date, and In-Order Traversal automatically
//           prints medicines in sorted chronological order!
// ============================================================================

struct BSTNode {
    string expiry;    // Key (e.g. "2026-10", "2027-05")
    int medId;        // Medicine ID
    string medName;   // Medicine name
    BSTNode* left;    // Left child pointer
    BSTNode* right;   // Right child pointer
};

class ExpiryBST {
private:
    BSTNode* root;

    // Recursive helper functions:
    BSTNode* insertHelper(BSTNode* node, const string& expiry, int id, const string& name);
    void inorderHelper(BSTNode* node) const;
    void destroyHelper(BSTNode* node);

public:
    ExpiryBST();
    ~ExpiryBST();

    void insert(const string& expiry, int id, const string& name);
    void displaySorted() const; // In-order traversal
    bool isEmpty() const { return root == nullptr; }
};

#endif
