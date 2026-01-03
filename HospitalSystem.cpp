#include "HospitalSystem.h"
#include "Algorithms.h"
#include "Stack.h" 
#include <fstream>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>

namespace fs = std::filesystem;

// Comparison functions for sorting
bool compareDocName(const Doctor& a, const Doctor& b) { return a.getName() > b.getName(); }
bool compareDocSpec(const Doctor& a, const Doctor& b) { return a.getSpecialization() > b.getSpecialization(); }
bool comparePatName(const Patient& a, const Patient& b) { return a.getName() > b.getName(); }
bool compareApptDate(const Appointment& a, const Appointment& b) { return a.getDate() > b.getDate(); }

// Search helper functions (for use with search algorithms)
namespace SearchHelpers {
    // Global variables to store search criteria (for function pointers)
    static string searchID;
    static string searchName;
    static string searchSpec;
    
    bool matchDoctorByID(const Doctor& d) { return d.getDoctorID() == searchID; }
    bool matchPatientByID(const Patient& p) { return p.getPatientID() == searchID; }
    bool matchDoctorByName(const Doctor& d) { return d.getName().find(searchName) != string::npos; }
    bool matchPatientByName(const Patient& p) { return p.getName().find(searchName) != string::npos; }
    bool matchDoctorBySpec(const Doctor& d) {
        string ds = d.getSpecialization();
        string ss = searchSpec;
        transform(ds.begin(), ds.end(), ds.begin(), [](unsigned char c){ return tolower(c); });
        transform(ss.begin(), ss.end(), ss.begin(), [](unsigned char c){ return tolower(c); });
        return ds == ss;
    }
    bool matchAppointmentByDoctorID(const Appointment& a) { return a.getDoctorID() == searchID; }
    bool matchAppointmentByPatientID(const Appointment& a) { return a.getPatientID() == searchID; }
    
    // Process functions for linearSearchAll
    void processDoctorDisplay(const Doctor& d) { d.displayDoctor(); }
    void processPatientDisplay(const Patient& p) { p.displayPatient(); }
    void processAppointmentDisplay(const Appointment& a) { a.displayAppointmentSpaced(2); }
    void processDoctorDisplayForPatient(const Doctor& d) { d.displayDoctorBrief(); }
}

HospitalSystem::HospitalSystem() {
    loadData();
}

HospitalSystem::~HospitalSystem() {
    saveData();
}

// ================= FILE HANDLING =================
void HospitalSystem::saveData() {
    fs::path base = fs::current_path();
    cout << "[DEBUG] Saving data to folder: " << base.string() << endl;
    // Save Doctors
    ofstream docFile((base / "doctors.txt").string());
    Node<Doctor>* dNode = doctors.getHead();
    while(dNode) {
        docFile << dNode->data.toFileString() << endl;
        dNode = dNode->next;
    }
    docFile.close();

    // Save Patients
    ofstream patFile((base / "patients.txt").string());
    Node<Patient>* pNode = patients.getHead();
    while(pNode) {
        patFile << pNode->data.toFileString() << endl;
        pNode = pNode->next;
    }
    patFile.close();

    // Save Appointments
    ofstream apptFile((base / "appointments.txt").string());
    Node<Appointment>* aNode = appointments.getHead();
    while(aNode) {
        apptFile << aNode->data.toFileString() << endl;
        aNode = aNode->next;
    }
    apptFile.close();
}

