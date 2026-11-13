# 👥 DS-PBL Team Division & Preparation Guide
### Medical Inventory Management System — 4 Members, 4 Data Structures

This guide assigns each team member their exact **Header (`.h`)** and **Implementation (`.cpp`)** files, what their functions do, and what to say when the teacher questions them.

---

## 📋 Overview Table of Responsibilities

| Member | Assigned Data Structure | Header File | Implementation File | Key Functions Owned |
|:---:|:---|:---|:---|:---|
| **Member 1** | **Singly Linked List** | `modules/InventoryList.h` | `modules/InventoryList.cpp` | `addMedicine`, `deleteMedicine`, `searchMedicine`, `display` |
| **Member 2** | **Hash Table (Chaining)** | `modules/HashTable.h` | `modules/HashTable.cpp` | `hashFunction`, `insert`, `search`, `displayTable` |
| **Member 3** | **Binary Search Tree** | `modules/ExpiryBST.h` | `modules/ExpiryBST.cpp` | `insertHelper`, `inorderHelper`, `displaySorted` |
| **Member 4** | **Circular Array Queue** | `modules/StockQueue.h` | `modules/StockQueue.cpp` | `enqueue`, `dequeue`, `display`, modulo wrap-around |

*All members also share `common/Medicine.h` (the record structure).*

---

## 👤 Member 1: Singly Linked List

### 📁 Assigned Files:
* `modules/InventoryList.h`
* `modules/InventoryList.cpp`

### 🎯 Your Role:
You own the **Master Storage**. All medicines in the hospital are stored in your Linked List on the heap.

### 🔑 Key Code to Explain to the Teacher:
```cpp
struct Node {
    Medicine data; // Payload
    Node* next;    // Pointer to next node
};

// 1. Insert at end (Append)
void InventoryList::addMedicine(const Medicine& m) {
    Node* newNode = new Node{m, nullptr};
    if (head == nullptr) { head = newNode; return; }
    Node* temp = head;
    while (temp->next != nullptr) temp = temp->next;
    temp->next = newNode;
}

// 2. Delete by ID
bool InventoryList::deleteMedicine(int id) {
    if (!head) return false;
    if (head->data.id == id) {
        Node* t = head; head = head->next; delete t; return true;
    }
    Node* prev = head;
    while (prev->next && prev->next->data.id != id) prev = prev->next;
    if (prev->next) {
        Node* target = prev->next;
        prev->next = target->next;
        delete target;
        return true;
    }
    return false;
}
```

### 🗣️ What to Say in the Viva:
* *"I implemented the Singly Linked List. It stores the master medicine records."*
* *"We chose a Linked List because medicines are added and deleted dynamically. An array would have a fixed size and overflow, whereas a Linked List allocates nodes on the heap as needed."*
* **Time Complexity**: Insertion is $O(n)$, Search is $O(n)$, Deletion is $O(n)$.

---

## 👤 Member 2: Hash Table with Chaining

### 📁 Assigned Files:
* `modules/HashTable.h`
* `modules/HashTable.cpp`

### 🎯 Your Role:
You own the **High-Speed Search Index**. When a pharmacist searches for a medicine by ID, you find it in instant $O(1)$ time instead of scanning the whole list.

### 🔑 Key Code to Explain to the Teacher:
```cpp
// 1. Hash Function (Modulo Hashing)
int HashTable::hashFunction(int id) const {
    return id % 10; // Table size = 10
}

// 2. Insert with Chaining (Separate Chaining)
void HashTable::insert(int id, Medicine* record) {
    int index = hashFunction(id);
    HashNode* newNode = new HashNode{id, record, table[index]};
    table[index] = newNode; // Insert at head of bucket chain
}

// 3. Search - O(1) average
Medicine* HashTable::search(int id) {
    int index = hashFunction(id);
    HashNode* curr = table[index];
    while (curr) {
        if (curr->id == id) return curr->record;
        curr = curr->next;
    }
    return nullptr;
}
```

