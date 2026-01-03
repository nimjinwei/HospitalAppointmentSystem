#ifndef MENUFUNCTIONS_H
#define MENUFUNCTIONS_H

#include "HospitalSystem.h"
#include <string>

// Helper functions for UI formatting
void printSeparator(int width = 60);
void printTitle(const string& title, int width = 60);

// Menu functions
void adminMenu(HospitalSystem& system);
void doctorMenu(HospitalSystem& system, Doctor* currentDoctor);
void patientMenu(HospitalSystem& system, Patient* currentPatient);

#endif