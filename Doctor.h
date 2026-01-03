#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <iostream>
#include <iomanip>
#include "LinkedList.h"

using namespace std;

class Doctor {
private:
    string id;
    string name;
    string specialization;
    string phone;
    string room;
    string password;
    LinkedList<string> unavailableDates; // uses custom LinkedList

public:
    Doctor();
    Doctor(string id, string name, string spec, string phone, string room, string pwd);

    void setDoctorInfo(string id, string name, string spec, string phone, string room, string pwd = "doctor123");

    string getDoctorID() const { return id; }
    string getName() const { return name; }
    string getSpecialization() const { return specialization; }
    string getPhone() const { return phone; }
    string getRoom() const { return room; }
    string getPassword() const { return password; }
    void setPassword(string newPassword) { password = newPassword; }

    // Availability management
    void addUnavailableDate(string date);
    void removeUnavailableDate(string date);
    bool isAvailableOnDate(string date) const;
    void displayUnavailableDates() const;

    void displayDoctor() const;
    void displayDoctorBrief() const;
    
    // File helper functions
    string toFileString() const;
    void loadDatesFromString(string datesStr); // load unavailable dates from file string
};

#endif