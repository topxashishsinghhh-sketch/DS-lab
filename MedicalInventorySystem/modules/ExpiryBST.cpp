// ExpiryBST.cpp - MEMBER 2
#include "ExpiryBST.h"
#include "../common/DateUtil.h"

ExpiryBST::ExpiryBST() { root = nullptr; }

ExpiryBST::~ExpiryBST() { freeTree(root); }

void ExpiryBST::freeTree(BSTNode* node) {
    if (node == nullptr) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

// Recursive insert: smaller date goes left, bigger date goes right,
// same date -> just add the ID to that node's list
BSTNode* ExpiryBST::insertNode(BSTNode* node, const string& date, const string& id) {
    if (node == nullptr) {
        BSTNode* newNode = new BSTNode;
        newNode->expiryDate = date;
        newNode->ids.push_back(id);
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    if (date < node->expiryDate) {
        node->left = insertNode(node->left, date, id);
    } else if (date > node->expiryDate) {
        node->right = insertNode(node->right, date, id);
    } else {
        node->ids.push_back(id);     // same expiry date
    }
    return node;
}

void ExpiryBST::insert(const string& date, const string& id) {
    root = insertNode(root, date, id);
}

// Recursive search for a date
BSTNode* ExpiryBST::searchNode(BSTNode* node, const string& date) {
    if (node == nullptr) return nullptr;
    if (date == node->expiryDate) return node;
    if (date < node->expiryDate) return searchNode(node->left, date);
    return searchNode(node->right, date);
}

BSTNode* ExpiryBST::search(const string& date) {
    return searchNode(root, date);
}

// Remove one medicine ID from a date node (node stays, list may become empty)
bool ExpiryBST::remove(const string& date, const string& id) {
    BSTNode* node = searchNode(root, date);
    if (node == nullptr) return false;

    for (int i = 0; i < (int)node->ids.size(); i++) {
        if (node->ids[i] == id) {
            node->ids.erase(node->ids.begin() + i);
            return true;
        }
    }
    return false;
}

// Print every medicine stored in one node
void ExpiryBST::printNode(BSTNode* node, HashTable& ht, int& count) {
    for (int i = 0; i < (int)node->ids.size(); i++) {
        Medicine* m = ht.search(node->ids[i]);
        if (m != nullptr) {
            cout << "  " << node->expiryDate << " | " << m->medicineID
                 << " | " << m->name << " | Qty: " << m->quantity << endl;
            count++;
        }
    }
}

// INORDER: left -> node -> right  gives dates in ascending order
void ExpiryBST::inorderPrint(BSTNode* node, HashTable& ht) {
    if (node == nullptr) return;
    inorderPrint(node->left, ht);
    int count = 0;
    printNode(node, ht, count);
    inorderPrint(node->right, ht);
}

void ExpiryBST::inorder(HashTable& ht) {
    if (root == nullptr) {
        cout << "  No expiry records." << endl;
        return;
    }
    inorderPrint(root, ht);
}

// Expired = expiry date is before today
void ExpiryBST::expiredPrint(BSTNode* node, const string& today, HashTable& ht, int& count) {
    if (node == nullptr) return;
    expiredPrint(node->left, today, ht, count);
    if (node->expiryDate < today) {
        printNode(node, ht, count);
        expiredPrint(node->right, today, ht, count);   // only go right if still expired
    }
}

void ExpiryBST::displayExpired(const string& today, HashTable& ht) {
    int count = 0;
    expiredPrint(root, today, ht, count);
    if (count == 0) cout << "  No expired medicines." << endl;
}

// Expiring soon = between today and (today + days)
void ExpiryBST::expiringPrint(BSTNode* node, int todayDays, int days, HashTable& ht, int& count) {
    if (node == nullptr) return;
    expiringPrint(node->left, todayDays, days, ht, count);

    int daysLeft = dateToDays(node->expiryDate) - todayDays;
    if (daysLeft >= 0 && daysLeft <= days) {
        printNode(node, ht, count);
    }

    expiringPrint(node->right, todayDays, days, ht, count);
}

void ExpiryBST::displayExpiring(const string& today, int days, HashTable& ht) {
    int count = 0;
    expiringPrint(root, dateToDays(today), days, ht, count);
    if (count == 0) cout << "  No medicines expiring in the next " << days << " days." << endl;
}
