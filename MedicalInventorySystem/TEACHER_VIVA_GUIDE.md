# 🎓 Teacher Viva & Project Defense Guide
## Medical Inventory Management System — Data Structures Lab

This document contains everything you need to confidently explain every file, line of code, data structure choice, and time complexity to your teacher.

---

## 1. High-Level System Architecture

### The Core Architectural Principle
> **"The Singly Linked List OWNS the master medicine records. The Hash Table and Binary Search Tree are INDEXES."**

| Data Structure | Implementation File | Role in the System | Time Complexity |
|---|---|---|---|
| **Singly Linked List** | `modules/InventoryList.cpp` | Stores master records on the heap | Append: $O(n)$, Search: $O(n)$ |
| **Hash Table (Chaining)** | `modules/HashTable.cpp` | High-speed index by Medicine ID | Search/Insert: $O(1)$ average |
| **Binary Search Tree** | `modules/ExpiryBST.cpp` | Chronological index by Expiry Date | Search/Insert: $O(\log n)$ average |
| **Circular Array Queue** | `modules/StockQueue.cpp` | FIFO supplier restocking queue | Enqueue/Dequeue: $O(1)$ |

---

## 2. Line-by-Line Breakdown by Data Structure

### 🔗 Module 1: Singly Linked List (`modules/InventoryList.h` & `modules/InventoryList.cpp`)

#### What to tell your teacher:
> *"Sir/Ma'am, we chose a Singly Linked List because medicine inventory changes dynamically. Unlike a static array, a linked list does not waste memory or require costly reallocation when new medicines arrive."*

#### Key Code Snippet:
```cpp
struct Node {
    Medicine data;   // Real Medicine record (name, price, stock, etc.)
    Node* next;      // Heap pointer to the next node in memory
};
```

#### How Insertion Works (`addMedicine`):
1. `Node* newNode = new Node;` allocates heap memory for the new element.
2. `newNode->data = m;` copies the payload.
3. `newNode->next = nullptr;` sets the tail pointer.
4. If `head == nullptr`, `head = newNode;`.
5. Otherwise, `while (current->next != nullptr)` walks to the end, then `current->next = newNode;`.

#### How Deletion Works (`deleteMedicine`):
- **Case 1 (Deleting head)**: `head = current->next; delete current;`
- **Case 2 (Deleting middle or tail)**: `previous->next = current->next; delete current;`
- **Why `delete current`?**: Frees dynamic heap memory and prevents memory leaks.

---

### ⚡ Module 2: Hash Table with Chaining (`modules/HashTable.h` & `modules/HashTable.cpp`)

#### What to tell your teacher:
> *"When a dispensary pharmacist searches for a medicine by ID, searching a linked list takes $O(n)$ time. By hashing the ID string into an array index, our Hash Table provides instant $O(1)$ average lookup."*

#### Key Code Snippet:
```cpp
int HashTable::hashFunction(const string& id) const {
    int sum = 0;
    for (char ch : id) {
        sum += static_cast<unsigned char>(ch);
    }
    return sum % TABLE_SIZE; // TABLE_SIZE = 11 (prime number)
}
```

#### How Collisions are Solved (Separate Chaining):
- Each bucket in `table[11]` holds a linked list (`HashNode*`).
- If two different medicine IDs yield the same bucket index, they are chained:
  ```cpp
  newNode->next = table[index]; // Insert at head of bucket chain
  table[index] = newNode;
  ```
- **Demo Proof for Teacher**:
  - `MED101` ASCII sum: `77 + 69 + 68 + 49 + 48 + 49 = 360`. `360 % 11 = 8`.
  - `MED110` ASCII sum: `77 + 69 + 68 + 49 + 49 + 48 = 360`. `360 % 11 = 8`.
  - In Option 18, show your teacher **Bucket 8**: `[MED110] -> [MED101] -> NULL`. They collide and chain together!

---

### 🌲 Module 3: Binary Search Tree (`modules/ExpiryBST.h` & `modules/ExpiryBST.cpp`)

