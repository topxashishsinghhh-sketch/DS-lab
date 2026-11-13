// ============================================================================
// main.cpp - Medical Inventory Management System
// Data Structures Lab Project
//
// 4 Data Structures Used:
//   1. Singly Linked List   - Master storage of medicine records
//   2. Hash Table (Chaining) - Instant O(1) lookup by Medicine ID
//   3. Binary Search Tree   - Sorted chronological expiry dates
//   4. Circular Array Queue - First-In, First-Out (FIFO) restock orders
// ============================================================================

#include <iostream>
#include <string>
#include <cstdlib>

#include "common/Medicine.h"
#include "common/StockRequest.h"
#include "modules/InventoryList.h"
#include "modules/HashTable.h"
#include "modules/ExpiryBST.h"
#include "modules/StockQueue.h"

using namespace std;

// The 4 Core Data Structures
InventoryList inventory;
HashTable     hashTable;
ExpiryBST     expiryTree;
StockQueue    orderQueue;

// Sync helper: When adding a medicine, insert into all 3 structures
void addToSystem(const Medicine& m) {
    inventory.addMedicine(m);
    Medicine* ptr = inventory.searchMedicine(m.id);
    hashTable.insert(m.id, ptr);
    expiryTree.insert(m.expiry, m.id, m.name);
}

// Pre-load sample data so the demo works right away
void loadSampleData() {
    // Note: ID 101 and ID 111 both have (ID % 10 == 1).
    // This creates an intentional collision in Bucket 1 to show the teacher!
    addToSystem({101, "Paracetamol",   "Tablet",  150, 2.50, "2027-01"});
    addToSystem({102, "Amoxicillin",   "Capsule",  15, 8.75, "2026-10"});
    addToSystem({103, "Cough Syrup",   "Syrup",    60, 4.50, "2026-11"});
    addToSystem({111, "Ibuprofen",     "Tablet",  200, 3.20, "2026-12"});
    addToSystem({104, "Vitamin C",     "Tablet",    8, 1.50, "2028-03"});

    // Pre-load 2 sample orders in the queue
    orderQueue.enqueue(102, "Amoxicillin", 50);
    orderQueue.enqueue(104, "Vitamin C",   100);
}

// Option 1: Add Medicine
void handleAddMedicine() {
    Medicine m;
    cout << "\n--- ADD NEW MEDICINE ---\n";
    cout << "Enter Medicine ID (e.g. 105): ";
    cin >> m.id;

    if (hashTable.search(m.id) != nullptr) {
        cout << "Error: ID " << m.id << " already exists in inventory!\n";
        return;
    }

    cout << "Enter Name (single word, e.g. Aspirin): ";
    cin >> m.name;
    cout << "Enter Category (e.g. Tablet): ";
    cin >> m.category;
    cout << "Enter Quantity: ";
    cin >> m.quantity;
    cout << "Enter Unit Price: ";
    cin >> m.price;
    cout << "Enter Expiry Date (YYYY-MM, e.g. 2027-08): ";
    cin >> m.expiry;

    addToSystem(m);
    cout << "\n[Success] Medicine added to Linked List, Hash Table, and BST!\n";
}

// Option 3: Search by ID (Hash Table)
void handleSearch() {
    cout << "\n--- SEARCH MEDICINE (HASH TABLE O(1)) ---\n";
    cout << "Enter Medicine ID to search: ";
    int id;
    cin >> id;

    int bucket = hashTable.hashFunction(id);
    cout << "Calculation: " << id << " % 10 = Bucket [" << bucket << "]\n";

    Medicine* m = hashTable.search(id);
    if (m != nullptr) {
        cout << "\n[Found via Hash Table]\n";
        printMedicine(*m);
    } else {
        cout << "[Not Found] No medicine with ID " << id << "\n";
    }
}

// Option 5: Delete Medicine
void handleDelete() {
    cout << "\n--- DELETE MEDICINE ---\n";
    cout << "Enter Medicine ID to delete: ";
    int id;
    cin >> id;

    if (hashTable.search(id) == nullptr) {
        cout << "Medicine with ID " << id << " not found!\n";
        return;
    }

    hashTable.remove(id);
    inventory.deleteMedicine(id);
    cout << "[Success] Medicine " << id << " deleted from Linked List and Hash Table!\n";
}

