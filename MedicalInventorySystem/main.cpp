// main.cpp - Menu + integration. It only COORDINATES the four modules.
// Compile:  g++ -std=c++17 main.cpp modules/*.cpp -o inventory
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "common/Medicine.h"
#include "common/StockRequest.h"
#include "common/DateUtil.h"
#include "modules/InventoryList.h"
#include "modules/HashTable.h"
#include "modules/ExpiryBST.h"
#include "modules/StockQueue.h"
using namespace std;

const string MEDICINE_FILE = "storage/medicines.txt";
const string REQUEST_FILE  = "storage/requests.txt";
const string REPORT_FILE   = "storage/inventory_report.txt";

// The four data structures (one object each)
InventoryList inventory;     // Member 4 - Linked List (owns the records)
HashTable     hashTable;     // Member 1 - Hash Table  (search index)
ExpiryBST     expiryTree;    // Member 2 - BST         (expiry index)
StockQueue    requestQueue;  // Member 3 - Queue       (stock requests)

// =====================================================================
//  INPUT HELPERS - read from keyboard safely
// =====================================================================
string readLine(const string& prompt) {
    string s;
    cout << prompt;
    if (!getline(cin, s)) exit(0);     // end of input -> stop program
    return s;
}

// Text that must not be empty. '|' is replaced because it separates fields in the file
string readText(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        for (int i = 0; i < (int)s.length(); i++) {
            if (s[i] == '|') s[i] = '/';
        }
        if (s != "") return s;
        cout << "  Cannot be empty. Try again." << endl;
    }
}

string readId(const string& prompt) {
    string s = readText(prompt);
    for (int i = 0; i < (int)s.length(); i++) s[i] = toupper(s[i]);   // med101 -> MED101
    return s;
}

int readInt(const string& prompt, int minValue) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t used;
            int value = stoi(s, &used);
            if (used == s.length() && value >= minValue) return value;
        } catch (...) { }
        cout << "  Enter a whole number (minimum " << minValue << ")." << endl;
    }
}

double readPrice(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t used;
            double value = stod(s, &used);
            if (used == s.length() && value >= 0) return value;
        } catch (...) { }
        cout << "  Enter a valid price (0 or more)." << endl;
    }
}

string readDate(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        if (isValidDate(s)) return s;
        cout << "  Use the format YYYY-MM-DD (example 2027-01-10)." << endl;
    }
}

void pause() {
    readLine("\n  Press Enter to continue...");
}

// =====================================================================
//  INTEGRATION - keep Linked List, Hash Table and BST in sync
// =====================================================================

// Used by "Add Medicine" and by "Load from file"
void addToAllStructures(const Medicine& m) {
    inventory.addMedicine(m);                           // 1. Linked List stores the record
    Medicine* p = inventory.findMedicine(m.medicineID); //    get pointer to that record
    hashTable.insert(m.medicineID, p);                  // 2. Hash Table indexes the ID
    expiryTree.insert(m.expiryDate, m.medicineID);      // 3. BST indexes the expiry date
}

// =====================================================================
//  FILE HANDLING
// =====================================================================

// Split "a|b|c" into a vector of pieces
vector<string> splitLine(const string& line) {
    vector<string> parts;
    string piece = "";
    for (int i = 0; i < (int)line.length(); i++) {
        if (line[i] == '|') {
            parts.push_back(piece);
            piece = "";
        } else {
            piece = piece + line[i];
        }
    }
    parts.push_back(piece);
    return parts;
}

void loadMedicines() {
    ifstream file(MEDICINE_FILE);
    if (!file) {
        cout << "  No medicines file found. Starting with empty inventory." << endl;
        return;
    }
    string line;
    int loaded = 0, skipped = 0;
    while (getline(file, line)) {
        if (line == "") continue;
        vector<string> f = splitLine(line);
        try {
            if (f.size() != 9) throw 1;
            Medicine m;
            m.medicineID = f[0];
            m.name = f[1];
            m.category = f[2];
            m.manufacturer = f[3];
            m.quantity = stoi(f[4]);
            m.price = stod(f[5]);
            m.manufacturingDate = f[6];
            m.expiryDate = f[7];
            m.minStockLevel = stoi(f[8]);
            if (!isValidDate(m.manufacturingDate) || !isValidDate(m.expiryDate)) throw 1;
            if (hashTable.search(m.medicineID) != nullptr) throw 1;   // duplicate ID
            addToAllStructures(m);
            loaded++;
        } catch (...) {
            skipped++;      // malformed line
        }
    }
    cout << "  Loaded " << loaded << " medicine(s)";
    if (skipped > 0) cout << " (" << skipped << " bad line(s) skipped)";
    cout << "." << endl;
}

