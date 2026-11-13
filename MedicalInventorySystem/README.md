# Medical Inventory Management System (C++17, Console)

## Build and run
    g++ -std=c++17 main.cpp modules/*.cpp -o inventory
    ./inventory          (Windows: inventory.exe)
Run it from the project folder so it can find the `storage/` folder.

## Who owns what
| Member | Files | Data structure | Functions to explain |
|---|---|---|---|
| 1 | modules/HashTable.h/.cpp | Hash Table + chaining | hashFunction, insert, search, update, remove, displayTable |
| 2 | modules/ExpiryBST.h/.cpp | Binary Search Tree | insertNode, searchNode, inorderPrint, expiredPrint, expiringPrint, remove |
| 3 | modules/StockQueue.h/.cpp | Circular array Queue | enqueue, dequeue, front, isFull, isEmpty, display |
| 4 | modules/InventoryList.h/.cpp | Singly Linked List | addMedicine, deleteMedicine, findMedicine, updateMedicine, stockIn/Out |
| All | common/*.h, main.cpp | Shared structs, menu, files | addToAllStructures, load/save |

## The one idea that ties it together
The Linked List OWNS the medicine records. The Hash Table and BST are only INDEXES:
- Hash Table: ID -> pointer to the record in the list
- BST: expiry date -> list of IDs
See `addToAllStructures()` and `deleteMedicine()` in main.cpp.

## Demo script for the teacher
1. Option 3, search `MED101` -> Hash Table lookup.
2. Option 18 -> show buckets; MED110 and MED101 share Bucket 8 (collision, chaining).
3. Option 6 -> inorder traversal prints dates in ascending order.
4. Option 8, enter 30 -> medicines expiring in 30 days.
5. Option 13 then 12 -> oldest request (REQ001) is processed first (FIFO).
6. Option 1 then 2 -> add a medicine, see it in the list.
7. Option 17 -> exit, reopen, data is still there (file handling).

## Data files (storage/)
medicines.txt: ID|name|category|manufacturer|qty|price|mfgDate|expiryDate|minStock
requests.txt : first line = next request number, then ID|medicineID|qty|date|status
Sample expiry dates are set around November 2026; edit medicines.txt to get expired/expiring examples for your demo day.

## Complexity
Hash Table search/insert: avg O(1), worst O(n) | BST search/insert: avg O(log n), worst O(n)
Queue enqueue/dequeue: O(1) | Linked List add (tail) / search / delete: O(n)

## Design choices (be ready to explain)
- Linked list adds at the tail so the file order stays stable after each save/load.
- BST is a normal (unbalanced) tree; deleting a medicine removes its ID from the node's list but keeps the node.
- Queue is circular so freed slots are reused (the % MAX_REQUESTS trick).
- Dates are YYYY-MM-DD strings, so `date1 < date2` compares correctly.
