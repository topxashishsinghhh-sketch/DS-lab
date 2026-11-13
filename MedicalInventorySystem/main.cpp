// ============================================================================
// main.cpp - Medical Inventory Management System
// ============================================================================
// Complete integration of the 4 Core Data Structures:
//   1. Singly Linked List   (modules/InventoryList) - Owns the medicine records
//   2. Hash Table + Chaining (modules/HashTable)     - Fast O(1) ID lookup
//   3. Binary Search Tree   (modules/ExpiryBST)     - Chronological expiry index
//   4. Circular Array Queue (modules/StockQueue)    - FIFO stock requests
//
// Build Command:
//   g++ -std=c++17 main.cpp modules/*.cpp -o inventory
// ============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

#include "common/Medicine.h"
#include "common/StockRequest.h"
#include "common/DateUtil.h"
#include "modules/InventoryList.h"
#include "modules/HashTable.h"
#include "modules/ExpiryBST.h"
#include "modules/StockQueue.h"

using namespace std;

// Storage File Paths
const string MEDICINE_FILE = "storage/medicines.txt";
const string REQUEST_FILE  = "storage/requests.txt";
const string REPORT_FILE   = "storage/inventory_report.txt";

// Global instances of our 4 Data Structures
InventoryList inventory;     // Linked List (Primary Storage)
HashTable     hashTable;     // Hash Table (ID Search Index)
ExpiryBST     expiryTree;    // BST (Expiry Date Index)
StockQueue    requestQueue;  // Circular Queue (Request Buffer)

// ============================================================================
//  SECTION 1: BULLET-PROOF INPUT UTILITIES
// ============================================================================

// Safely reads a non-empty string line from standard input
string readLine(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (!getline(cin, s)) {
            cin.clear();
            return "";
        }
        // Trim leading and trailing whitespace
        size_t first = s.find_first_not_of(" \t\r\n");
        if (first == string::npos) {
            // User just hit Enter; continue if empty is not allowed, or return empty
            return "";
        }
        size_t last = s.find_last_not_of(" \t\r\n");
        return s.substr(first, (last - first + 1));
    }
}

// Reads required text; replaces delimiter '|' to prevent file corruption
string readText(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        if (!s.empty()) {
            for (char &c : s) {
                if (c == '|') c = '/';
            }
            return s;
        }
        cout << "    [!] Field cannot be empty. Please enter a value.\n";
    }
}

// Reads and capitalizes an alphanumeric identifier (e.g., "med101" -> "MED101")
string readId(const string& prompt) {
    string s = readText(prompt);
    for (char &c : s) {
        c = toupper(static_cast<unsigned char>(c));
    }
    return s;
}

// Safely reads an integer with a minimum threshold
int readInt(const string& prompt, int minValue) {
    while (true) {
        string s = readLine(prompt);
        if (cin.eof()) {
            return 17; // Cleanly trigger exit on EOF/Ctrl+C
        }
        try {
            size_t pos = 0;
            int val = stoi(s, &pos);
            if (pos == s.length() && val >= minValue) {
                return val;
            }
        } catch (...) {}
        cout << "    [!] Invalid integer (minimum " << minValue << "). Try again.\n";
    }
}

// Safely reads a monetary floating-point number
double readPrice(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        if (cin.eof()) {
            return 0.0;
        }
        try {
            size_t pos = 0;
            double val = stod(s, &pos);
            if (pos == s.length() && val >= 0.0) {
                return val;
            }
        } catch (...) {}
        cout << "    [!] Invalid price (must be >= 0). Try again.\n";
    }
}

// Safely reads a date strictly matching "YYYY-MM-DD"
string readDate(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        if (isValidDate(s)) {
            return s;
        }
        cout << "    [!] Invalid format. Must be YYYY-MM-DD (e.g. 2027-01-15).\n";
    }
}

// Waits for user to press Enter before returning to menu
void pauseScreen() {
    cout << "\n  >> Press [Enter] to return to menu...";
    string dummy;
    getline(cin, dummy);
}

// ============================================================================
//  SECTION 2: MULTI-STRUCTURE SYNCHRONIZATION
// ============================================================================
// The golden rule of this system:
// 1. Linked List OWNS the Medicine record.
// 2. Hash Table stores the Medicine ID and a POINTER to the Linked List record.
// 3. Expiry BST stores the Expiry Date and the Medicine ID.
// ============================================================================

