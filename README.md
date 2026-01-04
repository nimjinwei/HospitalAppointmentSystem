doctors.txt example:
# Hospital Appointment Management System

## Project Overview
This is a C++ console application implementing a small hospital appointment management system with three roles: administrator, doctor, and patient. The project focuses on manual implementations of core data structures and algorithms (linked lists, stack, custom hash table) rather than STL containers.

## Important Design Choices
- **Patient / Doctor ID**: The system uses the real-world identifier (IC / national ID) as the unique ID for patients (and a similar ID for doctors). IDs are stored as `string` to preserve leading zeros and formatting.
- **Password storage**: Passwords are stored as SHA-256 hex digests in the data files. At runtime, user input is hashed with SHA-256 and compared to the stored digest. This avoids storing plaintext passwords.
- **Fast ID lookup**: The system maintains a custom `HashTable` (separate chaining) for O(1) average-time lookups by ID. The linked lists remain the primary storage and are used for ordered traversals and sorting.

## Key Features
- User authentication for admin, doctors, and patients (passwords verified via SHA-256 hash)
- Doctor management: add / edit / delete / search, sort by name or specialization
- Patient management: register / edit / delete / search
- Appointment scheduling: create / edit / cancel, conflict detection, time slots
- System activity logs (stack-based)
- Data persistence to plain text files (with hashed passwords)

## Project Files
- [LinkedList.h](LinkedList.h) — Custom doubly linked list used for collections
- [Stack.h](Stack.h) — Stack used for activity logs
- [HashTable.h](HashTable.h) — Custom hash table for ID → object mapping
- [Doctor.h](Doctor.h) / [Doctor.cpp](Doctor.cpp) — Doctor entity and helpers (passwords stored as hashes)
- [Patient.h](Patient.h) / [Patient.cpp](Patient.cpp) — Patient entity and helpers (passwords stored as hashes)
- [Appointment.h](Appointment.h) / [Appointment.cpp](Appointment.cpp) — Appointment entity and helpers
- [HospitalSystem.h](HospitalSystem.h) / [HospitalSystem.cpp](HospitalSystem.cpp) — Core business logic, indexing, and persistence
- [MenuFunctions.h](MenuFunctions.h) / [MenuFunctions.cpp](MenuFunctions.cpp) — Console menus and UI helpers
- [Algorithms.h](Algorithms.h) — Sorting and search helpers (manual implementations)
- [sha256.h](sha256.h) / [sha256.cpp](sha256.cpp) — SHA-256 helper used to hash passwords
- [main.cpp](main.cpp) — Program entry and main loop

## Data file formats (notes)
All passwords in the files are stored as SHA-256 hex digests (64 hex characters). Example lines below show hashed-password placeholders (`<sha256-hash>`). If you currently have plaintext passwords in files, the system tolerates them and will convert to hashed storage on next edit/save, but it is recommended to store only hashed passwords.

doctors.txt (format: ID|Name|Spec|Phone|Room|PasswordHash|DateList):

```
D001|Dr. Smith|Cardiology|555-1234|R101|<sha256-hash>|2024-12-25;
```

patients.txt (format: ID|Name|Age|Phone|PasswordHash):

```
P001|Alice|30|555-5678|<sha256-hash>
```

appointments.txt (format: ApptID|DoctorID|PatientID|Date|Time|Status):

```
AP001|D001|P001|2024-12-31|09:00|Completed
```

## Data Structures & Algorithms implemented (manual)
- Custom `LinkedList` (doubly): primary storage for lists, supports pointer relinking for in-place operations.
- `Stack`: LIFO activity log implementation.
- `HashTable` (separate chaining): custom implementation to provide O(1) average ID lookups.
- `Insertion Sort` (manual pointer relinking) for sorting linked lists.
- `Linear` and `Hash`-based searches: linear scans for partial matches; hash-based exact-match searches for IDs.

## Security & Notes
- Passwords: The project hashes passwords using a built-in SHA-256 helper. For production-level security, use a salted hash (e.g., bcrypt/Argon2) and do not implement cryptography primitives by hand.
- Privacy: IC numbers are sensitive — treat data files as confidential. Consider encrypting storage or limiting access.

