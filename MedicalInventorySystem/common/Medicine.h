// Medicine.h - SHARED by all 4 members. Nobody makes their own version.
#ifndef MEDICINE_H
#define MEDICINE_H

#include <iostream>
#include <string>
using namespace std;

struct Medicine {
    string medicineID;
    string name;
    string category;
    string manufacturer;
    int quantity;
    double price;
    string manufacturingDate;   // format: YYYY-MM-DD
    string expiryDate;          // format: YYYY-MM-DD
    int minStockLevel;
};

// Prints one medicine in "card" format (used by Search and View)
inline void printMedicineDetails(const Medicine& m) {
    cout << "  Medicine ID        : " << m.medicineID << endl;
    cout << "  Medicine Name      : " << m.name << endl;
    cout << "  Category           : " << m.category << endl;
    cout << "  Manufacturer       : " << m.manufacturer << endl;
    cout << "  Quantity           : " << m.quantity << endl;
    cout << "  Price              : " << m.price << endl;
    cout << "  Manufacturing Date : " << m.manufacturingDate << endl;
    cout << "  Expiry Date        : " << m.expiryDate << endl;
    cout << "  Minimum Stock      : " << m.minStockLevel << endl;
}

#endif