void addToAllStructures(const Medicine& m) {
    // 1. Add record to master Singly Linked List
    inventory.addMedicine(m);

    // 2. Obtain direct pointer to the newly added record in the Linked List
    Medicine* ptr = inventory.findMedicine(m.medicineID);

    // 3. Insert into Hash Table index for O(1) lookup by ID
    hashTable.insert(m.medicineID, ptr);

    // 4. Insert into Binary Search Tree for O(log n) chronological lookup by date
    expiryTree.insert(m.expiryDate, m.medicineID);
}

// ============================================================================
//  SECTION 3: FILE PERSISTENCE (storage/)
// ============================================================================

// Splits a pipe-delimited string line: "A|B|C" -> ["A", "B", "C"]
vector<string> splitLine(const string& line) {
    vector<string> parts;
    string current = "";
    for (char ch : line) {
        if (ch == '|') {
            parts.push_back(current);
            current = "";
        } else {
            current += ch;
        }
    }
    parts.push_back(current);
    return parts;
}

// Loads medicines from storage/medicines.txt into all three data structures
void loadMedicines() {
    ifstream file(MEDICINE_FILE);
    if (!file) {
        cout << "  [*] Note: storage/medicines.txt not found. Initializing sample data.\n";
        // Pre-populate with standard sample records so demo is ready out of the box
        Medicine sample1 = {"MED101", "Paracetamol", "Tablet", "ABC Pharma", 150, 2.50, "2026-01-10", "2027-01-10", 20};
        Medicine sample2 = {"MED102", "Amoxicillin", "Capsule", "HealthCare Ltd", 15, 8.75, "2025-12-01", "2026-10-01", 25};
        Medicine sample3 = {"MED103", "Cough Syrup", "Syrup", "MediLife", 60, 45.00, "2026-05-20", "2026-11-30", 10};
        Medicine sample4 = {"MED110", "Ibuprofen", "Tablet", "ABC Pharma", 200, 3.20, "2026-02-15", "2026-12-10", 30};
        Medicine sample5 = {"MED104", "Vitamin C", "Tablet", "NutriWell", 8, 1.50, "2026-03-01", "2028-03-15", 20};
        Medicine sample6 = {"MED105", "Antacid Gel", "Syrup", "MediLife", 90, 30.00, "2026-06-05", "2026-12-10", 15};

        addToAllStructures(sample1);
        addToAllStructures(sample2);
        addToAllStructures(sample3);
        addToAllStructures(sample4);
        addToAllStructures(sample5);
        addToAllStructures(sample6);
        return;
    }

    string line;
    int loaded = 0, skipped = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> f = splitLine(line);
        try {
            if (f.size() != 9) throw 1;
            Medicine m;
            m.medicineID        = f[0];
            m.name              = f[1];
            m.category          = f[2];
            m.manufacturer      = f[3];
            m.quantity          = stoi(f[4]);
            m.price             = stod(f[5]);
            m.manufacturingDate = f[6];
            m.expiryDate        = f[7];
            m.minStockLevel     = stoi(f[8]);

            if (!isValidDate(m.manufacturingDate) || !isValidDate(m.expiryDate)) throw 1;
            if (hashTable.search(m.medicineID) != nullptr) throw 1; // Duplicate key check

            addToAllStructures(m);
            loaded++;
        } catch (...) {
            skipped++;
        }
    }
    cout << "  [OK] Successfully loaded " << loaded << " medicine records from file";
    if (skipped > 0) cout << " (" << skipped << " malformed rows ignored)";
    cout << ".\n";
}

// Saves all medicines currently in the Linked List back to file
void saveMedicines() {
    ofstream file(MEDICINE_FILE);
    if (!file) return;

    Node* current = inventory.getHead();
    while (current != nullptr) {
        const Medicine& m = current->data;
        file << m.medicineID << "|" << m.name << "|" << m.category << "|"
             << m.manufacturer << "|" << m.quantity << "|" << m.price << "|"
             << m.manufacturingDate << "|" << m.expiryDate << "|"
             << m.minStockLevel << "\n";
        current = current->next;
    }
}

