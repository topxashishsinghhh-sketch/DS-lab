// ============================================================================
// modules/ExpiryBST.cpp
// ============================================================================
// Implementation of the Binary Search Tree for Medicine Expiry Tracking
// ============================================================================

#include "ExpiryBST.h"
#include "../common/DateUtil.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor: Initializes empty tree
ExpiryBST::ExpiryBST() {
    root = nullptr;
}

// Destructor: Post-order recursive cleanup of all tree nodes
ExpiryBST::~ExpiryBST() {
    freeTree(root);
    root = nullptr;
}

// Post-Order traversal deletion: frees left child, right child, then parent node
void ExpiryBST::freeTree(BSTNode* node) {
    if (node == nullptr) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

bool ExpiryBST::isEmpty() const {
    return root == nullptr;
}

// Recursive Insert:
// - If date < current node: recurse into left subtree
// - If date > current node: recurse into right subtree
// - If date == current node: append ID to existing node's list
BSTNode* ExpiryBST::insertNode(BSTNode* node, const string& date, const string& id) {
    if (node == nullptr) {
        BSTNode* newNode = new BSTNode;
        newNode->expiryDate = date;
        newNode->ids.push_back(id);
        newNode->left  = nullptr;
        newNode->right = nullptr;
        return newNode;
    }

    if (date < node->expiryDate) {
        node->left = insertNode(node->left, date, id);
    } else if (date > node->expiryDate) {
        node->right = insertNode(node->right, date, id);
    } else {
        // Date already exists in tree — add ID if not already in list
        bool exists = false;
        for (const string& existingId : node->ids) {
            if (existingId == id) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            node->ids.push_back(id);
        }
    }
    return node;
}

void ExpiryBST::insert(const string& date, const string& id) {
    root = insertNode(root, date, id);
}

// Recursive Search: O(log n) average lookup by date
BSTNode* ExpiryBST::searchNode(BSTNode* node, const string& date) const {
    if (node == nullptr || node->expiryDate == date) {
        return node;
    }
    if (date < node->expiryDate) {
        return searchNode(node->left, date);
    }
    return searchNode(node->right, date);
}

BSTNode* ExpiryBST::search(const string& date) const {
    return searchNode(root, date);
}

// Finds the node with the minimum key in a subtree (leftmost node)
BSTNode* ExpiryBST::findMin(BSTNode* node) const {
    while (node && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Standard BST Node Deletion (handles 0, 1, or 2 children)
BSTNode* ExpiryBST::deleteNode(BSTNode* node, const string& date) {
    if (node == nullptr) return nullptr;

    if (date < node->expiryDate) {
        node->left = deleteNode(node->left, date);
    } else if (date > node->expiryDate) {
        node->right = deleteNode(node->right, date);
    } else {
        // Found the node to delete
        // Case 1: No children (leaf node)
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }
        // Case 2: One child (right only)
        else if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        }
        // Case 2: One child (left only)
        else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }
        // Case 3: Two children -> Replace with in-order successor (min of right subtree)
        else {
            BSTNode* successor = findMin(node->right);
            node->expiryDate = successor->expiryDate;
            node->ids = successor->ids;
            node->right = deleteNode(node->right, successor->expiryDate);
        }
    }
    return node;
}

// Removes a medicine ID from a specific expiry date in the tree
// If that date has no more medicines associated with it, deletes the BST node!
bool ExpiryBST::remove(const string& date, const string& id) {
    BSTNode* node = search(date);
    if (node == nullptr) return false;

    bool found = false;
    for (auto it = node->ids.begin(); it != node->ids.end(); ++it) {
        if (*it == id) {
            node->ids.erase(it);
            found = true;
            break;
        }
    }

    // Clean up empty BST node if all IDs for this date are gone
    if (found && node->ids.empty()) {
        root = deleteNode(root, date);
    }

    return found;
}

// Helper to print all medicines stored inside a BST node
void ExpiryBST::printNode(BSTNode* node, HashTable& ht, int& count) const {
    if (node == nullptr) return;
    for (const string& id : node->ids) {
        Medicine* m = ht.search(id);
        if (m != nullptr) {
            cout << "  | " << left << setw(12) << node->expiryDate
                 << "| " << left << setw(8)  << m->medicineID
                 << "| " << left << setw(20) << (m->name.length() > 19 ? m->name.substr(0, 16) + "..." : m->name)
                 << "| " << left << setw(12) << m->category
                 << "| " << right << setw(6) << m->quantity << " units |\n";
            count++;
        }
    }
}

// IN-ORDER TRAVERSAL:
// Left subtree -> Root -> Right subtree
// Due to BST properties, this prints dates in strictly ASCENDING order!
void ExpiryBST::inorderPrint(BSTNode* node, HashTable& ht) const {
    if (node == nullptr) return;
    inorderPrint(node->left, ht);
    int count = 0;
    printNode(node, ht, count);
    inorderPrint(node->right, ht);
}