void saveMedicines() {
    ofstream file(MEDICINE_FILE);
    Node* current = inventory.getHead();
    while (current != nullptr) {
        Medicine& m = current->data;
        file << m.medicineID << "|" << m.name << "|" << m.category << "|"
             << m.manufacturer << "|" << m.quantity << "|" << m.price << "|"
             << m.manufacturingDate << "|" << m.expiryDate << "|"
             << m.minStockLevel << endl;
        current = current->next;
    }
}

// requests.txt: first line = next request number, then one request per line
void loadRequests() {
    ifstream file(REQUEST_FILE);
    if (!file) return;
    string line;
    if (getline(file, line)) {
        try { requestQueue.setNextNumber(stoi(line)); } catch (...) { }
    }
    int loaded = 0;
    while (getline(file, line)) {
        if (line == "") continue;
        vector<string> f = splitLine(line);
        try {
            if (f.size() != 5) throw 1;
            StockRequest r;
            r.requestID = f[0];
            r.medicineID = f[1];
            r.requestedQuantity = stoi(f[2]);
            r.requestDate = f[3];
            r.status = f[4];
            if (requestQueue.enqueue(r)) loaded++;
        } catch (...) { }
    }
    cout << "  Loaded " << loaded << " pending request(s)." << endl;
}

void saveRequests() {
    ofstream file(REQUEST_FILE);
    file << requestQueue.getNextNumber() << endl;
    for (int i = 0; i < requestQueue.size(); i++) {
        StockRequest r = requestQueue.getAt(i);
        file << r.requestID << "|" << r.medicineID << "|" << r.requestedQuantity
             << "|" << r.requestDate << "|" << r.status << endl;
    }
}

void saveAll() {
    saveMedicines();
    saveRequests();
    cout << "  Data saved." << endl;
}

// =====================================================================
//  MENU ACTIONS
// =====================================================================

// 1. Add New Medicine
void addNewMedicine() {
    cout << "\n--- ADD NEW MEDICINE ---" << endl;
    Medicine m;
    m.medicineID = readId("  Medicine ID (example MED101): ");
    if (hashTable.search(m.medicineID) != nullptr) {      // Hash Table checks duplicates
        cout << "  Error: this ID already exists." << endl;
        return;
    }
    m.name = readText("  Name: ");
    m.category = readText("  Category (Tablet/Syrup/...): ");
    m.manufacturer = readText("  Manufacturer: ");
    m.quantity = readInt("  Quantity: ", 0);
    m.price = readPrice("  Price per unit: ");
    m.manufacturingDate = readDate("  Manufacturing date (YYYY-MM-DD): ");
    m.expiryDate = readDate("  Expiry date (YYYY-MM-DD): ");
    if (m.expiryDate <= m.manufacturingDate) {
        cout << "  Error: expiry date must be after manufacturing date." << endl;
        return;
    }
    m.minStockLevel = readInt("  Minimum stock level: ", 0);

    addToAllStructures(m);
    cout << "\n  Medicine added successfully!" << endl;
}

// 2. View All Medicines
void viewAllMedicines() {
    cout << "\n--- ALL MEDICINES ---" << endl;
    inventory.displayInventory();
}

// 3. Search Medicine by ID
void searchMedicine() {
    cout << "\n--- SEARCH MEDICINE ---" << endl;
    string id = readId("  Enter Medicine ID: ");
    Medicine* m = hashTable.search(id);                   // Hash Table lookup
    if (m == nullptr) {
        cout << "  Medicine not found." << endl;
    } else {
        cout << "\n  Medicine found:" << endl;
        printMedicineDetails(*m);
    }
}

// 4. Update Medicine Details
void updateMedicine() {
    cout << "\n--- UPDATE MEDICINE ---" << endl;
    string id = readId("  Enter Medicine ID: ");
    Medicine* p = hashTable.search(id);
    if (p == nullptr) {
        cout << "  Medicine not found." << endl;
        return;
    }
    Medicine updated = *p;            // work on a copy
    string oldExpiry = p->expiryDate;

    cout << "  What do you want to update?" << endl;
    cout << "    1. Name" << endl;
    cout << "    2. Price" << endl;
    cout << "    3. Expiry date" << endl;
    cout << "    4. Minimum stock level" << endl;
    int choice = readInt("  Choice: ", 1);

    if (choice == 1) updated.name = readText("  New name: ");
    else if (choice == 2) updated.price = readPrice("  New price: ");
    else if (choice == 3) updated.expiryDate = readDate("  New expiry date: ");
    else if (choice == 4) updated.minStockLevel = readInt("  New minimum stock: ", 0);
    else { cout << "  Invalid choice." << endl; return; }

    inventory.updateMedicine(id, updated);                // Linked List changes the record
    if (updated.expiryDate != oldExpiry) {                // BST must follow the change
        expiryTree.remove(oldExpiry, id);
        expiryTree.insert(updated.expiryDate, id);
    }
    cout << "  Medicine updated." << endl;
}

