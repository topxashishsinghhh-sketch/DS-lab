# Medical Inventory Management System — Data Structures Lab

A complete, production-grade **Data Structures Lab Final Project** in C++17 featuring both a **Console Interface** (with live ASCII Data Structure Inspectors) and an **Interactive Web Dashboard GUI** (`gui.html`).

---

## 🏗️ 4 Core Data Structures Implemented From Scratch

1. **Singly Linked List** (`modules/InventoryList.h / .cpp`)
   - **Role**: Primary Master Storage for all medicine records.
   - **Properties**: Dynamic heap nodes (`Node*`), tail append, linear search, node unlinking.
2. **Hash Table with Chaining** (`modules/HashTable.h / .cpp`)
   - **Role**: Instant $O(1)$ medicine lookup by ID.
   - **Properties**: Custom ASCII modulo hash function, 11 buckets, separate chaining for collision resolution (e.g. `MED101` and `MED110` chain in Bucket 8).
3. **Binary Search Tree** (`modules/ExpiryBST.h / .cpp`)
   - **Role**: Chronological index of medicine expiry dates.
   - **Properties**: Left child < Root < Right child, in-order traversal for chronological sorting, recursive search & node deletion.
4. **Circular Array Queue** (`modules/StockQueue.h / .cpp`)
   - **Role**: First-In, First-Out (FIFO) processing of restocking requests.
   - **Properties**: Array buffer with modulo wrap-around `(rear + 1) % MAX_REQUESTS`, zero memory drift, $O(1)$ enqueue and dequeue.

---

## 🚀 How to Build & Run

### Method 1: One-Click Batch File (Windows)
Double-click:
```cmd
build_and_run.bat
```
This automatically compiles `inventory.exe` and launches both the console app and the browser GUI.

### Method 2: Manual Terminal Build
```bash
g++ -std=c++17 main.cpp modules/*.cpp -o inventory.exe
./inventory.exe
```

---

## 🎨 Interactive Visual Web UI
Open `gui.html` in any browser (Google Chrome, Edge, Firefox) by double-clicking it.
- **No dependencies required** (pure HTML5 / CSS3 / JavaScript).
- **Interactive animated diagrams** for the Linked List, Hash Table, Circular Queue, and Expiry BST.
- **Teacher Viva Guide Tab** with answers to top exam/defense questions.

---

## 📂 Project Organization
```
MedicalInventorySystem/
├── build_and_run.bat        # Windows build and run launcher
├── gui.html                 # Companion Interactive Visual GUI
├── main.cpp                 # Main controller and synchronization logic
├── TEACHER_VIVA_GUIDE.md    # Detailed viva defense questions & answers
├── README.md                # Project documentation
├── common/
│   ├── DateUtil.h           # Date conversion & validation helpers
│   ├── Medicine.h           # Master Medicine struct definition
│   └── StockRequest.h       # Restock order request definition
├── modules/
│   ├── ExpiryBST.h / .cpp   # Binary Search Tree (Expiry dates)
│   ├── HashTable.h / .cpp   # Hash Table with Chaining (O(1) ID lookup)
│   ├── InventoryList.h/.cpp # Singly Linked List (Master records)
│   └── StockQueue.h / .cpp  # Circular Array Queue (FIFO orders)
└── storage/
    ├── medicines.txt        # Persisted inventory records
    ├── requests.txt         # Persisted pending requests
    └── inventory_report.txt # Exported audit report
```
