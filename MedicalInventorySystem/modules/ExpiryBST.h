// ============================================================================
// modules/ExpiryBST.h
// ============================================================================
// DATA STRUCTURE: Binary Search Tree (BST)
// ROLE: Chronological Expiry Date Index
//
// Key Concepts for Viva:
// 1. BST Property: For every node with date K:
//    - All nodes in LEFT subtree have dates < K (earlier dates)
//    - All nodes in RIGHT subtree have dates > K (later dates)
// 2. Lexicographical Ordering: "YYYY-MM-DD" formatted dates compare naturally
//    via standard string operators ('<', '>').
// 3. Multi-Value Mapping: Multiple medicines may share the same expiry date.
//    Each BST node therefore holds a list of medicine IDs (`vector<string> ids`).
// 4. In-Order Traversal: Visiting (Left Subtree -> Root -> Right Subtree)
//    automatically processes dates in ascending chronological order!
//
// Time Complexity:
// - Search  : Average O(log n), Worst O(n) (skewed tree)
// - Insert  : Average O(log n), Worst O(n)
// - In-order: O(n) visits all nodes
// ============================================================================

#ifndef EXPIRYBST_H
#define EXPIRYBST_H

#include <string>
#include <vector>
#include "HashTable.h"

// A node in the Expiry Binary Search Tree
struct BSTNode {
    string expiryDate;        // The BST key (format: YYYY-MM-DD)
    vector<string> ids;       // List of all medicine IDs expiring on this date
    BSTNode* left;            // Left child (earlier dates)
    BSTNode* right;           // Right child (later dates)
};

class ExpiryBST {
private:
    BSTNode* root;

    // Recursive helper functions
    BSTNode* insertNode(BSTNode* node, const string& date, const string& id);
    BSTNode* searchNode(BSTNode* node, const string& date) const;
    BSTNode* deleteNode(BSTNode* node, const string& date);
    BSTNode* findMin(BSTNode* node) const;

    void inorderPrint(BSTNode* node, HashTable& ht) const;
    void expiredPrint(BSTNode* node, const string& today, HashTable& ht, int& count) const;
    void expiringPrint(BSTNode* node, int todayDays, int windowDays, HashTable& ht, int& count) const;
    void printNode(BSTNode* node, HashTable& ht, int& count) const;
    void freeTree(BSTNode* node);

    // Tree ASCII visualizer helper
    void printTreeVisual(BSTNode* node, const string& prefix, bool isLeft) const;

public:
    ExpiryBST();
    ~ExpiryBST();

    // Core operations
    void insert(const string& date, const string& id);
    bool remove(const string& date, const string& id);
    BSTNode* search(const string& date) const;

    // Traversals and Queries
    void inorder(HashTable& ht) const;
    void displayExpired(const string& today, HashTable& ht) const;
    void displayExpiring(const string& today, int days, HashTable& ht) const;

    // Visual display for demo
    void displayVisualTree() const;
    bool isEmpty() const;
};

#endif // EXPIRYBST_H