### 🗣️ What to Say in the Viva:
* *"I implemented the Hash Table using Separate Chaining. It provides $O(1)$ instant lookup by Medicine ID."*
* *"Our hash function is `id % 10`. If two IDs produce the same bucket (like ID 101 and ID 111 both giving 1), they collide. I resolve this collision using a linked list chain in Bucket 1."*
* **Teacher Demo**: Run Option 9 in the console to show `Bucket [1]: [111: Ibuprofen] -> [101: Paracetamol] -> NULL`.

---

## 👤 Member 3: Binary Search Tree (BST)

### 📁 Assigned Files:
* `modules/ExpiryBST.h`
* `modules/ExpiryBST.cpp`

### 🎯 Your Role:
You own the **Expiry Date Monitoring System**. You organize medicines chronologically so doctors never prescribe expired drugs.

### 🔑 Key Code to Explain to the Teacher:
```cpp
// 1. Recursive Insert (BST Property: Left < Root <= Right)
BSTNode* ExpiryBST::insertHelper(BSTNode* node, const string& expiry, int id, const string& name) {
    if (node == nullptr) return new BSTNode{expiry, id, name, nullptr, nullptr};
    if (expiry < node->expiry)
        node->left = insertHelper(node->left, expiry, id, name);
    else
        node->right = insertHelper(node->right, expiry, id, name);
    return node;
}

// 2. In-Order Traversal (Sorted output)
void ExpiryBST::inorderHelper(BSTNode* node) const {
    if (node == nullptr) return;
    inorderHelper(node->left);                          // Left
    cout << node->expiry << " - " << node->medName;     // Root
    inorderHelper(node->right);                         // Right
}
```

### 🗣️ What to Say in the Viva:
* *"I implemented the Binary Search Tree for tracking medicine expiry dates."*
* *"Dates are formatted as YYYY-MM, so string comparison `<` matches chronological order."*
* *"Because Left < Root $\le$ Right, visiting the tree **In-Order** (Left $\rightarrow$ Root $\rightarrow$ Right) automatically outputs medicines in sorted chronological order!"*
* **Time Complexity**: Search/Insert is $O(\log n)$ average, In-order display is $O(n)$.

---

## 👤 Member 4: Circular Array Queue

### 📁 Assigned Files:
* `modules/StockQueue.h`
* `modules/StockQueue.cpp`

### 🎯 Your Role:
You own the **Restocking Order Queue**. When medicine stock runs low, orders are processed in **FIFO (First-In, First-Out)** order.

### 🔑 Key Code to Explain to the Teacher:
```cpp
// 1. Enqueue (Add to rear with circular wrap-around)
bool StockQueue::enqueue(int medId, const string& medName, int qty) {
    if (isFull()) return false; // Overflow check
    rear = (rear + 1) % MAX_QUEUE;
    items[rear] = {nextId++, medId, medName, qty};
    count++;
    return true;
}

// 2. Dequeue (Serve oldest from front)
bool StockQueue::dequeue(StockRequest& result) {
    if (isEmpty()) return false; // Underflow check
    result = items[front];
    front = (front + 1) % MAX_QUEUE;
    count--;
    return true;
}
```

### 🗣️ What to Say in the Viva:
* *"I implemented the Circular Array Queue for restocking orders on a FIFO basis."*
* *"In a simple array queue, removing elements leaves empty slots at the front that are wasted. By using the modulo operator `(index + 1) % MAX`, my queue wraps around and reuses freed slots."*
* **Time Complexity**: Enqueue is $O(1)$, Dequeue is $O(1)$. No shifting of array elements is needed.

---

## 🤝 Project Leader / Coordinator (Connecting It All)

If the teacher asks how all 4 members' work connects together:
> *"The Singly Linked List (Member 1) holds the master records. When a medicine is added, Member 2's Hash Table stores a pointer to it for $O(1)$ ID search, and Member 3's BST indexes its expiry date for chronological audits. When stock runs low, Member 4's Circular Queue handles restocking in fair FIFO order. All 4 structures stay synchronized in `main.cpp` and export live to our GUI dashboard."*