// Loads pending stock requests into the Circular Queue
void loadRequests() {
    ifstream file(REQUEST_FILE);
    if (!file) return;

    string line;
    if (getline(file, line)) {
        try {
            requestQueue.setNextNumber(stoi(line));
        } catch (...) {}
    }

    int loaded = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> f = splitLine(line);
        try {
            if (f.size() != 5) throw 1;
            StockRequest r;
            r.requestID         = f[0];
            r.medicineID        = f[1];
            r.requestedQuantity = stoi(f[2]);
            r.requestDate       = f[3];
            r.status            = f[4];
            if (requestQueue.enqueue(r)) loaded++;
        } catch (...) {}
    }
    cout << "  [OK] Successfully loaded " << loaded << " pending order requests.\n";
}

// Saves pending queue requests and the next request sequence number
void saveRequests() {
    ofstream file(REQUEST_FILE);
    if (!file) return;

    file << requestQueue.getNextNumber() << "\n";
    for (int i = 0; i < requestQueue.size(); i++) {
        StockRequest r = requestQueue.getAt(i);
        file << r.requestID << "|" << r.medicineID << "|" << r.requestedQuantity
             << "|" << r.requestDate << "|" << r.status << "\n";
    }
}

void saveAll() {
    saveMedicines();
    saveRequests();
    cout << "  [+] All inventory and order data saved safely to storage/ files.\n";
}

// ============================================================================
//  SECTION 4: SYSTEM CONTROLLER ACTIONS
// ============================================================================

// Action 1: Add New Medicine
void addNewMedicine() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |                     ADD NEW MEDICINE                        |\n";
    cout << "  +-------------------------------------------------------------+\n";

    Medicine m;
    m.medicineID = readId("  Enter Medicine ID (e.g. MED106): ");

    if (hashTable.search(m.medicineID) != nullptr) {
        cout << "  [ERROR] Medicine with ID '" << m.medicineID << "' already exists in inventory!\n";
        return;
    }

    m.name = readText("  Enter Medicine Name             : ");
    m.category = readText("  Enter Category (Tablet/Syrup...) : ");
    m.manufacturer = readText("  Enter Manufacturer Name         : ");
    m.quantity = readInt("  Enter Starting Stock Quantity   : ", 0);
    m.price = readPrice("  Enter Unit Price ($)            : ");
    m.manufacturingDate = readDate("  Enter Mfg Date (YYYY-MM-DD)     : ");
    m.expiryDate = readDate("  Enter Expiry Date (YYYY-MM-DD)  : ");

    if (m.expiryDate <= m.manufacturingDate) {
        cout << "  [ERROR] Expiry date must strictly be AFTER the manufacturing date!\n";
        return;
    }

    m.minStockLevel = readInt("  Enter Minimum Safe Stock Level  : ", 0);

    addToAllStructures(m);
    saveAll();

    cout << "\n  [SUCCESS] Medicine '" << m.name << "' successfully synchronized across all structures!\n";
    printMedicineCard(m);
}

// Action 2: View All Medicines
void viewAllMedicines() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |                     COMPLETE INVENTORY                      |\n";
    cout << "  +-------------------------------------------------------------+\n";
    inventory.displayInventory();
}

// Action 3: Search Medicine by ID (Demonstrates O(1) Hash Table)
void searchMedicine() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |             SEARCH MEDICINE (HASH TABLE O(1))               |\n";
    cout << "  +-------------------------------------------------------------+\n";

    string id = readId("  Enter Medicine ID to search: ");
    int bucket = hashTable.getBucketIndex(id);
    cout << "  [*] Hash function computed: hash(\"" << id << "\") -> Bucket [" << bucket << "]\n";

    Medicine* m = hashTable.search(id);
    if (m == nullptr) {
        cout << "  [!] Medicine ID '" << id << "' not found in Hash Table.\n";
    } else {
        cout << "  [FOUND] Retrieved directly from Bucket [" << bucket << "] via pointer dereference:\n";
        printMedicineCard(*m);
    }
}

