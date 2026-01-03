# Hospital Appointment Management System
## Project Overview
A simple hospital appointment management system written in C++. It supports three user roles: administrator, doctor, and patient.

## Key Features
- User authentication for admin, doctors, and patients
- Doctor management: add / edit / delete / search, sort by name or specialization
- Patient management: register / edit / delete / search
- Appointment scheduling: create / edit / cancel, conflict detection, time slots
- System activity logs (stack-based)
- Data persistence to plain text files

## Project Files
- `LinkedList.h` — Doubly linked list used for collections
- `Stack.h` — Stack used for activity logs
- `Doctor.h` / `Doctor.cpp` — Doctor entity and helpers
- `Patient.h` / `Patient.cpp` — Patient entity and helpers
- `Appointment.h` / `Appointment.cpp` — Appointment entity and helpers
- `HospitalSystem.h` / `HospitalSystem.cpp` — Core business logic
- `MenuFunctions.h` / `MenuFunctions.cpp` — Console menus and UI helpers
- `main.cpp` — Program entry and main loop

## Build and run
```bash
cd d:\\HospitalAppointmentSystem
g++ -fdiagnostics-color=always -g *.cpp -o main.exe
./main.exe
```

## Example data formats

doctors.txt example:

```
D001|Dr. Smith|Cardiology|555-1234|R101|doctor123|2024-12-25
```

patients.txt example:

```
P001|Alice|30|555-5678|patient123
```

appointments.txt example:

```
AP001|D001|P001|2024-12-31|09:00|Completed
```

## Data Structures Used

- LinkedList (custom doubly linked list): used for `doctors`, `patients`, and `appointments` collections. Supports append, remove, traversal, and in-place sorting by relinking nodes.
- Stack: used to keep recent activity logs (LIFO).


## Algorithms Used

- Linear search: used to find doctors/patients/appointments by ID or name by traversing the linked lists (O(n)).
- Insertion sort algorithm: implemented in `Algorithms.h::manualSort()` by relinking linked list nodes for in-place sorting (O(n²) time, O(1) extra space).