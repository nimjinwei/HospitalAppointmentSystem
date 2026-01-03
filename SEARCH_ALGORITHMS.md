### For Name/Specialization/Appointment Searches
# Search Algorithms Implementation

## Overview
The project uses a hybrid approach:
- **Hash table (unordered_map)** for exact ID (IC) lookups — O(1) average-time.
- **Linear scan** for partial matches and multi-result queries.

---

## Implementation & Security Notes

- **IC as ID**: Patient IDs are expected to be the national ID / IC (stored as `std::string` to preserve leading zeros and formatting). This is the primary exact-match key used by the hash index.
- **Index implementation**: A custom `HashTable` (`HashTable.h`) provides ID → object mapping. The index is built during data load and must be updated on add/delete/edit operations.
- **Password handling**: Authentication compares SHA-256(password_input) with the stored password digest. Password hashes are persisted in `patients.txt`/`doctors.txt` rather than plaintext (see `sha256.h`/`sha256.cpp`).


---

## Implemented Search Approaches

### **Hashed Lookup (ID / IC)**
- **Location**: Index built during load in `HospitalSystem.cpp` (example: `std::unordered_map<std::string, Patient*> indexByIC`).
- **Algorithm Type**: Hash table exact-match lookup
- **Time Complexity**: $O(1)$ average, $O(n)$ worst (rare)
- **Space Complexity**: $O(n)$ extra for the index
- **Use Case**: Exact ID lookups (login, fetch patient by IC)
- **Current Usage**:
  - `searchPatientByID()` - Single map lookup and pointer validation
  - `searchDoctorByID()` - (if a doctor index is built similarly)

### **Linear Search** (Manual Implementation)
- **Location**: Direct traversal in search functions
- **Time Complexity**: $O(n)$
- **Space Complexity**: $O(1)$
- **Use Case**: Partial matches, filters, and retrieving all matching entries
- **Current Usage**:
  - `searchDoctorByName()` - Partial name matching (substring search)
  - `searchPatientByName()` - Partial name matching (substring search)
  - `searchDoctorBySpecialization()` - Multiple matches possible
  - `searchAppointmentByDoctorID()` - Multiple appointments per doctor
  - `searchAppointmentByPatientID()` - Multiple appointments per patient

---

## Algorithm Selection Strategy

### For ID Searches (Unique Identifiers)
- **Algorithm**: Hash table (unordered_map)
- **Reason**: IC is a unique, fixed key; hashing gives fast exact-match retrieval with a very small runtime cost to maintain the index on insert/delete.

### For Name/Specialization/Appointment Searches
- **Algorithm**: Linear Search (manual)
- **Reason**: Need full traversal for partial matches and to collect multiple results.

---

## Code Structure

### HospitalSystem (Indexing)
- Build the `indexByIC` when loading `patients.txt` and update it on insert/delete operations.

### Algorithms.h / Search Functions
- Keep linear-scan helper(s) for substring and multi-result queries.

---

## Performance

| Search Type | Algorithm | Best Case | Average Case | Worst Case |
|-------------|-----------|-----------|--------------|------------|
| ID Search | Hash Table | O(1) | O(1) | O(n) |
| Name/Specialization | Linear Search | O(1) | O(n/2) | O(n) |

---

## Benefits

1. **Fast ID Lookups**: Hash table provides near-constant-time retrieval for IC-based searches.
2. **Clear Separation**: Indexing handles exact-match fast paths; linear scans handle complex queries.
3. **Maintainability**: Simple insert/delete hooks keep index consistent.

---

## Notes / Future Enhancements

1. Ensure `Patient` IC is stored as `std::string` and validated/normalized when loading.
2. Consider persisting a compact index if startup load time becomes significant.
3. For very large datasets, combine the index with database-backed storage or paging.

