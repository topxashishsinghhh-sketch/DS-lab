# 🎓 Super Easy Viva Guide: Medical Inventory System
## How to Explain Every Data Structure in 30 Seconds

---

## 🌟 The Big Picture (Say this first!)
> *"Teacher, our project uses **4 core data structures** to manage a hospital medical store:
> 1. **Singly Linked List** stores all medicines dynamically.
> 2. **Hash Table** finds any medicine in $O(1)$ instant time using `ID % 10`.
> 3. **Binary Search Tree** sorts medicines chronologically by expiry date.
> 4. **Circular Queue** processes supplier restocking orders in FIFO (First-In, First-Out) order."*

---

## 1. Singly Linked List (`modules/InventoryList.cpp`)

### 🗣️ How to explain it:
*"A Linked List is a chain of nodes in memory. Each node has two parts: the **Medicine data** and a **next pointer** pointing to the following node."*

### ✍️ How to write it on a whiteboard (Only 7 lines!):
```cpp
struct Node {
    Medicine data;
    Node* next;
};

void addMedicine(Medicine m) {
    Node* newNode = new Node{m, nullptr};
    if (head == nullptr) { head = newNode; return; }
    Node* temp = head;
    while (temp->next != nullptr) temp = temp->next;
    temp->next = newNode;
}
```

### 🎯 Key Viva Question: *Why not just use an array?*
**Answer**: *"An array has a fixed size. If the hospital gets new medicines, an array overflows. A linked list grows and shrinks dynamically in heap memory without wasting space."*

---

## 2. Hash Table (`modules/HashTable.cpp`)

### 🗣️ How to explain it:
*"Searching a list takes $O(n)$ time. Our Hash Table takes the Medicine ID and computes `index = ID % 10`. This lets us jump directly to the exact bucket in instant $O(1)$ time!"*

### ✍️ How to write it on a whiteboard (Only 6 lines!):
```cpp
int hashFunction(int id) { 
    return id % 10; 
}

void insert(int id, Medicine* record) {
    int index = id % 10;
    HashNode* newNode = new HashNode{id, record, table[index]};
    table[index] = newNode; // Insert at front of bucket chain
}
```

### 💥 Viva Demo (Show this in Option 9!):
- Medicine **101** % 10 = **Bucket 1**
- Medicine **111** % 10 = **Bucket 1**
- Both go to Bucket 1! This is a **Collision**.
- We solve it by **Separate Chaining**: Bucket 1 holds a linked list:
  `[111: Ibuprofen] -> [101: Paracetamol] -> NULL`.

---

## 3. Binary Search Tree (`modules/ExpiryBST.cpp`)

### 🗣️ How to explain it:
*"A BST organizes medicines by expiry date. Smaller/earlier dates go to the **Left**, and larger/later dates go to the **Right**."*

### ✍️ How to write it on a whiteboard (Only 6 lines!):
```cpp
BSTNode* insert(BSTNode* node, string expiry, int id, string name) {
    if (node == nullptr) return new BSTNode{expiry, id, name, nullptr, nullptr};
    if (expiry < node->expiry) node->left = insert(node->left, expiry, id, name);
    else node->right = insert(node->right, expiry, id, name);
    return node;
}
```

### 🎯 Key Viva Question: *Why In-Order Traversal?*
```cpp
void inorder(BSTNode* node) {
    if (!node) return;
    inorder(node->left);                          // 1. Earlier dates
    cout << node->expiry << " " << node->medName; // 2. Current date
    inorder(node->right);                         // 3. Later dates
}
```
**Answer**: *"Because Left < Root < Right, visiting In-Order (Left $\rightarrow$ Root $\rightarrow$ Right) automatically prints all medicines in sorted chronological order!"*

---

## 4. Circular Queue (`modules/StockQueue.cpp`)

### 🗣️ How to explain it:
*"Restock orders must be served in the order they were placed: **FIFO (First-In, First-Out)**. We use a circular array with `% 10` so when the end is reached, it wraps around to slot 0 and reuses freed slots."*

### ✍️ How to write it on a whiteboard (Only 6 lines!):
```cpp
// Enqueue (Add to rear)
rear = (rear + 1) % 10;
items[rear] = newOrder;
count++;

// Dequeue (Remove oldest from front)
Order served = items[front];
front = (front + 1) % 10;
count--;
```

### 🎯 Key Viva Question: *Why Circular instead of a normal array?*
**Answer**: *"In a regular array queue, when you remove items, the front slots are permanently wasted. A circular queue wraps around using modulo `(index + 1) % MAX`, reusing slots in $O(1)$ time without shifting elements!"*

---

## 5. Summary Table for Viva

| Data Structure | Code File | Purpose | Time Complexity |
|---|---|---|---|
| **Singly Linked List** | `InventoryList.cpp` | Master inventory storage | Insert: $O(n)$, Search: $O(n)$ |
| **Hash Table** | `HashTable.cpp` | Search by ID via `id % 10` | Search: $O(1)$ average |
| **Binary Search Tree** | `ExpiryBST.cpp` | Expiry audit (In-order) | Search: $O(\log n)$, Sort: $O(n)$ |
| **Circular Queue** | `StockQueue.cpp` | Restock orders (FIFO) | Enqueue: $O(1)$, Dequeue: $O(1)$ |