// Action 4: Update Medicine Details
void updateMedicine() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |                   UPDATE MEDICINE DETAILS                   |\n";
    cout << "  +-------------------------------------------------------------+\n";

    string id = readId("  Enter Medicine ID to update: ");
    Medicine* p = hashTable.search(id);
    if (p == nullptr) {
        cout << "  [!] Medicine not found.\n";
        return;
    }

    printMedicineCard(*p);

    cout << "  Select Field to Modify:\n";
    cout << "    [1] Medicine Name\n";
    cout << "    [2] Unit Price\n";
    cout << "    [3] Expiry Date (Updates BST Index)\n";
    cout << "    [4] Minimum Stock Level\n";
    cout << "    [0] Cancel\n";

    int choice = readInt("  Enter option (0-4): ", 0);
    Medicine updated = *p;
    string oldExpiry = p->expiryDate;

    if (choice == 1) {
        updated.name = readText("  Enter new Name: ");
    } else if (choice == 2) {
        updated.price = readPrice("  Enter new Price ($): ");
    } else if (choice == 3) {
        updated.expiryDate = readDate("  Enter new Expiry Date (YYYY-MM-DD): ");
    } else if (choice == 4) {
        updated.minStockLevel = readInt("  Enter new Minimum Stock Level: ", 0);
    } else {
        cout << "  Update cancelled.\n";
        return;
    }

    inventory.updateMedicine(id, updated);

    // If expiry date was modified, update the BST index as well
    if (updated.expiryDate != oldExpiry) {
        expiryTree.remove(oldExpiry, id);
        expiryTree.insert(updated.expiryDate, id);
        cout << "  [*] Binary Search Tree re-indexed: moved from " << oldExpiry << " to " << updated.expiryDate << "\n";
    }

    saveAll();
    cout << "  [SUCCESS] Medicine record updated successfully.\n";
}

// Action 5: Delete Medicine
void deleteMedicine() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |                      DELETE MEDICINE                        |\n";
    cout << "  +-------------------------------------------------------------+\n";

    string id = readId("  Enter Medicine ID to delete: ");
    Medicine* p = hashTable.search(id);
    if (p == nullptr) {
        cout << "  [!] Medicine not found.\n";
        return;
    }

    cout << "  Target to be removed:\n";
    printMedicineCard(*p);

    string confirm = readLine("  Are you sure you want to permanently delete this medicine? (y/N): ");
    if (confirm != "y" && confirm != "Y") {
        cout << "  Deletion cancelled.\n";
        return;
    }

    string expiry = p->expiryDate;

    // Remove synchronously from all structures:
    expiryTree.remove(expiry, id); // 1. Remove from BST
    hashTable.remove(id);          // 2. Remove from Hash Table
    inventory.deleteMedicine(id);  // 3. Remove from master Linked List

    saveAll();
    cout << "  [SUCCESS] Medicine '" << id << "' permanently deleted from all data structures.\n";
}

// Action 6: Manage Expiry Dates (BST In-Order Display)
void manageExpiry() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |          MEDICINES EXPIRY CHRONOLOGY (BST IN-ORDER)         |\n";
    cout << "  +-------------------------------------------------------------+\n";
    expiryTree.inorder(hashTable);

    cout << "\n  Search a specific expiry date? (Press Enter to skip):\n";
    string d = readLine("  Enter Date (YYYY-MM-DD) or blank: ");
    if (d.empty()) return;

    if (!isValidDate(d)) {
        cout << "  [!] Invalid date format.\n";
        return;
    }

    BSTNode* node = expiryTree.search(d);
    if (node == nullptr || node->ids.empty()) {
        cout << "  [!] No medicines in inventory expire on " << d << ".\n";
    } else {
        cout << "  [FOUND] Medicines expiring on " << d << ":\n";
        for (const string& id : node->ids) {
            Medicine* m = hashTable.search(id);
            if (m) cout << "    -> " << m->medicineID << " : " << m->name << " (Stock: " << m->quantity << ")\n";
        }
    }
}

// Action 7: View Expired Medicines
void viewExpired() {
    string today = todayDate();
    expiryTree.displayExpired(today, hashTable);
}

// Action 8: View Medicines Expiring Soon
void viewExpiringSoon() {
    string today = todayDate();
    int days = readInt("  Show medicines expiring within how many days? (1-365): ", 1);
    expiryTree.displayExpiring(today, days, hashTable);
}