void HospitalSystem::loadData() {
    fs::path base = fs::current_path();
    cout << "[DEBUG] Loading data from folder: " << base.string() << endl;
    // Load Doctors
    ifstream docFile((base / "doctors.txt").string());
    string line;
    if(docFile.is_open()) {
        while(getline(docFile, line)) {
            LinkedList<string> parts;
            splitString(line, '|', parts);
            if(parts.getSize() >= 6) {
                Node<string>* curr = parts.getHead();
                string id = curr->data; curr = curr->next;
                string name = curr->data; curr = curr->next;
                string spec = curr->data; curr = curr->next;
                string phone = curr->data; curr = curr->next;
                string room = curr->data; curr = curr->next;
                string pwd = curr->data; curr = curr->next;
                
                Doctor d(id, name, spec, phone, room, pwd);
                // handle unavailable dates
                if(curr) d.loadDatesFromString(curr->data);
                doctors.append(d);
            }
        }
        docFile.close();
    }

    // Load Patients
    ifstream patFile((base / "patients.txt").string());
    if(patFile.is_open()) {
        while(getline(patFile, line)) {
            LinkedList<string> parts;
            splitString(line, '|', parts);
            if(parts.getSize() >= 5) {
                Node<string>* curr = parts.getHead();
                string id = curr->data; curr = curr->next;
                string name = curr->data; curr = curr->next;
                int age = stoi(curr->data); curr = curr->next;
                string phone = curr->data; curr = curr->next;
                string pwd = curr->data;
                
                patients.append(Patient(id, name, age, phone, pwd));
            }
        }
        patFile.close();
    }

    // Load Appointments
    ifstream apptFile((base / "appointments.txt").string());
    if(apptFile.is_open()) {
        while(getline(apptFile, line)) {
            LinkedList<string> parts;
            splitString(line, '|', parts);
            if(parts.getSize() >= 6) {
                Node<string>* curr = parts.getHead();
                string id = curr->data; curr = curr->next;
                string docId = curr->data; curr = curr->next;
                string patId = curr->data; curr = curr->next;
                string date = curr->data; curr = curr->next;
                string time = curr->data; curr = curr->next;
                string stat = curr->data;
                
                appointments.append(Appointment(id, docId, patId, date, time, stat));
            }
        }
        apptFile.close();
    }
}

// ================= System Log (Stack Implementation) =================
void HospitalSystem::displaySystemLogs() {
    if (logs.isEmpty()) {
        cout << "\nNo recent activity logs.\n";
    } else {
        cout << "\n=== System Activity Logs (Most Recent First) ===\n";
        logs.displayStack();
        cout << "================================================\n";
    }
}

// ================= Doctor Operations =================
void HospitalSystem::addDoctor(const Doctor& d) {
    if(doctorExists(d.getDoctorID())) {
        cout << "Error: Doctor ID already exists!\n";
        return;
    }
    doctors.append(d);
    
    // [Stack] Log Activity
    logs.push("Admin added Doctor: " + d.getName() + " (" + d.getDoctorID() + ")");
    
    cout << "Doctor added successfully.\n";
}

Doctor* HospitalSystem::searchDoctorByID(string id) {
    // Use Sequential Search: O(n) worst case, O(1) best case
    // Stops immediately when ID is found (exact match, unique ID)
    SearchHelpers::searchID = id;
    return sequentialSearch<Doctor>(doctors, SearchHelpers::matchDoctorByID);
}

void HospitalSystem::deleteDoctor(string id) {
    Node<Doctor>* curr = doctors.getHead();
    while(curr) {
        if(curr->data.getDoctorID() == id) {
            string docName = curr->data.getName();
            doctors.removeNode(curr);
            
            // [Stack] Log Activity
            logs.push("Admin deleted Doctor: " + docName + " (" + id + ")");
            
            cout << "Doctor deleted.\n";
            return;
        }
        curr = curr->next;
    }
    cout << "Doctor not found.\n";
}

void HospitalSystem::displayAllDoctors() {
    if(doctors.isEmpty()) { cout << "No doctors.\n"; return; }
    cout << "\n" << left << setw(10) << "ID" << setw(20) << "Name" 
         << setw(20) << "Specialization" << setw(15) << "Phone" << endl;
    cout << string(65, '-') << endl;
    Node<Doctor>* curr = doctors.getHead();
    while(curr) {
        curr->data.displayDoctor();
        curr = curr->next;
    }
}

void HospitalSystem::sortDoctorsByName() {
    manualSort(doctors, compareDocName);
    cout << "Doctors sorted by name.\n";
}

void HospitalSystem::sortDoctorsBySpecialization() {
    manualSort(doctors, compareDocSpec);
    cout << "Doctors sorted by specialization.\n";
}

