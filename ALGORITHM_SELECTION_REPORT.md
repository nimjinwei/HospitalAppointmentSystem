# Algorithm Selection Report
## Hospital Appointment Management System

---

## Executive Summary

This system implements a mix of algorithms optimized for typical hospital workflows:
1.  **Hashed Lookup (unordered_map)** - For unique ID (IC) lookups with O(1) average-time retrieval.
2.  **Linear Search** - For partial matches and multi-result queries.
3.  **Insertion Sort** - For ordering linked-list records via pointer relinking.

The selection balances **runtime performance** for ID lookups (using a hash index) with pointer-focused implementations for list manipulation and small/medium datasets.

---

## 1. Hashed Lookup (Single Match by IC)

### Implementation Location
- **File**: Index is built when loading `patients.txt` (e.g., `HospitalSystem.cpp`) and stored in an `std::unordered_map<std::string, Patient*>` (commonly named `indexByIC`).
- **Used in**: `searchPatientByID()` (lookup by IC), and similar index for doctors if needed.

### Why Hashed Lookup?

#### ✅ **Reasons for Selection:**

1.  **O(1) average lookup**
    - Exact-match lookups by IC benefit from constant average-time access.
    - Best/average case: $O(1)$; worst case (degenerate collisions): $O(n)$ but extremely rare with a good hash.

2.  **Simplicity at Call Site**
    - After building the index, search code becomes a single map lookup and a null check.

3.  **Practical for ID lookups**
    - ICs are natural unique keys; hashing them is straightforward (`std::string` key).

### Tradeoffs

- **Memory**: The index stores additional pointers/keys (extra memory), an acceptable trade for much faster queries.
- **Maintenance**: Insertions/deletions must update both the linked list and the hash index.

### Security & Implementation Notes

- **Password storage**: Authentication uses SHA-256 hashes stored in the data files. At runtime the input password is hashed and compared to the stored digest. This improves security over plaintext storage (see `sha256.h`/`sha256.cpp`).
- **Index implementation**: The project uses a custom `HashTable` implementation (`HashTable.h`) with separate chaining. The hash index stores object copies/pointers and must be kept in sync when the linked list is modified.
- **Maintenance requirements**: When adding/deleting/updating records you must update both the linked list (primary storage) and the hash table (index). The current implementation updates the index on load, add, delete and edit operations to prevent inconsistent state (and to avoid duplicate IDs being inserted).

### Why NOT Other Algorithms?

#### ❌ **Binary Search**
- Still incompatible with linked lists without converting to an array or adding random access structures.

#### ❌ **Pure Sequential Scan (no index)**
- For frequent ID lookups, repeated O(n) scans are much slower than hash lookups; hence the index is preferred.

---

## 2. Linear Search Algorithm (Multiple Matches)

### Implementation Location
- **File**: `Algorithms.h::linearSearchAll()` (and manual loops in system)
- **Used in**: `searchDoctorByName()`, `searchAppointmentByDoctorID()`

### Why Linear Search?

#### ✅ **Reasons for Selection:**

1.  **Exhaustive Retrieval**
    - **Scenario**: Finding "all appointments for Dr. Smith".
    - **Logic**: We cannot stop at the first match; there might be another appointment at the end of the list.
    - **Requirement**: Must traverse $n$ nodes ($O(n)$) to guarantee completeness.

2.  **Partial String Matching**
    - **Scenario**: Searching for "John" should find "John Smith" and "Dr. Johnson".
    - **Constraint**: Partial matches cannot be indexed easily or sorted effectively. Linear scanning is the only reliable method.

---

## 3. Sorting Algorithm: Insertion Sort

### Implementation Location
- **File**: `Algorithms.h::manualSort()`
- **Used in**: `sortDoctorsByName()`, `sortAppointmentsByDate()`

### Why Insertion Sort?

#### ✅ **Reasons for Selection:**

1.  **In-Place Pointer Relinking (Memory Efficiency)**
    - **Technique**: The implemented algorithm does **not** swap data values or create new nodes.
    - **Mechanism**: It detaches a node from the unsorted portion and essentially "re-links" pointers (`next`, `prev`) to insert it into the sorted portion.
    - **Result**: **Space Complexity is strictly $O(1)$**. No auxiliary array or stack is needed.

2.  **Efficiency on Small/Nearly Sorted Data**
    - **Scenario**: If a list is already partially sorted (e.g., appointments added chronologically), Insertion Sort runs in nearly $O(n)$ time.
    - **Stability**: It preserves the relative order of elements with equal keys (Critical for multi-level sorting, e.g., sorting by Date then Time).

3.  **Simplicity for Linked Lists**
    - It builds the sorted list incrementally, which is intuitive to implement using pointers without complex recursion.

### Why NOT Other Algorithms?

#### ❌ **Quick Sort**
- **Problem**: Depends heavily on picking a "pivot" and random access swapping.
- **Linked List Issue**: Very slow on linked lists due to pointer traversal for partitioning and poor cache locality. Unstable sort.

#### ❌ **Merge Sort**
- **Pros**: It is the optimal sort for Linked Lists ($O(n \log n)$).
- **Cons**: High implementation complexity (requires recursion or complex iterative logic).
- **Verdict**: While faster for huge datasets, Insertion Sort ($O(n^2)$) is sufficient for this project's scope and demonstrates better pointer manipulation skills.

#### ❌ **Bubble Sort**
- **Inefficiency**: Requires repeated passes and extensive swapping of adjacent elements.
- **Verdict**: Generally performs worse than Insertion Sort in practice because it performs more write operations (swaps) than necessary.

---

## 4. Algorithm Comparison Summary

| Algorithm | Type | Time Complexity | Space | Use Case |
|-----------|------|-----------------|-------|----------|
| **Hash Table (unordered_map)** | Search | $O(1)$ Avg / $O(n)$ Worst | $O(n)$ extra | Unique ID Lookup (IC → Patient)
| **Linear Search** | Search | $O(n)$ Always | $O(1)$ | Filters, Partial Names, Reports |
| **Insertion Sort** | Sort | $O(n^2)$ Avg / $O(n)$ Best | $O(1)$ | Reordering lists by Name/Date |
| Binary Search | Search | $O(\log n)$ | - | **Rejected**: Incompatible with Linked List |
| Merge Sort | Sort | $O(n \log n)$ | $O(\log n)$ | Useful for large datasets (future)

---

## 5. Performance & Design Analysis

### Why Linked List + Insertion Sort?
While Arrays + QuickSort are standard for static data, this system prioritizes **Dynamic Memory Management**:
* **Insertion/Deletion**: Linked Lists allow $O(1)$ node removal (e.g., cancelling an appointment) without shifting elements like an array.
* **Sorting**: Our Insertion Sort implementation leverages this by simply changing pointers, avoiding the costly data copying required in array-based sorting.

### Real-world Scalability
- **Current State**: With < 1,000 records, the index construction time is negligible and lookups are effectively instant.
- **Future Path**: For much larger datasets (100k+), keep the hash index for ID lookups and use Merge Sort or external sorting for large-scale reporting.

---

## 6. Code Evidence

### Insertion Sort (Pointer Relinking)
```cpp
// Algorithms.h
template <typename T>
void manualSort(LinkedList<T>& list, bool (*comp)(const T&, const T&)) {
    // ... logic ...
    // Detach current node
    curr->prev = curr->next = nullptr; 
    
    // Find insertion point and Relink
    curr->next = pos;
    curr->prev = pos->prev;
    pos->prev = curr;
    // ... logic ...
}