#### What to tell your teacher:
> *"Medicines must be monitored by expiry date so patients are never given expired drugs. In our BST, dates are stored in ISO format (YYYY-MM-DD). Because dates are formatted this way, standard string comparison `<` and `>` accurately compares chronology."*

#### Key Properties of the BST:
1. **Left Subtree**: Earlier dates (`date < node->expiryDate`).
2. **Right Subtree**: Later dates (`date > node->expiryDate`).
3. **Multi-Value Mapping**: Since multiple medicines can expire on the same date, each `BSTNode` has `vector<string> ids`.

#### Why In-Order Traversal is Special:
```cpp
void ExpiryBST::inorderPrint(BSTNode* node, HashTable& ht) {
    if (node == nullptr) return;
    inorderPrint(node->left, ht);  // 1. Visit earlier dates
    printNode(node, ht);          // 2. Visit current date
    inorderPrint(node->right, ht); // 3. Visit later dates
}
```
> *"In-order traversal visits nodes in strictly ASCENDING chronological order, allowing us to generate an audit report sorted from earliest to latest expiry date in $O(n)$ time!"*

---

### 🔄 Module 4: Circular Array Queue (`modules/StockQueue.h` & `modules/StockQueue.cpp`)

#### What to tell your teacher:
> *"Dispensing and restocking requests must be served in the order they arrived (FIFO: First-In, First-Out). We used a Circular Queue with modulo arithmetic rather than a simple array to prevent memory drift."*

#### Key Code Snippet:
```cpp
// Enqueue (Insert at rear)
rearIndex = (rearIndex + 1) % MAX_REQUESTS;
items[rearIndex] = r;

// Dequeue (Remove from front)
out = items[frontIndex];
frontIndex = (frontIndex + 1) % MAX_REQUESTS;
```

#### Why Modulo `(rear + 1) % MAX_REQUESTS`?
- In a plain array queue, after dequeuing 5 items, slots 0 through 4 are empty and wasted unless shifted ($O(n)$).
- Modulo arithmetic wraps around to slot 0 when the end of the array is reached, keeping both `enqueue` and `dequeue` strictly **$O(1)$** without shifting elements.

---

## 3. Top 5 Questions Teachers Ask in Viva & Model Answers

### Q1: What happens when you delete a medicine?
**Answer**:
> *"We maintain full consistency across all data structures in `deleteMedicine()`:
> 1. We remove the ID from the Expiry BST. If no other medicine expires on that date, the BST node is deleted using standard BST deletion.
> 2. We remove the ID node from the Hash Table bucket chain.
> 3. We unbind the node from the Singly Linked List and call `delete current` to return memory to the OS.
> 4. We persist changes to `storage/medicines.txt`."*

### Q2: What is the load factor of your Hash Table?
**Answer**:
> *"Load factor $\alpha = \frac{n}{k}$, where $n$ is the number of keys and $k$ is the number of buckets (11). With 6 sample medicines, $\alpha = \frac{6}{11} \approx 0.54$. Because $\alpha < 0.75$, the probability of long collision chains is low, maintaining average $O(1)$ lookup time."*

### Q3: Why didn't you use C++ STL containers (`std::map`, `std::queue`)?
**Answer**:
> *"Because this is a fundamental Data Structures project. Implementing the Node pointers, hash functions, tree recursions, and modulo queue pointers manually from scratch demonstrates a true conceptual understanding of heap memory and algorithms."*

### Q4: How is data saved across program restarts?
**Answer**:
> *"In the `storage/` directory, `medicines.txt` stores pipe-delimited records:
> `ID|Name|Category|Manufacturer|Qty|Price|MfgDate|ExpiryDate|MinStock`
> On startup, `loadMedicines()` parses each row and calls `addToAllStructures()` to repopulate the Linked List, Hash Table, and BST simultaneously."*

### Q5: How can you show me the visual working of these structures?
**Answer**:
> *"Option 19 in our console application has an interactive Data Structure Visual Inspector showing node links, hash chains, and tree levels. In addition, we have built `gui.html`, which can be opened in any web browser to see the animated structures live."*