// Action 9: Stock-In (Restock inventory)
void stockIn() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |                      STOCK-IN (RESTOCK)                     |\n";
    cout << "  +-------------------------------------------------------------+\n";

    string id = readId("  Enter Medicine ID: ");
    Medicine* m = hashTable.search(id);
    if (m == nullptr) {
        cout << "  [!] Medicine not found in inventory.\n";
        return;
    }

    cout << "  Current Stock for " << m->name << " : " << m->quantity << " units.\n";
    int qty = readInt("  Enter quantity to ADD: ", 1);

    inventory.stockIn(id, qty);
    saveAll();

    cout << "  [SUCCESS] Added " << qty << " units. New quantity: " << m->quantity << " units.\n";
}

// Action 10: Stock-Out (Dispense medicine)
void stockOut() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |                     STOCK-OUT (DISPENSE)                    |\n";
    cout << "  +-------------------------------------------------------------+\n";

    string id = readId("  Enter Medicine ID: ");
    Medicine* m = hashTable.search(id);
    if (m == nullptr) {
        cout << "  [!] Medicine not found in inventory.\n";
        return;
    }

    cout << "  Current Stock for " << m->name << " : " << m->quantity << " units (Min Safe: " << m->minStockLevel << ").\n";
    int qty = readInt("  Enter quantity to DISPENSE: ", 1);

    if (!inventory.stockOut(id, qty)) {
        cout << "  [ERROR] Insufficient stock! Only " << m->quantity << " units available.\n";
        return;
    }

    saveAll();
    cout << "  [SUCCESS] Dispensed " << qty << " units. Remaining: " << m->quantity << " units.\n";
    if (m->quantity <= m->minStockLevel) {
        cout << "  [WARNING] Stock is at or below minimum threshold (" << m->minStockLevel << ")! Reorder suggested.\n";
    }
}

// Action 11: Create Stock Request (Enqueue)
void createRequest() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |            CREATE STOCK REQUEST (QUEUE ENQUEUE)             |\n";
    cout << "  +-------------------------------------------------------------+\n";

    if (requestQueue.isFull()) {
        cout << "  [ERROR] Request Queue is FULL (Capacity = " << MAX_REQUESTS << ").\n";
        cout << "  Please process pending requests before enqueuing new ones.\n";
        return;
    }

    string id = readId("  Enter Target Medicine ID: ");
    Medicine* m = hashTable.search(id);
    if (m == nullptr) {
        cout << "  [!] Target medicine not found in inventory.\n";
        return;
    }

    int qty = readInt("  Enter quantity to request: ", 1);

    StockRequest r;
    r.requestID         = requestQueue.makeRequestId();
    r.medicineID        = id;
    r.requestedQuantity = qty;
    r.requestDate       = todayDate();
    r.status            = "Pending";

    requestQueue.enqueue(r);
    saveRequests();

    cout << "  [SUCCESS] Request " << r.requestID << " for " << qty << " units of "
         << m->name << " enqueued at circular rear.\n";
}

// Action 12: Process Stock Request (Dequeue - FIFO)
void processRequest() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |           PROCESS STOCK REQUEST (QUEUE DEQUEUE)             |\n";
    cout << "  +-------------------------------------------------------------+\n";

    StockRequest r;
    if (!requestQueue.dequeue(r)) {
        cout << "  [!] Queue Underflow: No pending requests waiting in the queue.\n";
        return;
    }

    cout << "  Processing Oldest Request (FIFO):\n";
    cout << "    Request ID : " << r.requestID << "\n";
    cout << "    Medicine ID: " << r.medicineID << "\n";
    cout << "    Quantity   : " << r.requestedQuantity << "\n";
    cout << "    Date Placed: " << r.requestDate << "\n";

    Medicine* m = hashTable.search(r.medicineID);
    if (m == nullptr) {
        cout << "  [!] Medicine " << r.medicineID << " was deleted from inventory. Discarding request.\n";
    } else {
        inventory.stockIn(r.medicineID, r.requestedQuantity);
        cout << "  [SUCCESS] Stock replenished! Added " << r.requestedQuantity
             << " units to " << m->name << ". New balance: " << m->quantity << " units.\n";
    }

    saveAll();
}

// Action 13: View Pending Requests
void viewRequests() {
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |             PENDING STOCK REQUESTS (QUEUE FIFO)             |\n";
    cout << "  +-------------------------------------------------------------+\n";
    requestQueue.display();
}

