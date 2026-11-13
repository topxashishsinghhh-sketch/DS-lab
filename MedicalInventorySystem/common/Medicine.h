// ============================================================================
// common/Medicine.h
// ============================================================================
// CONCEPT: Base Data Entity (Record)
// This structure defines what a "Medicine" is in our system.
// Crucial Architecture Note for Viva:
//   - The Singly Linked List (InventoryList) OWNS the actual Medicine objects.
//   - The Hash Table stores pointers (Medicine*) pointing directly into the list.
//   - The BST stores medicine IDs mapped to expiry dates.
// ============================================================================

#ifndef MEDICINE_H
#define MEDICINE_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct Medicine {
    string medicineID;        // Unique key, e.g., "MED101"
    string name;              // Medicine brand/generic name
    string category;          // e.g., "Tablet", "Syrup", "Capsule", "Injection"
    string manufacturer;      // Company name, e.g., "ABC Pharma"
    int    quantity;          // Current units available in stock
    double price;             // Price per unit in currency
    string manufacturingDate; // ISO format: YYYY-MM-DD
    string expiryDate;        // ISO format: YYYY-MM-DD (used as key in BST)
    int    minStockLevel;     // Alert threshold for low-stock warning
};

// Prints a single medicine in a clean, bordered card format for the UI
inline void printMedicineCard(const Medicine& m) {
    cout << "  +-------------------------------------------------------------+\n";
    cout << "  |                     MEDICINE DETAILS                        |\n";
    cout << "  +-------------------------------------------------------------+\n";
    cout << "  | ID           : " << left << setw(44) << m.medicineID << "|\n";
    cout << "  | Name         : " << left << setw(44) << m.name << "|\n";
    cout << "  | Category     : " << left << setw(44) << m.category << "|\n";
    cout << "  | Manufacturer : " << left << setw(44) << m.manufacturer << "|\n";
    cout << "  | Quantity     : " << left << setw(44) << to_string(m.quantity) + " units" << "|\n";
    cout << "  | Unit Price   : $" << left << fixed << setprecision(2) << setw(43) << m.price << "|\n";
    cout << "  | Mfg Date     : " << left << setw(44) << m.manufacturingDate << "|\n";
    cout << "  | Expiry Date  : " << left << setw(44) << m.expiryDate << "|\n";
    cout << "  | Min Stock    : " << left << setw(44) << to_string(m.minStockLevel) + " units" << "|\n";
    cout << "  +-------------------------------------------------------------+\n";
}

// Prints one row in a tabular list view
inline void printMedicineRow(const Medicine& m) {
    cout << "  | " << left << setw(8)  << m.medicineID
         << "| " << left << setw(18) << (m.name.length() > 17 ? m.name.substr(0, 14) + "..." : m.name)
         << "| " << left << setw(12) << m.category
         << "| " << right << setw(6) << m.quantity
         << " | $" << right << fixed << setprecision(2) << setw(7) << m.price
         << " | " << left << setw(10) << m.expiryDate
         << " | " << right << setw(5) << m.minStockLevel << " |\n";
}

#endif // MEDICINE_H
