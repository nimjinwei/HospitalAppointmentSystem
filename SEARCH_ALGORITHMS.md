# Search Algorithms Implementation

## Overview
The project implements **Sequential Search algorithm** in `Algorithms.h` for ID-based searches, which provides early termination when the target is found.

---

## Implemented Search Algorithm

### **Sequential Search** (Early Termination)
- **Location**: `Algorithms.h::sequentialSearch()`
- **Algorithm Type**: Sequential Search
- **Time Complexity**: O(n) worst case, O(1) best case
- **Space Complexity**: O(1)
- **Use Case**: Exact match searches where we can stop immediately when found
- **Current Usage**: 
  - `searchDoctorByID()` - Stops as soon as unique ID is found
  - `searchPatientByID()` - Stops as soon as unique ID is found

### **Linear Search** (Manual Implementation)
- **Location**: Direct implementation in search functions
- **Time Complexity**: O(n)
- **Space Complexity**: O(1)
- **Use Case**: Partial matches or when all elements must be checked
- **Current Usage**:
  - `searchDoctorByName()` - Partial name matching (substring search)
  - `searchPatientByName()` - Partial name matching (substring search)
  - `searchDoctorBySpecialization()` - Multiple matches possible
  - `searchAppointmentByDoctorID()` - Multiple appointments per doctor
  - `searchAppointmentByPatientID()` - Multiple appointments per patient

---

## Algorithm Selection Strategy

### For ID Searches (Unique Identifiers)
- **Algorithm**: Sequential Search (from Algorithms.h)
- **Reason**: IDs are unique, so we can stop immediately when found
- **Example**: `searchDoctorByID()`, `searchPatientByID()`

### For Name/Specialization/Appointment Searches
- **Algorithm**: Linear Search (manual implementation in functions)
- **Reason**: Need to check all elements for partial matches or multiple results
- **Example**: `searchDoctorByName()`, `searchPatientByName()`, `searchAppointmentByDoctorID()`

---

## Code Structure

### Algorithms.h
Contains the sequential search algorithm:
- `sequentialSearch()` - Sequential search with early termination

### HospitalSystem.cpp
Uses appropriate search method based on search type:
- ID searches → Sequential Search (from Algorithms.h)
- Name/Specialization/Appointment searches → Linear Search (manual implementation)

---

## Performance

| Search Type | Algorithm | Best Case | Average Case | Worst Case |
|-------------|-----------|-----------|--------------|------------|
| ID Search | Sequential Search | O(1) | O(n/2) | O(n) |
| Name/Specialization | Linear Search | O(1) | O(n/2) | O(n) |

---

## Benefits

1. **Early Termination**: Sequential search stops immediately when ID is found
2. **Code Organization**: Search algorithm centralized in Algorithms.h
3. **Educational Value**: Demonstrates understanding of search algorithms
4. **Efficiency**: Optimal for unique identifier lookups

---

## Future Enhancements

1. **Implement Hash Table**: For O(1) ID lookups (if HashTable.h is implemented)
2. **Optimized Search for Sorted Data**: Can add optimized search after sorting
3. **Binary Search Variant**: For sorted data (though challenging with linked lists)