// Action 14: Low Stock Medicines Report
void lowStock() {
    inventory.displayLowStock();
}

// Action 15: Generate Comprehensive Inventory Report File
void generateReport() {
    ofstream file(REPORT_FILE);
    if (!file) {
        cout << "  [ERROR] Could not open " << REPORT_FILE << " for writing.\n";
        return;
    }

    string today = todayDate();
    double totalVal = 0.0;
    Node* current = inventory.getHead();

    while (current != nullptr) {
        totalVal += (current->data.quantity * current->data.price);
        current = current->next;
    }

    file << "========================================================================\n";
    file << "                 HOSPITAL MEDICAL INVENTORY AUDIT REPORT                \n";
    file << "========================================================================\n";
    file << "Generated On    : " << today << "\n";
    file << "Total Medicines : " << inventory.countMedicines() << "\n";
    file << "Total Units     : " << inventory.totalQuantity() << "\n";
    file << "Inventory Worth : $" << fixed << setprecision(2) << totalVal << "\n";
    file << "Queued Requests : " << requestQueue.size() << "\n\n";

    file << "--- CRITICAL & LOW STOCK ITEMS ---\n";
    current = inventory.getHead();
    int lowCount = 0;
    while (current != nullptr) {
        if (current->data.quantity <= current->data.minStockLevel) {
            file << "  * [" << current->data.medicineID << "] " << left << setw(20) << current->data.name
                 << " Qty: " << current->data.quantity << " (Threshold: " << current->data.minStockLevel << ")\n";
            lowCount++;
        }
        current = current->next;
    }
    if (lowCount == 0) file << "  (None. Stock levels healthy)\n";

    file << "\n--- EXPIRED INVENTORY ITEMS ---\n";
    current = inventory.getHead();
    int expCount = 0;
    while (current != nullptr) {
        if (current->data.expiryDate < today) {
            file << "  * [" << current->data.medicineID << "] " << left << setw(20) << current->data.name
                 << " Expired On: " << current->data.expiryDate << " (Qty: " << current->data.quantity << ")\n";
            expCount++;
        }
        current = current->next;
    }
    if (expCount == 0) file << "  (None. All stock unexpired)\n";

    file << "\n========================================================================\n";
    file << "Report generated successfully by Medical Inventory System\n";
    file.close();

    cout << "\n  [SUCCESS] Comprehensive audit report written to: " << REPORT_FILE << "\n";
}

// Action 18: Hash Table Collision & Buckets Visualizer
void showHashTableDemo() {
    hashTable.displayTable();

    cout << "\n  Would you like to see the step-by-step hash formula for an ID?\n";
    string id = readLine("  Enter ID (e.g. MED101) or leave blank to return: ");
    if (!id.empty()) {
        hashTable.explainHash(id);
    }
}

// Action 19: Visual DS Architecture Inspector (For Viva & Presentation)
void showDataStructureInspector() {
    cout << "\n  ==================================================================\n";
    cout << "                 DATA STRUCTURES LAB VISUAL INSPECTOR               \n";
    cout << "  ==================================================================\n";
    cout << "  Select structure to inspect visually:\n";
    cout << "    [1] Singly Linked List  (Master Records & Heap Pointers)\n";
    cout << "    [2] Hash Table          (Buckets & Collision Chains)\n";
    cout << "    [3] Binary Search Tree  (Expiry Hierarchy & Branches)\n";
    cout << "    [4] Circular Queue      (Front/Rear Slots & Modulo Arithmetic)\n";
    cout << "    [5] All Structures Simultaneously\n";
    cout << "    [0] Return to Main Menu\n";

    int choice = readInt("  Your choice (0-5): ", 0);

    if (choice == 1 || choice == 5) {
        inventory.displayVisualList();
    }
    if (choice == 2 || choice == 5) {
        hashTable.displayTable();
    }
    if (choice == 3 || choice == 5) {
        expiryTree.displayVisualTree();
    }
    if (choice == 4 || choice == 5) {
        requestQueue.displayVisualQueue();
    }
}

// Action 20: Launch Interactive Web Browser UI
void openBrowserUI() {
    cout << "\n  [*] Launching Interactive Web UI in your default browser...\n";
    #ifdef _WIN32
        system("start gui.html");
    #elif __APPLE__
        system("open gui.html");
    #else
        system("xdg-open gui.html");
    #endif
    cout << "  [SUCCESS] Opened gui.html! Check your browser window.\n";
}