void ExpiryBST::inorder(HashTable& ht) const {
    if (root == nullptr) {
        cout << "  [!] No expiry records currently tracked in the BST.\n";
        return;
    }

    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";
    cout << "  | Expiry Date | Med ID  | Medicine Name       | Category    | Current Stock|\n";
    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";

    inorderPrint(root, ht);

    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";
    cout << "  (Ordered chronologically via BST In-Order Traversal: Left -> Root -> Right)\n";
}

// Expired Medicines Traversal (all dates < todayDate)
void ExpiryBST::expiredPrint(BSTNode* node, const string& today, HashTable& ht, int& count) const {
    if (node == nullptr) return;

    // Search left subtree (always contains earlier dates)
    expiredPrint(node->left, today, ht, count);

    // If current node is expired, print and check right subtree
    if (node->expiryDate < today) {
        printNode(node, ht, count);
        // Only inspect right subtree if there is still a chance it is expired
        expiredPrint(node->right, today, ht, count);
    }
}

void ExpiryBST::displayExpired(const string& today, HashTable& ht) const {
    int count = 0;
    cout << "  +--------------------------------------------------------------------------+\n";
    cout << "  |                     EXPIRED MEDICINES AUDIT REPORT                       |\n";
    cout << "  | Reference Date: " << left << setw(57) << today << "|\n";
    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";
    cout << "  | Expiry Date | Med ID  | Medicine Name       | Category    | Current Stock|\n";
    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";

    expiredPrint(root, today, ht, count);

    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";
    if (count == 0) {
        cout << "  [OK] No expired medicines found in inventory!\n";
    } else {
        cout << "  [DANGER] Found " << count << " expired batch(es) that must be removed immediately!\n";
    }
}

// Expiring Soon Traversal: dates within [today, today + windowDays]
void ExpiryBST::expiringPrint(BSTNode* node, int todayDays, int windowDays, HashTable& ht, int& count) const {
    if (node == nullptr) return;

    int nodeDays = dateToDays(node->expiryDate);
    int daysLeft = nodeDays - todayDays;

    // If earlier dates could fall in range, inspect left
    if (daysLeft >= 0 || node->left != nullptr) {
        expiringPrint(node->left, todayDays, windowDays, ht, count);
    }

    // Check current node
    if (daysLeft >= 0 && daysLeft <= windowDays) {
        printNode(node, ht, count);
    }

    // If later dates could fall in range, inspect right
    if (daysLeft <= windowDays) {
        expiringPrint(node->right, todayDays, windowDays, ht, count);
    }
}

void ExpiryBST::displayExpiring(const string& today, int days, HashTable& ht) const {
    int count = 0;
    int todayDays = dateToDays(today);

    cout << "  +--------------------------------------------------------------------------+\n";
    cout << "  |                 MEDICINES EXPIRING WITHIN " << right << setw(3) << days << " DAYS                     |\n";
    cout << "  | Reference Date: " << left << setw(57) << today << "|\n";
    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";
    cout << "  | Expiry Date | Med ID  | Medicine Name       | Category    | Current Stock|\n";
    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";

    expiringPrint(root, todayDays, days, ht, count);

    cout << "  +-------------+---------+---------------------+-------------+--------------+\n";
    if (count == 0) {
        cout << "  [OK] No medicines expiring within the next " << days << " days.\n";
    } else {
        cout << "  [!] Notice: " << count << " batch(es) expiring soon. Prioritize dispensary.\n";
    }
}

// ASCII Tree Visualizer
void ExpiryBST::printTreeVisual(BSTNode* node, const string& prefix, bool isLeft) const {
    if (node == nullptr) return;

    cout << prefix;
    cout << (isLeft ? "|-- (L) " : "\\-- (R) ");
    cout << "[" << node->expiryDate << " : " << node->ids.size() << " item(s)]\n";

    string nextPrefix = prefix + (isLeft ? "|   " : "    ");
    if (node->left != nullptr || node->right != nullptr) {
        if (node->left) {
            printTreeVisual(node->left, nextPrefix, true);
        } else {
            cout << nextPrefix << "|-- (L) [NULL]\n";
        }
        if (node->right) {
            printTreeVisual(node->right, nextPrefix, false);
        } else {
            cout << nextPrefix << "\\-- (R) [NULL]\n";
        }
    }
}

void ExpiryBST::displayVisualTree() const {
    cout << "\n  [ BINARY SEARCH TREE (BST) HIERARCHY VISUALIZATION ]\n";
    if (root == nullptr) {
        cout << "  Tree is empty (root == nullptr).\n";
        return;
    }
    cout << "  Root: [" << root->expiryDate << " : " << root->ids.size() << " item(s)]\n";
    if (root->left)  printTreeVisual(root->left, "  ", true);
    if (root->right) printTreeVisual(root->right, "  ", false);
    cout << "\n  Key rule: Left child < Parent date < Right child\n";
}