bool HospitalSystem::doctorExists(string id) {
    return searchDoctorByID(id) != nullptr;
}

void HospitalSystem::editDoctor(string id) {
    Doctor* d = searchDoctorByID(id);
    if(d) {
        string name, phone, room, spec;
        cout << "Enter new name: "; cin.ignore(); getline(cin, name);
        spec = selectSpecialization();
        if(spec.empty()) return;
        cout << "Enter new phone: "; cin.ignore(); getline(cin, phone);
        cout << "Enter new room: "; getline(cin, room);
        d->setDoctorInfo(id, name, spec, phone, room, d->getPassword());
        
        // [Stack] Log Activity
        logs.push("Admin edited Doctor: " + id);
        
        cout << "Updated.\n";
    } 
    else 
        cout << "Not found.\n";
}

void HospitalSystem::searchDoctorByName(string name) {
    // Use Linear Search: O(n)
    // Needed for partial name matching - must check all elements
    SearchHelpers::searchName = name;
    int count = linearSearchAll<Doctor>(doctors, SearchHelpers::matchDoctorByName, 
                                        SearchHelpers::processDoctorDisplay);
    if(count == 0) 
        cout << "No doctors found.\n";
}

void HospitalSystem::searchDoctorBySpecialization(string spec) {
    // Use Linear Search: O(n)
    // Multiple matches possible, must check all elements
    SearchHelpers::searchSpec = spec;
    linearSearchAll<Doctor>(doctors, SearchHelpers::matchDoctorBySpec,
                           SearchHelpers::processDoctorDisplay);
}

void HospitalSystem::searchDoctorBySpecializationForPatient(string spec) {
    SearchHelpers::searchSpec = spec;
    linearSearchAll<Doctor>(doctors, SearchHelpers::matchDoctorBySpec,
                           SearchHelpers::processDoctorDisplayForPatient);
}

// ================= PATIENT OPS =================
void HospitalSystem::addPatient(const Patient& p) {
    if(patientExists(p.getPatientID())) {
        cout << "Error: ID exists.\n";
        return;
    }
    patients.append(p);
    
    // [Stack] Log Activity
    logs.push("New Patient registered: " + p.getName() + " (" + p.getPatientID() + ")");
    
    cout << "Patient added.\n";
}

Patient* HospitalSystem::searchPatientByID(string id) {
    // Use Sequential Search: O(n) worst case, O(1) best case
    // Stops immediately when ID is found (exact match, unique ID)
    SearchHelpers::searchID = id;
    return sequentialSearch<Patient>(patients, SearchHelpers::matchPatientByID);
}

void HospitalSystem::displayAllPatients() {
    Node<Patient>* curr = patients.getHead();
    while(curr) {
        curr->data.displayPatient();
        curr = curr->next;
    }
}

void HospitalSystem::sortPatientsByName() {
    manualSort(patients, comparePatName);
    cout << "Sorted by name.\n";
}

bool HospitalSystem::patientExists(string id) { return searchPatientByID(id) != nullptr; }

void HospitalSystem::editPatient(string id) {
    Patient* p = searchPatientByID(id);
    if(p) {
        string name, phone; int age;
        cout << "New name: "; cin.ignore(); getline(cin, name);
        cout << "New age: "; cin >> age;
        cout << "New phone: "; cin.ignore(); getline(cin, phone);
        p->setPatientInfo(id, name, age, phone, p->getPassword());
        
        // [Stack] Log Activity
        logs.push("Admin edited Patient: " + id);
        
        cout << "Updated.\n";
    } else cout << "Not found.\n";
}

void HospitalSystem::deletePatient(string id) {
    Node<Patient>* curr = patients.getHead();
    while(curr) {
        if(curr->data.getPatientID() == id) {
            string patName = curr->data.getName();
            patients.removeNode(curr);
            
            // [Stack] Log Activity
            logs.push("Admin deleted Patient: " + patName + " (" + id + ")");
            
            cout << "Deleted.\n";
            return;
        }
        curr = curr->next;
    }
}