// Option 6: Request Restock (Enqueue)
void handleEnqueue() {
    cout << "\n--- REQUEST RESTOCK (QUEUE ENQUEUE) ---\n";
    cout << "Enter Medicine ID: ";
    int id;
    cin >> id;

    Medicine* m = hashTable.search(id);
    if (m == nullptr) {
        cout << "Medicine not found in inventory!\n";
        return;
    }

    int qty;
    cout << "Enter Quantity to order: ";
    cin >> qty;

    if (orderQueue.enqueue(id, m->name, qty)) {
        cout << "[Success] Restock request added to rear of circular queue!\n";
    }
}

// Option 7: Process Next Restock (Dequeue - FIFO)
void handleDequeue() {
    cout << "\n--- PROCESS RESTOCK (QUEUE DEQUEUE - FIFO) ---\n";
    StockRequest req;
    if (orderQueue.dequeue(req)) {
        cout << "[Processing Oldest Order First (FIFO)]\n";
        cout << "  Ticket #" << req.requestId << " : " << req.medicineName 
             << " (ID " << req.medicineId << ") - Qty: " << req.quantity << "\n";

        Medicine* m = hashTable.search(req.medicineId);
        if (m != nullptr) {
            m->quantity += req.quantity;
            cout << "  [Restocked] New stock for " << m->name << ": " << m->quantity << " units\n";
        }
    }
}

// Option 10: Open Browser UI
void handleOpenBrowser() {
    cout << "\nLaunching Interactive Web UI in your default browser...\n";
    #ifdef _WIN32
        system("start gui.html");
    #elif __APPLE__
        system("open gui.html");
    #else
        system("xdg-open gui.html");
    #endif
}

// Main Menu
void showMenu() {
    cout << "\n============================================================\n";
    cout << "          MEDICAL INVENTORY MANAGEMENT SYSTEM (DS LAB)       \n";
    cout << "============================================================\n";
    cout << "  [ LINKED LIST & HASH TABLE ]\n";
    cout << "    1. Add New Medicine\n";
    cout << "    2. View All Medicines         (Singly Linked List)\n";
    cout << "    3. Search Medicine by ID      (Hash Table O(1))\n";
    cout << "    4. Delete Medicine            (Linked List + Hash Table)\n";
    cout << "\n";
    cout << "  [ BINARY SEARCH TREE ]\n";
    cout << "    5. View Medicines by Expiry   (BST In-Order Traversal)\n";
    cout << "\n";
    cout << "  [ CIRCULAR QUEUE - FIFO ]\n";
    cout << "    6. Place Restock Order        (Queue Enqueue)\n";
    cout << "    7. Process Next Restock       (Queue Dequeue - FIFO)\n";
    cout << "    8. View Pending Restock Queue (Queue Status)\n";
    cout << "\n";
    cout << "  [ VIVA DEMOS & UI ]\n";
    cout << "    9. Show Hash Table Collisions (Viva Demo: Bucket Chains)\n";
    cout << "   10. Open Browser UI            (gui.html)\n";
    cout << "    0. Exit\n";
    cout << "============================================================\n";
    cout << "Enter your choice (0-10): ";
}

int main() {
    loadSampleData();

    int choice = -1;
    while (choice != 0) {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            string dummy;
            cin >> dummy;
            continue;
        }

        switch (choice) {
            case 1:  handleAddMedicine();              break;
            case 2:  inventory.display(); 
                     inventory.displayVisualChain();   break;
            case 3:  handleSearch();                   break;
            case 4:  handleDelete();                   break;
            case 5:  expiryTree.displaySorted();       break;
            case 6:  handleEnqueue();                  break;
            case 7:  handleDequeue();                  break;
            case 8:  orderQueue.display();             break;
            case 9:  hashTable.displayTable();         break;
            case 10: handleOpenBrowser();              break;
            case 0:  cout << "\nGoodbye!\n";           break;
            default: cout << "Invalid choice! Please choose 0 to 10.\n";
        }
    }

    return 0;
}
