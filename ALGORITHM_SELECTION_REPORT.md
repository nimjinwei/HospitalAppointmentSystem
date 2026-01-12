# Algorithm Selection & Implementation Report
## Hospital Appointment Management System

---

## 1. Executive Summary

The Hospital Appointment Management System employs a **hybrid algorithmic approach** tailored to specific workflow requirements. By balancing runtime performance with memory efficiency, the system utilizes:

1.  **Hashed Lookup (Custom/Map)**: For $O(1)$ average-time exact ID lookups (e.g., login, patient retrieval).
2.  **Linear Search**: For partial string matching, filters, and multi-result queries.
3.  **Insertion Sort**: For memory-efficient ordering of linked lists via pointer relinking.

This selection strategy ensures instant access for critical operations while maintaining flexibility for reporting and filtering.

---

## 2. Implementation & Security Foundations

Before detailing specific search/sort algorithms, the system relies on the following core data and security structures:

### A. ID & Indexing Structure
* **IC as Unique ID**: Patient IDs are expected to be the national ID / IC. These are stored as `std::string` to preserve formatting (e.g., leading zeros) and serve as the primary key.
* **Index Construction**: An index (e.g., `indexByIC`) is built dynamically in `HospitalSystem.cpp` during the data load phase (reading `patients.txt`/`doctors.txt`).
* **Synchronization**: The index must be updated on all `add`, `delete`, and `edit` operations to remain consistent with the primary linked list storage.

### B. Security (Password Handling)
* **Algorithm**: SHA-256 Hashing.
* **Implementation**: Defined in `sha256.h` / `sha256.cpp`.
* **Logic**: Passwords are **never** stored in plaintext. Authentication compares `SHA-256(input)` against the stored digest in the text files.

---

## 3. Search Algorithms: Selection & Trade-offs

### A. Hashed Lookup (Exact Match)
**Used for:** Login authentication, finding a specific Patient/Doctor by unique IC.

* **Algorithm**: Hash Table (Exact-Match).
* **Implementation**: Utilizes a custom `HashTable.h` (or `std::unordered_map`) to map `std::string ID` $\rightarrow$ `Object Pointer`.
* **Time Complexity**: $O(1)$ Average / $O(n)$ Worst (rare collisions).

#### Comparative Analysis: Why Hash Table?
| Comparison | Reason for Selection |
| :--- | :--- |
| **VS Linear Search** | Linear search requires $O(n)$ traversal. For frequent lookups (like login), repeated linear scans are inefficient. Hashing provides instant access. |
| **VS Binary Search** | Binary Search requires **Random Access** (arrays). Since our data is stored in **Linked Lists**, we cannot jump to the middle element, making Binary Search impossible without costly data conversion. |

### B. Linear Search (Partial & Multi-Match)
**Used for:** Searching by Name (substring), Specialization, or finding all appointments for a user.

* **Algorithm**: Iterative Traversal (Manual Loop).
* **Location**: Helper functions in `Algorithms.h`.
* **Time Complexity**: $O(n)$ (Must traverse the full list).

#### Comparative Analysis: Why Linear Search?
| Comparison | Reason for Selection |
| :--- | :--- |
| **VS Hash Table** | Hash tables only support **Exact Matches**. They cannot handle logic like "Find names containing 'John'". Only Linear Search can process partial string matches effectively. |
| **VS Binary Search** | Names and specializations are often entered in random order. Binary Search requires sorted data and random access, neither of which is available for these fields in our Linked List structure. |

---

## 4. Sorting Algorithm: Selection & Trade-offs

### Insertion Sort (Pointer Relinking)
**Used for:** Ordering lists by Name, Date, or ID.

* **Algorithm**: Insertion Sort (Adapted for Linked Lists).
* **Location**: `Algorithms.h::manualSort()`.
* **Time Complexity**: $O(n^2)$ Average / $O(n)$ Best (nearly sorted).
* **Space Complexity**: $O(1)$ (Strict).

#### Comparative Analysis: Why Insertion Sort?

1.  **In-Place Pointer Manipulation (Memory Efficiency)**
    * **Why it wins**: Unlike array sorting which moves data, this implementation only changes `next`/`prev` pointers. It does not require creating new nodes or swapping large data objects.

2.  **Stability**
    * **Why it wins**: It preserves the relative order of elements with equal keys. This is critical for multi-level sorting (e.g., sorting by Date, then by Time).

3.  **Why NOT Other Algorithms?**

| Algorithm | Status | Reason for Rejection |
| :--- | :--- | :--- |
| **Quick Sort** | ❌ Rejected | **Incompatible with Linked Lists**. Quick Sort relies on picking a "pivot" via random access (`arr[i]`), which is very slow on linked lists. It is also an unstable sort. |
| **Merge Sort** | ❌ Rejected | **Complexity Overhead**. While optimal ($O(n \log n)$), it requires complex recursion and extra space/logic. For the current dataset size (<10,000), Insertion Sort is sufficient and simpler to implement. |
| **Bubble Sort** | ❌ Rejected | **Inefficiency**. Requires excessive "swapping" operations. Even on linked lists, the number of write operations is significantly higher than Insertion Sort, making it slower in practice. |

---

## 5. Performance Summary

| Search/Sort Type | Algorithm | Best Case | Average Case | Worst Case | Space |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **ID Search** | Hash Table | $O(1)$ | $O(1)$ | $O(n)$ | $O(n)$ |
| **Filter/Name** | Linear Search | $O(1)$ | $O(n/2)$ | $O(n)$ | $O(1)$ |
| **Sorting** | Insertion Sort | $O(n)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ |



---

## 6. Future Enhancements

1.  **Validation**: Ensure stricter input normalization for ICs during the loading phase to guarantee index integrity.
2.  **Scalability**: If the dataset size grows significantly (e.g., >100k records), consider:
    * Persisting a compact index to disk to reduce startup load time.
    * Switching to Merge Sort or external sorting for large-scale reporting.