#include "Appointment.h"

Appointment::Appointment() : status("Scheduled") {}

Appointment::Appointment(string id, string docID, string patID, string date, string time, string st) {
    this->id = id;
    this->doctorID = docID;
    this->patientID = patID;
    this->date = date;
    this->time = time;
    this->status = st;
}

void Appointment::setAppointmentInfo(string id, string doctorID, string patientID, string date, string time) {
    this->id = id;
    this->doctorID = doctorID;
    this->patientID = patientID;
    this->date = date;
    this->time = time;
    this->status = "Scheduled";
}

void Appointment::reschedule(string newDate, string newTime) {
    date = newDate;
    time = newTime;
}

void Appointment::displayAppointment() const {
    cout << left << setw(10) << id 
         << setw(12) << doctorID 
         << setw(15) << patientID 
        << setw(15) << date 
        << setw(10) << time 
        << setw(15) << status << endl;
}

void Appointment::displayAppointmentSpaced(int extraSpaces) const {
    cout << left << setw(10) << id 
        << setw(12) << doctorID 
        << setw(12) << patientID;

    for(int i = 0; i < extraSpaces; ++i) cout << ' ';

    cout << left << setw(15) << date 
        << setw(10) << time 
        << setw(15) << status << endl;
}

string Appointment::toFileString() const {
    return id + "|" + doctorID + "|" + patientID + "|" + date + "|" + time + "|" + status;
}