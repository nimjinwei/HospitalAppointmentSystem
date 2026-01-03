#include "Doctor.h"
#include "sha256.h"
#include "Algorithms.h" // for splitString

Doctor::Doctor() {}

Doctor::Doctor(string id, string name, string spec, string phone, string room, string pwd) {
    setDoctorInfo(id, name, spec, phone, room, pwd);
}

void Doctor::setDoctorInfo(string id, string name, string spec, string phone, string room, string pwd) {
    this->id = id;
    this->name = name;
    this->specialization = spec;
    this->phone = phone;
    this->room = room;
    // pwd may be a plaintext password or a stored hash (hex 64 chars). Detect and set appropriately.
    bool looksLikeHash = (pwd.size() == 64);
    if(looksLikeHash) setPasswordHash(pwd);
    else setPassword(pwd);
}

void Doctor::setPassword(string newPassword) {
    this->password = sha256(newPassword);
}

void Doctor::setPasswordHash(const string& hash) {
    this->password = hash;
}

void Doctor::addUnavailableDate(string date) {
    // check existence
    if (isAvailableOnDate(date)) { // if available, the date is not present
        unavailableDates.append(date);
    }
}

void Doctor::removeUnavailableDate(string date) {
    Node<string>* curr = unavailableDates.getHead();
    while (curr) {
        if (curr->data == date) {
            unavailableDates.removeNode(curr);
            return;
        }
        curr = curr->next;
    }
}

bool Doctor::isAvailableOnDate(string date) const {
    Node<string>* curr = unavailableDates.getHead();
    while (curr) {
        if (curr->data == date) return false; // found, not available
        curr = curr->next;
    }
    return true;
}

void Doctor::displayUnavailableDates() const {
    if (unavailableDates.isEmpty()) {
        cout << "No unavailable dates set." << endl;
        return;
    }
    cout << "\n=== Unavailable Dates ===\n";
    Node<string>* curr = unavailableDates.getHead();
    while (curr) {
        cout << "* " << curr->data << endl;
        curr = curr->next;
    }
}

void Doctor::displayDoctor() const {
    cout << left << setw(10) << id 
         << setw(20) << name 
         << setw(20) << specialization 
         << setw(15) << phone 
         << setw(10) << room << endl;
}

void Doctor::displayDoctorBrief() const {
    cout << left << setw(10) << id 
         << setw(25) << name 
         << setw(25) << specialization << endl;
}

string Doctor::toFileString() const {
    string datesStr = "";
    Node<string>* curr = unavailableDates.getHead();
    while(curr) {
        datesStr += curr->data + ";"; // separate dates with semicolon
        curr = curr->next;
    }
    if(datesStr.empty()) datesStr = "NONE";
    
    // format: ID|Name|Spec|Phone|Room|Pass|Date1;Date2;
    return id + "|" + name + "|" + specialization + "|" + phone + "|" + room + "|" + password + "|" + datesStr;
}

void Doctor::loadDatesFromString(string datesStr) {
    if (datesStr == "NONE" || datesStr.empty()) return;
    LinkedList<string> tempList;
    splitString(datesStr, ';', tempList);
    
    Node<string>* curr = tempList.getHead();
    while(curr) {
        addUnavailableDate(curr->data);
        curr = curr->next;
    }
}