void HospitalSystem::searchPatientByName(string name) {
    // Use Linear Search: O(n)
    // Needed for partial name matching - must check all elements
    SearchHelpers::searchName = name;
    linearSearchAll<Patient>(patients, SearchHelpers::matchPatientByName,
                            SearchHelpers::processPatientDisplay);
}

// ================= Appointment Operations =================
void HospitalSystem::addAppointment(const Appointment& a) {
    appointments.append(a);
    
    // [Stack] Log Activity
    logs.push("Appointment Booked: " + a.getAppointmentID() + " (Doc: " + a.getDoctorID() + ", Pat: " + a.getPatientID() + ")");
    
    cout << "Appointment scheduled.\n";
}

void HospitalSystem::displayAllAppointments() {
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        curr->data.displayAppointment();
        curr = curr->next;
    }
}

void HospitalSystem::sortAppointmentsByDate() {
    manualSort(appointments, compareApptDate);
    cout << "Sorted by date.\n";
}

string HospitalSystem::generateAppointmentID() {
    return "APT" + to_string(appointments.getSize() + 1001);
}

void HospitalSystem::searchAppointmentByDoctorID(string id) {
    // Use Linear Search: O(n)
    // Multiple appointments possible for one doctor, must check all elements
    SearchHelpers::searchID = id;
    linearSearchAll<Appointment>(appointments, SearchHelpers::matchAppointmentByDoctorID,
                                SearchHelpers::processAppointmentDisplay);
}

void HospitalSystem::searchAppointmentByPatientID(string id) {
    // Use Linear Search: O(n)
    // Multiple appointments possible for one patient, must check all elements
    SearchHelpers::searchID = id;
    linearSearchAll<Appointment>(appointments, SearchHelpers::matchAppointmentByPatientID,
                                SearchHelpers::processAppointmentDisplay);
}

void HospitalSystem::cancelAppointmentByID(string id) {
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getAppointmentID() == id) {
            curr->data.cancelAppointment();
            
            // [Stack] Log Activity
            logs.push("Appointment Cancelled: " + id);
            
            cout << "Cancelled.\n";
            return;
        }
        curr = curr->next;
    }
    cout << "Not found.\n";
}

void HospitalSystem::markAppointmentCompleteByID(string id) {
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getAppointmentID() == id) {
            curr->data.markCompleted();
            
            // [Stack] Log Activity
            logs.push("Appointment Completed: " + id);
            
            cout << "Completed.\n";
            return;
        }
        curr = curr->next;
    }
}

void HospitalSystem::deleteAppointment(string id) {
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getAppointmentID() == id) {
            appointments.removeNode(curr);
            
            // [Stack] Log Activity
            logs.push("Appointment Deleted from records: " + id);
            
            cout << "Deleted.\n";
            return;
        }
        curr = curr->next;
    }
}

void HospitalSystem::editAppointment(string id) {
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getAppointmentID() == id) {
            string date, time;
            cout << "Current: " << curr->data.getDate() << " " << curr->data.getTime() << endl;
            cout << "New Date: "; cin >> date;
            showDoctorScheduleForDate(curr->data.getDoctorID(), date);
            cout << "New Time: "; cin >> time;
            if(!hasTimeConflict(curr->data.getDoctorID(), date, time, 30, id)) {
                curr->data.reschedule(date, time);
                
                // [Stack] Log Activity
                logs.push("Appointment Rescheduled: " + id + " to " + date + " " + time);
                
                cout << "Updated.\n";
            } else {
                cout << "Conflict detected.\n";
            }
            return;
        }
        curr = curr->next;
    }
}

void HospitalSystem::displayAppointmentDetails(string apptID) {
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getAppointmentID() == apptID) {
            cout << "ID: " << curr->data.getAppointmentID() << "\n";
            cout << "Doc: " << getDoctorNameByID(curr->data.getDoctorID()) << "\n";
            cout << "Pat: " << getPatientNameByID(curr->data.getPatientID()) << "\n";
            cout << "When: " << curr->data.getDate() << " " << curr->data.getTime() << "\n";
            cout << "Status: " << curr->data.getStatus() << "\n";
            return;
        }
        curr = curr->next;
    }
    cout << "Not found.\n";
}