// 5. Delete Medicine
void deleteMedicine() {
    cout << "\n--- DELETE MEDICINE ---" << endl;
    string id = readId("  Enter Medicine ID: ");
    Medicine* p = hashTable.search(id);
    if (p == nullptr) {
        cout << "  Medicine not found." << endl;
        return;
    }
    string expiry = p->expiryDate;       // remember BEFORE the record is deleted

    expiryTree.remove(expiry, id);       // remove from BST
    hashTable.remove(id);                // remove from Hash Table
    inventory.deleteMedicine(id);        // remove from Linked List
    cout << "  Medicine deleted from all structures." << endl;
}

// 6. Manage Expiry Dates (sorted view + search a date)
void manageExpiry() {
    cout << "\n--- EXPIRY DATES (earliest first) ---" << endl;
    expiryTree.inorder(hashTable);
    cout << "\n  Search a specific expiry date? (press Enter to skip)" << endl;
    string d = readLine("  Date (YYYY-MM-DD): ");
    if (d == "") return;
    if (!isValidDate(d)) { cout << "  Invalid date format." << endl; return; }
    BSTNode* node = expiryTree.search(d);
    if (node == nullptr || node->ids.empty()) {
        cout << "  No medicine expires on " << d << "." << endl;
    } else {
        cout << "  Medicines expiring on " << d << ":" << endl;
        for (int i = 0; i < (int)node->ids.size(); i++) {
            cout << "    " << node->ids[i] << endl;
        }
    }
}

// 7. View Expired Medicines
void viewExpired() {
    cout << "\n--- EXPIRED MEDICINES (today: " << todayDate() << ") ---" << endl;
    expiryTree.displayExpired(todayDate(), hashTable);
}

// 8. View Medicines Expiring Soon
void viewExpiringSoon() {
    cout << "\n--- EXPIRING SOON ---" << endl;
    int days = readInt("  Show medicines expiring within how many days? ", 1);
    cout << endl;
    expiryTree.displayExpiring(todayDate(), days, hashTable);
}

// 9. Stock-In
void stockIn() {
    cout << "\n--- STOCK-IN ---" << endl;
    string id = readId("  Medicine ID: ");
    if (hashTable.search(id) == nullptr) { cout << "  Medicine not found." << endl; return; }
    int qty = readInt("  Quantity to add: ", 1);
    inventory.stockIn(id, qty);
    cout << "  New quantity: " << hashTable.search(id)->quantity << endl;
}

// 10. Stock-Out
void stockOut() {
    cout << "\n--- STOCK-OUT ---" << endl;
    string id = readId("  Medicine ID: ");
    Medicine* m = hashTable.search(id);
    if (m == nullptr) { cout << "  Medicine not found." << endl; return; }
    int qty = readInt("  Quantity to remove: ", 1);
    if (!inventory.stockOut(id, qty)) {
        cout << "  Error: only " << m->quantity << " in stock." << endl;
        return;
    }
    cout << "  New quantity: " << m->quantity << endl;
    if (m->quantity <= m->minStockLevel) {
        cout << "  WARNING: stock is at or below minimum level (" << m->minStockLevel << ")!" << endl;
    }
}

// 11. Create Stock Request (ENQUEUE)
void createRequest() {
    cout << "\n--- CREATE STOCK REQUEST ---" << endl;
    if (requestQueue.isFull()) {
        cout << "  Queue is FULL (overflow). Process a request first." << endl;
        return;
    }
    string id = readId("  Medicine ID: ");
    if (hashTable.search(id) == nullptr) { cout << "  Medicine not found." << endl; return; }
    StockRequest r;
    r.requestID = requestQueue.makeRequestId();
    r.medicineID = id;
    r.requestedQuantity = readInt("  Quantity to request: ", 1);
    r.requestDate = todayDate();
    r.status = "Pending";
    requestQueue.enqueue(r);
    cout << "  Request " << r.requestID << " created." << endl;
}

// 12. Process Stock Request (DEQUEUE - oldest first)
void processRequest() {
    cout << "\n--- PROCESS STOCK REQUEST ---" << endl;
    StockRequest r;
    if (!requestQueue.dequeue(r)) {
        cout << "  No pending requests (underflow)." << endl;
        return;
    }
    cout << "  Processing oldest request: " << r.requestID << endl;
    if (hashTable.search(r.medicineID) == nullptr) {
        cout << "  Medicine " << r.medicineID << " no longer exists. Request discarded." << endl;
        return;
    }
    inventory.stockIn(r.medicineID, r.requestedQuantity);
    r.status = "Completed";
    cout << "  Added " << r.requestedQuantity << " units to " << r.medicineID
         << ". New quantity: " << hashTable.search(r.medicineID)->quantity << endl;
}

