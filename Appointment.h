#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class Appointment {
private:
    string id;
    string doctorID;
    string patientID;
    string date;
    string time;
    string status;

public:
    Appointment();
    Appointment(string id, string docID, string patID, string date, string time, string status = "Scheduled");
    
    void setAppointmentInfo(string id, string doctorID, string patientID, string date, string time);
    
    string getAppointmentID() const { return id; }
    string getDoctorID() const { return doctorID; }
    string getPatientID() const { return patientID; }
    string getDate() const { return date; }
    string getTime() const { return time; }
    string getStatus() const { return status; }
    
    void markCompleted() { status = "Completed"; }
    void cancelAppointment() { status = "Cancelled"; }
    void reschedule(string newDate, string newTime);
    
    void displayAppointment() const;
    void displayAppointmentSpaced(int extraSpaces = 3) const;
    string toFileString() const;
};

#endif