bool HospitalSystem::appointmentExists(string id) {
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getAppointmentID() == id) return true;
        curr = curr->next;
    }
    return false;
}

// ================= Authentication & Helper Functions =================
bool HospitalSystem::authenticateAdmin(string password) { return password == adminPassword; }

Doctor* HospitalSystem::authenticateDoctor(string id, string password) {
    Doctor* d = searchDoctorByID(id);
    if(d && d->getPassword() == password) return d;
    return nullptr;
}

Patient* HospitalSystem::authenticatePatient(string id, string password) {
    Patient* p = searchPatientByID(id);
    if(p && p->getPassword() == password) return p;
    return nullptr;
}

string HospitalSystem::getDoctorNameByID(string id) {
    Doctor* d = searchDoctorByID(id);
    return d ? d->getName() : "Unknown";
}

string HospitalSystem::getPatientNameByID(string id) {
    Patient* p = searchPatientByID(id);
    return p ? p->getName() : "Unknown";
}

int HospitalSystem::timeToMinutes(string time) {
    int h = stoi(time.substr(0, 2));
    int m = stoi(time.substr(3, 2));
    return h * 60 + m;
}

string HospitalSystem::minutesToTime(int minutes) {
    int h = minutes / 60;
    int m = minutes % 60;
    string hs = (h < 10 ? "0" : "") + to_string(h);
    string ms = (m < 10 ? "0" : "") + to_string(m);
    return hs + ":" + ms;
}

bool HospitalSystem::hasTimeConflict(string doctorID, string date, string time, int minInterval, string excludeApptID) {
    int newMins = timeToMinutes(time);
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getAppointmentID() == excludeApptID) { curr = curr->next; continue; }
        
        if(curr->data.getDoctorID() == doctorID && 
           curr->data.getDate() == date && 
           curr->data.getStatus() != "Cancelled") {
            int existingMins = timeToMinutes(curr->data.getTime());
            if(abs(newMins - existingMins) < minInterval) return true;
        }
        curr = curr->next;
    }
    return false;
}

string HospitalSystem::showDoctorScheduleForDate(string doctorID, string date) {
    Doctor* doc = searchDoctorByID(doctorID);
    if(doc && !doc->isAvailableOnDate(date)) {
        cout << "Doctor unavailable on this date.\n";
        return "0";
    }

    cout << "\nAvailable Slots for " << date << ":\n";
    int workStart = 9 * 60, workEnd = 17 * 60, lunchStart = 12 * 60, lunchEnd = 13 * 60;
    
    // Collect all booked times for the doctor on the given date
    LinkedList<int> bookedTimes;
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getDoctorID() == doctorID && curr->data.getDate() == date && curr->data.getStatus() != "Cancelled") {
            bookedTimes.append(timeToMinutes(curr->data.getTime()));
        }
        curr = curr->next;
    }

    int slotNum = 1;
    int count = 0;
    for(int t = workStart; t < workEnd; t += 30) {
        if(t >= lunchStart && t < lunchEnd) continue;
        
        bool booked = false;
        Node<int>* timeNode = bookedTimes.getHead();
        while(timeNode) {
            if(abs(timeNode->data - t) < 30) { booked = true; break; }
            timeNode = timeNode->next;
        }

        if(!booked) {
            cout << "[" << slotNum++ << "] " << minutesToTime(t) << "\n";
            count++;
        }
    }
    return to_string(count);
}