// 13. View Pending Requests
void viewRequests() {
    cout << "\n--- PENDING REQUESTS (oldest first) ---" << endl;
    requestQueue.display();
}

// 14. Low Stock Medicines
void lowStock() {
    cout << "\n--- LOW STOCK MEDICINES ---" << endl;
    inventory.displayLowStock();
}

// 15. Generate Inventory Report (writes a text file)
void generateReport() {
    ofstream file(REPORT_FILE);
    string today = todayDate();
    double value = 0;
    Node* current = inventory.getHead();
    while (current != nullptr) {
        value = value + current->data.quantity * current->data.price;
        current = current->next;
    }

    file << "INVENTORY REPORT - " << today << endl;
    file << "====================================" << endl;
    file << "Total medicines  : " << inventory.countMedicines() << endl;
    file << "Total quantity   : " << inventory.totalQuantity() << endl;
    file << "Inventory value  : " << value << endl;
    file << "Pending requests : " << requestQueue.size() << endl << endl;

    file << "LOW STOCK:" << endl;
    current = inventory.getHead();
    while (current != nullptr) {
        if (current->data.quantity <= current->data.minStockLevel) {
            file << "  " << current->data.medicineID << " " << current->data.name
                 << " (qty " << current->data.quantity << ")" << endl;
        }
        current = current->next;
    }
    file << endl << "EXPIRED:" << endl;
    current = inventory.getHead();
    while (current != nullptr) {
        if (current->data.expiryDate < today) {
            file << "  " << current->data.medicineID << " " << current->data.name
                 << " (expired " << current->data.expiryDate << ")" << endl;
        }
        current = current->next;
    }
    cout << "\n  Report saved to " << REPORT_FILE << endl;
}

// Bonus for the viva: shows buckets and collisions
void showHashTable() {
    cout << "\n--- HASH TABLE INTERNALS ---" << endl;
    hashTable.displayTable();
}

// =====================================================================
//  MENU
// =====================================================================
void showMenu() {
    cout << "\n================================================" << endl;
    cout << "       MEDICAL INVENTORY MANAGEMENT SYSTEM" << endl;
    cout << "================================================" << endl;
    cout << "  MEDICINES" << endl;
    cout << "   1. Add New Medicine" << endl;
    cout << "   2. View All Medicines" << endl;
    cout << "   3. Search Medicine by ID" << endl;
    cout << "   4. Update Medicine Details" << endl;
    cout << "   5. Delete Medicine" << endl;
    cout << "  EXPIRY" << endl;
    cout << "   6. Manage Expiry Dates" << endl;
    cout << "   7. View Expired Medicines" << endl;
    cout << "   8. View Medicines Expiring Soon" << endl;
    cout << "  STOCK" << endl;
    cout << "   9. Stock-In" << endl;
    cout << "  10. Stock-Out" << endl;
    cout << "  11. Create Stock Request" << endl;
    cout << "  12. Process Stock Request" << endl;
    cout << "  13. View Pending Requests" << endl;
    cout << "  REPORTS & DATA" << endl;
    cout << "  14. Low Stock Medicines" << endl;
    cout << "  15. Generate Inventory Report" << endl;
    cout << "  16. Save Data" << endl;
    cout << "  17. Exit" << endl;
    cout << "  18. (Demo) Show Hash Table Buckets" << endl;
    cout << "================================================" << endl;
}

int main() {
    cout << "Starting... today is " << todayDate() << endl;
    loadMedicines();      // rebuilds Linked List + Hash Table + BST
    loadRequests();       // rebuilds Queue

    int choice = 0;
    while (choice != 17) {
        showMenu();
        choice = readInt("  Enter your choice: ", 1);

        if (choice == 1) addNewMedicine();
        else if (choice == 2) viewAllMedicines();
        else if (choice == 3) searchMedicine();
        else if (choice == 4) updateMedicine();
        else if (choice == 5) deleteMedicine();
        else if (choice == 6) manageExpiry();
        else if (choice == 7) viewExpired();
        else if (choice == 8) viewExpiringSoon();
        else if (choice == 9) stockIn();
        else if (choice == 10) stockOut();
        else if (choice == 11) createRequest();
        else if (choice == 12) processRequest();
        else if (choice == 13) viewRequests();
        else if (choice == 14) lowStock();
        else if (choice == 15) generateReport();
        else if (choice == 16) saveAll();
        else if (choice == 17) { saveAll(); cout << "  Goodbye!" << endl; }
        else if (choice == 18) showHashTable();
        else cout << "  Invalid choice. Enter 1 to 18." << endl;

        if (choice != 17) pause();
    }
    return 0;
}
