#ifndef HOSPITALSYSTEM_H
#define HOSPITALSYSTEM_H

#include "LinkedList.h"
#include "Stack.h"
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include <string>

class HospitalSystem {
private:
    LinkedList<Doctor> doctors;
    LinkedList<Patient> patients;
    LinkedList<Appointment> appointments;
    Stack<string> logs;
    string adminPassword = "admin123";
    
    // Helper data
    const string predefinedSpecializations[16] = {
        "Cardiology", "Dermatology", "Emergency Medicine", "ENT",
        "Gastroenterology", "General Practice", "Neurology", "Obstetrics",
        "Oncology", "Ophthalmology", "Orthopedics", "Pediatrics",
        "Psychiatry", "Radiology", "Surgery", "Urology"
    };

public:
    HospitalSystem();
    ~HospitalSystem();

    void displaySystemLogs();
    // File Handling
    void loadData();
    void saveData();

    // Helper validation
    bool isValidDate(string date);
    bool isValidTime(string time);
    bool isValidAge(int age);
    bool isValidPhone(string phone);
    bool confirmAction(string message);

    // Authentication
    bool authenticateAdmin(string password);
    Doctor* authenticateDoctor(string id, string password);
    Patient* authenticatePatient(string id, string password);

    // Doctor Ops
    void addDoctor(const Doctor& d);
    void editDoctor(string id);
    void deleteDoctor(string id);
    Doctor* searchDoctorByID(string id);
    void searchDoctorByName(string name);
    void searchDoctorBySpecialization(string spec);
    void searchDoctorBySpecializationForPatient(string spec);
    void displayAllDoctors();
    void displayDoctorsBrief();
    void sortDoctorsByName();
    void sortDoctorsBySpecialization();
    bool doctorExists(string id);
    string selectSpecialization(); // Helper
    void displayAvailableSpecializations(); // Helper
    
    // Patient Ops
    void addPatient(const Patient& p);
    void editPatient(string id);
    void deletePatient(string id);
    Patient* searchPatientByID(string id);
    void searchPatientByName(string name);
    void displayAllPatients();
    void sortPatientsByName();
    bool patientExists(string id);

    // Appointment Ops
    void addAppointment(const Appointment& a);
    string generateAppointmentID();
    void editAppointment(string id);
    void deleteAppointment(string id);
    void cancelAppointmentByID(string id);
    void markAppointmentCompleteByID(string id);
    void searchAppointmentByDoctorID(string id);
    void searchAppointmentByPatientID(string id);
    void displayAllAppointments();
    void sortAppointmentsByDate();
    bool appointmentExists(string id);
    void displayAppointmentDetails(string apptID);
    
    // Schedule Logic
    int timeToMinutes(string time);
    string minutesToTime(int minutes);
    bool hasTimeConflict(string doctorID, string date, string time, int minInterval = 30, string excludeApptID = "");
    string showDoctorScheduleForDate(string doctorID, string date);
    string getTimeBySlotNumber(string doctorID, string date, int slotNumber);
    string getCurrentDate();
    void viewTodayScheduleByDoctorID(string doctorID);

    // Misc
    void displayStatistics();
    bool changeDoctorPassword(Doctor* doctor);
    bool changePatientPassword(Patient* patient);
    string getDoctorNameByID(string id);
    string getPatientNameByID(string id);
    
    // Get specialization list (for searching)
    void getAllSpecializations(LinkedList<string>& outList);
};

#endif