string HospitalSystem::getTimeBySlotNumber(string doctorID, string date, int slotNumber) {
    // Simplify logic: recompute to find the requested slot
    int workStart = 9 * 60, workEnd = 17 * 60, lunchStart = 12 * 60, lunchEnd = 13 * 60;
    LinkedList<int> bookedTimes;
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getDoctorID() == doctorID && curr->data.getDate() == date && curr->data.getStatus() != "Cancelled") {
            bookedTimes.append(timeToMinutes(curr->data.getTime()));
        }
        curr = curr->next;
    }

    int currentSlot = 1;
    for(int t = workStart; t < workEnd; t += 30) {
        if(t >= lunchStart && t < lunchEnd) continue;
        bool booked = false;
        Node<int>* timeNode = bookedTimes.getHead();
        while(timeNode) {
            if(abs(timeNode->data - t) < 30) { booked = true; break; }
            timeNode = timeNode->next;
        }
        if(!booked) {
            if(currentSlot == slotNumber) return minutesToTime(t);
            currentSlot++;
        }
    }
    return "";
}

// ================= Validation & Confirmation =================
bool HospitalSystem::isValidDate(string date) { return date.length() == 10; } // simple check
bool HospitalSystem::isValidTime(string time) { return time.length() == 5; }
bool HospitalSystem::isValidAge(int age) { return age > 0 && age < 150; }
bool HospitalSystem::isValidPhone(string phone) { return phone.length() >= 10; }
bool HospitalSystem::confirmAction(string msg) { 
    cout << msg << " (y/n): "; char c; cin >> c; return (c == 'y' || c == 'Y'); 
}

string HospitalSystem::selectSpecialization() {
    cout << "Select Specialization:\n";
    for(int i=0; i<16; i++) cout << (i+1) << ". " << predefinedSpecializations[i] << endl;
    cout << "Choice: "; int c; cin >> c;
    if(c > 0 && c <= 16) return predefinedSpecializations[c-1];
    return "";
}

void HospitalSystem::displayAvailableSpecializations() {
    //show all specializations in current records
    LinkedList<string> specs;
    getAllSpecializations(specs);
    Node<string>* curr = specs.getHead();
    int i = 1;
    while(curr) {
        cout << i++ << ". " << curr->data << endl;
        curr = curr->next;
    }
}

void HospitalSystem::getAllSpecializations(LinkedList<string>& outList) {
    Node<Doctor>* curr = doctors.getHead();
    while(curr) {
        //check specialization list
        bool exists = false;
        Node<string>* specNode = outList.getHead();
        while(specNode) {
            if(specNode->data == curr->data.getSpecialization()) { exists = true; break; }
            specNode = specNode->next;
        }
        if(!exists) outList.append(curr->data.getSpecialization());
        curr = curr->next;
    }
}

void HospitalSystem::displayDoctorsBrief() {
    if(doctors.isEmpty()) { cout << "None.\n"; return; }
    Node<Doctor>* curr = doctors.getHead();
    while(curr) {
        curr->data.displayDoctorBrief();
        curr = curr->next;
    }
}

string HospitalSystem::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[12];
    sprintf(buf, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buf);
}

void HospitalSystem::viewTodayScheduleByDoctorID(string doctorID) {
    string today = getCurrentDate();
    bool found = false;
    Node<Appointment>* curr = appointments.getHead();
    while(curr) {
        if(curr->data.getDoctorID() == doctorID && curr->data.getDate() == today) {
            curr->data.displayAppointmentSpaced(2);
            found = true;
        }
        curr = curr->next;
    }
    if(!found) cout << "No appointments today.\n";
}

void HospitalSystem::displayStatistics() {
    cout << "Doctors: " << doctors.getSize() << "\n";
    cout << "Patients: " << patients.getSize() << "\n";
    cout << "Appointments: " << appointments.getSize() << "\n";
}

bool HospitalSystem::changeDoctorPassword(Doctor* doctor) {
    string p1, p2;
    cout << "New Pass: "; cin >> p1;
    cout << "Confirm: "; cin >> p2;
    if(p1 == p2) { doctor->setPassword(p1); return true; }
    return false;
}

bool HospitalSystem::changePatientPassword(Patient* patient) {
    string p1, p2;
    cout << "New Pass: "; cin >> p1;
    cout << "Confirm: "; cin >> p2;
    if(p1 == p2) { patient->setPassword(p1); return true; }
    return false;
}