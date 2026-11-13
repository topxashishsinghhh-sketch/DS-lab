# Medical Inventory Management System (DS-PBL)

A clean C++17 Project-Based Learning (**DS-PBL**) implementation featuring both a **Console Interface** and a **Web GUI Dashboard** (`gui.html`).

---

## 4 Core Data Structures Used

1. **Singly Linked List** (`modules/InventoryList.h / .cpp`)
   - **Role**: Primary master storage for medicine records.
   - **Operations**: Dynamic heap node creation, tail append, deletion, linear search.
2. **Hash Table with Chaining** (`modules/HashTable.h / .cpp`)
   - **Role**: $O(1)$ fast lookup by Medicine ID.
   - **Operations**: Modulo hash function (`ID % 10`), separate chaining for collision resolution.
3. **Binary Search Tree** (`modules/ExpiryBST.h / .cpp`)
   - **Role**: Chronological tracking by medicine expiry date.
   - **Operations**: In-order traversal (Left $\rightarrow$ Root $\rightarrow$ Right) for sorted chronological listing.
4. **Circular Array Queue** (`modules/StockQueue.h / .cpp`)
   - **Role**: First-In, First-Out (FIFO) processing of restock orders.
   - **Operations**: Modulo wrap-around `(index + 1) % 10`, $O(1)$ enqueue and dequeue.

---

## How to Build & Run

### 1. One-Click Run (Windows)
Double-click:
```cmd
build_and_run.bat
```

### 2. Manual Terminal Build
```bash
g++ -std=c++17 main.cpp modules/*.cpp -o inventory.exe
./inventory.exe
```

### 3. Web Dashboard (GUI)
Open `gui.html` in any web browser (Chrome, Edge, Firefox), or press **`Ctrl + Alt + M`** on Windows.

---

## Project Structure
```
MedicalInventorySystem/
├── build_and_run.bat        # Windows build and run script
├── gui.html                 # Web dashboard interface
├── main.cpp                 # Main program loop and synchronization
├── README.md                # Project documentation
├── common/
│   ├── DateUtil.h           # Date comparison helper
│   ├── Medicine.h           # Medicine record struct
│   └── StockRequest.h       # Stock order request struct
└── modules/
    ├── ExpiryBST.h / .cpp   # Binary Search Tree (Expiry dates)
    ├── HashTable.h / .cpp   # Hash Table with Chaining (O(1) search)
    ├── InventoryList.h/.cpp # Singly Linked List (Master records)
    └── StockQueue.h / .cpp  # Circular Array Queue (FIFO orders)
```
