#include "ExpiryBST.h"
#include <iomanip>

ExpiryBST::ExpiryBST() {
    root = nullptr;
}

ExpiryBST::~ExpiryBST() {
    destroyHelper(root);
}

// Post-order tree cleanup (left, right, root)
void ExpiryBST::destroyHelper(BSTNode* node) {
    if (node == nullptr) return;
    destroyHelper(node->left);
    destroyHelper(node->right);
    delete node;
}

// RECURSIVE INSERT:
// Step 1: Base case - if spot is empty (nullptr), create new node
// Step 2: If new expiry is earlier (<), recurse into LEFT subtree
// Step 3: If new expiry is later (>=), recurse into RIGHT subtree
BSTNode* ExpiryBST::insertHelper(BSTNode* node, const string& expiry, int id, const string& name) {
    if (node == nullptr) {
        BSTNode* newNode = new BSTNode;
        newNode->expiry = expiry;
        newNode->medId = id;
        newNode->medName = name;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }

    if (expiry < node->expiry) {
        node->left = insertHelper(node->left, expiry, id, name);
    } else {
        node->right = insertHelper(node->right, expiry, id, name);
    }
    return node;
}

void ExpiryBST::insert(const string& expiry, int id, const string& name) {
    root = insertHelper(root, expiry, id, name);
}

// IN-ORDER TRAVERSAL: Left -> Root -> Right
// Due to BST property, this visits nodes in sorted chronological order!
void ExpiryBST::inorderHelper(BSTNode* node) const {
    if (node == nullptr) return;

    inorderHelper(node->left);  // 1. Visit earlier dates

    // 2. Print current medicine
    cout << "  | Expiry: " << setw(10) << left << node->expiry 
         << " | ID: " << setw(4) << right << node->medId 
         << " | Name: " << setw(18) << left << node->medName << " |\n";

    inorderHelper(node->right); // 3. Visit later dates
}

void ExpiryBST::displaySorted() const {
    if (root == nullptr) {
        cout << "  (Tree is empty)\n";
        return;
    }
    cout << "  === MEDICINES SORTED BY EXPIRY (BST IN-ORDER TRAVERSAL) ===\n";
    cout << "  -----------------------------------------------------------\n";
    inorderHelper(root);
    cout << "  -----------------------------------------------------------\n";
}
