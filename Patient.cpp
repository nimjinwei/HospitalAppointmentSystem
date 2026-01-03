#include "Patient.h"

Patient::Patient() : age(0) {}

Patient::Patient(string id, string name, int age, string phone, string pwd) {
    setPatientInfo(id, name, age, phone, pwd);
}

void Patient::setPatientInfo(string id, string name, int age, string phone, string pwd) {
    this->id = id;
    this->name = name;
    this->age = age;
    this->phone = phone;
    this->password = pwd;
}

void Patient::displayPatient() const {
    cout << left << setw(10) << id 
         << setw(20) << name 
         << setw(10) << age 
         << setw(15) << phone << endl;
}

string Patient::toFileString() const {
    return id + "|" + name + "|" + to_string(age) + "|" + phone + "|" + password;
}