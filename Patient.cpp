#include "Patient.h"
#include "sha256.h"

Patient::Patient() : age(0) {}

Patient::Patient(string id, string name, int age, string phone, string pwd) {
    setPatientInfo(id, name, age, phone, pwd);
}

void Patient::setPatientInfo(string id, string name, int age, string phone, string pwd) {
    this->id = id;
    this->name = name;
    this->age = age;
    this->phone = phone;
    bool looksLikeHash = (pwd.size() == 64);
    if(looksLikeHash) setPasswordHash(pwd);
    else setPassword(pwd);
}

void Patient::displayPatient() const {
    cout << left << setw(15) << id 
         << setw(20) << name 
         << setw(10) << age 
         << setw(15) << phone << endl;
}

string Patient::toFileString() const {
    return id + "|" + name + "|" + to_string(age) + "|" + phone + "|" + password;
}

void Patient::setPassword(string newPassword) {
    this->password = sha256(newPassword);
}

void Patient::setPasswordHash(const string& hash) {
    this->password = hash;
}