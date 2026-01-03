#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class Patient {
private:
    string id;
    string name;
    int age;
    string phone;
    string password;

public:
    Patient();
    Patient(string id, string name, int age, string phone, string pwd);
    
    void setPatientInfo(string id, string name, int age, string phone, string pwd = "patient123");
    
    string getPatientID() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getPhone() const { return phone; }
    string getPassword() const { return password; }
    void setPassword(string newPassword); // store hashed password
    void setPasswordHash(const string& hash); // store already-hashed password
    
    void displayPatient() const;
    string toFileString() const;
};

#endif