// ============================================================================
//  SECTION 5: MAIN MENU & RUNTIME LOOP
// ============================================================================

void printBanner() {
    cout << "\n";
    cout << "  ==================================================================\n";
    cout << "  ||                                                              ||\n";
    cout << "  ||             MEDICAL INVENTORY MANAGEMENT SYSTEM              ||\n";
    cout << "  ||             Data Structures Laboratory Project               ||\n";
    cout << "  ||                                                              ||\n";
    cout << "  ||   Structures: Linked List | Hash Table | BST | Circular Queue||\n";
    cout << "  ==================================================================\n";
    cout << "  Today's System Date: " << todayDate() << "\n";
}

void showMenu() {
    printBanner();
    cout << "  [ MEDICINE MANAGEMENT - LINKED LIST & HASH TABLE ]\n";
    cout << "    1. Add New Medicine               (Synchronize all structures)\n";
    cout << "    2. View All Medicines             (Singly Linked List)\n";
    cout << "    3. Search Medicine by ID          (Hash Table O(1))\n";
    cout << "    4. Update Medicine Details        (In-place modification)\n";
    cout << "    5. Delete Medicine                (Deallocate from all)\n";
    cout << "\n";
    cout << "  [ EXPIRY MANAGEMENT - BINARY SEARCH TREE ]\n";
    cout << "    6. View Expiry Chronology         (BST In-Order Traversal)\n";
    cout << "    7. Audit Expired Medicines        (Dates < Today)\n";
    cout << "    8. View Medicines Expiring Soon   (Within N days window)\n";
    cout << "\n";
    cout << "  [ STOCK & ORDER PROCESSING - CIRCULAR QUEUE ]\n";
    cout << "    9. Stock-In  (Replenish inventory)\n";
    cout << "   10. Stock-Out (Dispense to dispensary)\n";
    cout << "   11. Create Stock Request           (Queue Enqueue - FIFO)\n";
    cout << "   12. Process Next Stock Request     (Queue Dequeue - FIFO)\n";
    cout << "   13. View Pending Stock Requests    (Queue Status)\n";
    cout << "\n";
    cout << "  [ REPORTS, VISUALIZATIONS & VIVA TOOLS ]\n";
    cout << "   14. View Low Stock Alerts\n";
    cout << "   15. Generate Audit Report File\n";
    cout << "   16. Save All Data to Storage\n";
    cout << "   17. Exit System\n";
    cout << "   18. Hash Table Internals & Collisions Demo\n";
    cout << "   19. [VIVA DEMO] Visual Data Structure Inspector\n";
    cout << "   20. [BROWSER GUI] Launch Interactive UI in Browser\n";
    cout << "  ==================================================================\n";
}

int main() {
    cout << "  Initializing Medical Inventory System...\n";
    loadMedicines(); // Loads into Linked List + Hash Table + BST
    loadRequests();  // Loads into Circular Queue

    int choice = 0;
    while (choice != 17) {
        showMenu();
        choice = readInt("  Enter your choice (1-20): ", 1);

        switch (choice) {
            case 1:  addNewMedicine();            break;
            case 2:  viewAllMedicines();          break;
            case 3:  searchMedicine();            break;
            case 4:  updateMedicine();            break;
            case 5:  deleteMedicine();            break;
            case 6:  manageExpiry();              break;
            case 7:  viewExpired();               break;
            case 8:  viewExpiringSoon();          break;
            case 9:  stockIn();                   break;
            case 10: stockOut();                  break;
            case 11: createRequest();             break;
            case 12: processRequest();            break;
            case 13: viewRequests();              break;
            case 14: lowStock();                  break;
            case 15: generateReport();            break;
            case 16: saveAll();                   break;
            case 17:
                saveAll();
                cout << "\n  [GOODBYE] Data safely persisted. Exiting system.\n\n";
                break;
            case 18: showHashTableDemo();         break;
            case 19: showDataStructureInspector();break;
            case 20: openBrowserUI();             break;
            default:
                cout << "  [!] Invalid selection. Please enter a number from 1 to 20.\n";
                break;
        }

        if (choice != 17) {
            pauseScreen();
        }
    }

    return 0;
}
