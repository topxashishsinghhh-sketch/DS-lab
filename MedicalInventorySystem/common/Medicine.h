#ifndef MEDICINE_H
#define MEDICINE_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// ============================================================================
// MEDICINE STRUCTURE
// Holds basic details of one medicine.
// Easy to explain: "Just like a record card for each medicine in the store."
// ============================================================================
struct Medicine {
    int id;            // Medicine ID (e.g. 101, 102)
    string name;       // Medicine name (e.g. "Paracetamol")
    string category;   // Category (e.g. "Tablet", "Syrup")
    int quantity;      // Stock count
    double price;      // Price per unit
    string expiry;     // Expiry date (e.g. "2027-06")
};

// Simple helper to print one medicine row
inline void printMedicine(const Medicine& m) {
    cout << "  | ID: " << setw(4) << m.id 
         << " | Name: " << setw(14) << left << m.name 
         << " | Cat: " << setw(10) << m.category 
         << " | Qty: " << setw(4) << right << m.quantity 
         << " | Price: $" << setw(5) << fixed << setprecision(2) << m.price 
         << " | Expiry: " << m.expiry << " |\n";
}

#endif
