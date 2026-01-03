#include "MenuFunctions.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

// Helper function: Print separator line
void printSeparator(int width) {
    cout << string(width, '=') << endl;
}

// Helper function: Print title
void printTitle(const string& title, int width) {
    cout << "\n";
    printSeparator(width);
    int padding = (width - title.length() - 2) / 2;
    cout << "|" << string(padding, ' ') << title << string(width - padding - title.length() - 2, ' ') << "|" << endl;
    printSeparator(width);
}

// --- Helper Functions for sub-menus ---
void adminDoctorMgmt(HospitalSystem& sys) {
    int c;
    do {
        printTitle("Doctor Management");
        cout << "  [1] Add Doctor" << endl;
        cout << "  [2] Edit Doctor Information" << endl;
        cout << "  [3] Delete Doctor" << endl;
        cout << "  [4] Search Doctor by ID" << endl;
        cout << "  [5] List All Doctors" << endl;
        cout << "  [6] Sort Doctors by Name" << endl;
        cout << "  [0] Back to Main Menu" << endl;
        printSeparator();
        cout << "Please select an option: ";
        cin >> c;
        
        string id, name, spec, ph, room;
        if (c == 1) {
            printTitle("Add New Doctor");
            cout << "Please enter doctor information:" << endl;
            cout << "  Doctor ID: "; cin >> id;
            if (sys.doctorExists(id)) {
                cout << "\n[ERROR] Doctor ID already exists!\n" << endl;
                continue;
            }
            cout << "  Name: "; cin.ignore(); getline(cin, name);
            spec = sys.selectSpecialization();
            if (spec.empty()) {
                cout << "\nOperation cancelled\n" << endl;
                continue;
            }
            cout << "  Phone: "; cin.ignore(); getline(cin, ph);
            cout << "  Room: "; getline(cin, room);
            cout << "  Default Password: 123" << endl;
            sys.addDoctor(Doctor(id, name, spec, ph, room, "123"));
            cout << "\n[SUCCESS] Doctor added successfully!\n" << endl;
        } else if (c == 2) {
            printTitle("Edit Doctor Information");
            cout << "Enter Doctor ID to edit: "; cin >> id;
            Doctor* d = sys.searchDoctorByID(id);
            if (d) {
                cout << "\nCurrent Doctor Information:" << endl;
                d->displayDoctor();
                sys.editDoctor(id);
            } else {
                cout << "\n[ERROR] Doctor not found!\n" << endl;
            }
        } else if (c == 3) {
            printTitle("Delete Doctor");
            cout << "[WARNING] Deleting doctor will also delete all related appointments!" << endl;
            cout << "Enter Doctor ID to delete: "; cin >> id;
            Doctor* d = sys.searchDoctorByID(id);
            if (d) {
                cout << "\nDoctor Information:" << endl;
                d->displayDoctor();
                cout << "\nConfirm deletion? (y/n): ";
                char confirm; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    sys.deleteDoctor(id);
                    cout << "\n[SUCCESS] Doctor deleted successfully\n" << endl;
                } else {
                    cout << "\nOperation cancelled\n" << endl;
                }
            } else {
                cout << "\n[ERROR] Doctor not found!\n" << endl;
            }
        } else if (c == 4) {
            printTitle("Search Doctor");
            cout << "Enter Doctor ID: "; cin >> id;
            Doctor* d = sys.searchDoctorByID(id);
            if (d) {
                cout << "\nDoctor Found:" << endl;
                cout << left << setw(10) << "ID" << setw(20) << "Name" 
                     << setw(20) << "Specialization" << setw(15) << "Phone" << endl;
                cout << string(65, '-') << endl;
                d->displayDoctor();
                cout << endl;
            } else {
                cout << "\n[ERROR] Doctor not found!\n" << endl;
            }
        } else if (c == 5) {
            printTitle("All Doctors List");
            sys.displayAllDoctors();
            cout << endl;
        } else if (c == 6) {
            sys.sortDoctorsByName();
            cout << "\n[SUCCESS] Sorting completed!\n" << endl;
        } else if (c != 0) {
            cout << "\n[ERROR] Invalid option, please try again!\n" << endl;
        }
    } while (c != 0);
}

void adminPatientMgmt(HospitalSystem& sys) {
    int pc;
    do {
        printTitle("Patient Management");
        cout << "  [1] List All Patients" << endl;
        cout << "  [2] Search Patient by ID" << endl;
        cout << "  [3] Search Patient by Name" << endl;
        cout << "  [4] Edit Patient Information" << endl;
        cout << "  [5] Delete Patient" << endl;
        cout << "  [6] Sort Patients by Name" << endl;
        cout << "  [0] Back to Main Menu" << endl;
        printSeparator();
        cout << "Please select an option: ";
        cin >> pc;
        
        string id, name;
        if (pc == 1) {
            printTitle("All Patients List");
            cout << left << setw(15) << "ID" << setw(20) << "Name" 
                 << setw(10) << "Age" << setw(15) << "Phone" << endl;
            cout << string(55, '-') << endl;
            sys.displayAllPatients();
            cout << endl;
        } else if (pc == 2) {
            printTitle("Search Patient");
            cout << "Enter Patient ID: "; cin >> id;
            Patient* p = sys.searchPatientByID(id);
            if (p) {
                cout << "\nPatient Found:" << endl;
                cout << left << setw(10) << "ID" << setw(20) << "Name" 
                     << setw(10) << "Age" << setw(15) << "Phone" << endl;
                cout << string(55, '-') << endl;
                p->displayPatient();
                cout << endl;
            } else {
                cout << "\n[ERROR] Patient not found!\n" << endl;
            }
        } else if (pc == 3) {
            printTitle("Search Patient by Name");
            cout << "Enter Patient Name: "; 
            cin.ignore(); getline(cin, name);
            cout << "\nSearch Results:" << endl;
            cout << left << setw(10) << "ID" << setw(20) << "Name" 
                 << setw(10) << "Age" << setw(15) << "Phone" << endl;
            cout << string(55, '-') << endl;
            sys.searchPatientByName(name);
            cout << endl;
        } else if (pc == 4) {
            printTitle("Edit Patient Information");
            cout << "Enter Patient ID to edit: "; cin >> id;
            Patient* p = sys.searchPatientByID(id);
            if (p) {
                cout << "\nCurrent Patient Information:" << endl;
                p->displayPatient();
                sys.editPatient(id);
            } else {
                cout << "\n[ERROR] Patient not found!\n" << endl;
            }
        } else if (pc == 5) {
            printTitle("Delete Patient");
            cout << "[WARNING] Deleting patient will also delete all related appointments!" << endl;
            cout << "Enter Patient ID to delete: "; cin >> id;
            Patient* p = sys.searchPatientByID(id);
            if (p) {
                cout << "\nPatient Information:" << endl;
                p->displayPatient();
                cout << "\nConfirm deletion? (y/n): ";
                char confirm; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    sys.deletePatient(id);
                    cout << "\n[SUCCESS] Patient deleted successfully\n" << endl;
                } else {
                    cout << "\nOperation cancelled\n" << endl;
                }
            } else {
                cout << "\n[ERROR] Patient not found!\n" << endl;
            }
        } else if (pc == 6) {
            sys.sortPatientsByName();
            cout << "\n[SUCCESS] Sorting completed!\n" << endl;
        } else if (pc != 0) {
            cout << "\n[ERROR] Invalid option, please try again!\n" << endl;
        }
    } while (pc != 0);
}

void adminAppointmentMgmt(HospitalSystem& sys) {
    int ac;
    do {
        printTitle("Appointment Management");
        cout << "  [1] List All Appointments" << endl;
        cout << "  [2] View Appointments by Doctor ID" << endl;
        cout << "  [3] View Appointments by Patient ID" << endl;
        cout << "  [4] View Appointment Details" << endl;
        cout << "  [5] Edit Appointment" << endl;
        cout << "  [6] Cancel Appointment" << endl;
        cout << "  [7] Mark Appointment as Completed" << endl;
        cout << "  [8] Delete Appointment Record" << endl;
        cout << "  [9] Sort Appointments by Date" << endl;
        cout << "  [0] Back to Main Menu" << endl;
        printSeparator();
        cout << "Please select an option: ";
        cin >> ac;
        
        string id;
        if (ac == 1) {
            printTitle("All Appointments List");
            cout << left << setw(10) << "Appt ID" << setw(12) << "Doctor ID" 
                 << setw(15) << "Patient ID" << setw(15) << "Date" 
                 << setw(10) << "Time" << setw(15) << "Status" << endl;
            cout << string(74, '-') << endl;
            sys.displayAllAppointments();
            cout << endl;
        } else if (ac == 2) {
            printTitle("View Appointments by Doctor");
            cout << "Enter Doctor ID: "; cin >> id;
            if (sys.doctorExists(id)) {
                cout << "\nAll Appointments for this Doctor:" << endl;
                cout << left << setw(10) << "Appt ID" << setw(12) << "Doctor ID" 
                     << setw(12) << "Patient ID" << setw(15) << "Date" 
                     << setw(10) << "Time" << setw(15) << "Status" << endl;
                cout << string(74, '-') << endl;
                sys.searchAppointmentByDoctorID(id);
                cout << endl;
            } else {
                cout << "\n[ERROR] Doctor not found!\n" << endl;
            }
        } else if (ac == 3) {
            printTitle("View Appointments by Patient");
            cout << "Enter Patient ID: "; cin >> id;
            if (sys.patientExists(id)) {
                cout << "\nAll Appointments for this Patient:" << endl;
                cout << left << setw(10) << "Appt ID" << setw(12) << "Doctor ID" 
                     << setw(12) << "Patient ID" << setw(15) << "Date" 
                     << setw(10) << "Time" << setw(15) << "Status" << endl;
                cout << string(74, '-') << endl;
                sys.searchAppointmentByPatientID(id);
                cout << endl;
            } else {
                cout << "\n[ERROR] Patient not found!\n" << endl;
            }
        } else if (ac == 4) {
            printTitle("View Appointment Details");
            cout << "Enter Appointment ID: "; cin >> id;
            sys.displayAppointmentDetails(id);
            cout << endl;
        } else if (ac == 5) {
            printTitle("Edit Appointment");
            cout << "Enter Appointment ID to edit: "; cin >> id;
            if (sys.appointmentExists(id)) {
                sys.editAppointment(id);
            } else {
                cout << "\n[ERROR] Appointment not found!\n" << endl;
            }
        } else if (ac == 6) {
            printTitle("Cancel Appointment");
            cout << "Enter Appointment ID to cancel: "; cin >> id;
            if (sys.appointmentExists(id)) {
                sys.displayAppointmentDetails(id);
                cout << "\nConfirm cancellation? (y/n): ";
                char confirm; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    sys.cancelAppointmentByID(id);
                } else {
                    cout << "\nOperation cancelled\n" << endl;
                }
            } else {
                cout << "\n[ERROR] Appointment not found!\n" << endl;
            }
        } else if (ac == 7) {
            printTitle("Mark Appointment as Completed");
            cout << "Enter Appointment ID: "; cin >> id;
            if (sys.appointmentExists(id)) {
                sys.displayAppointmentDetails(id);
                cout << "\nConfirm mark as completed? (y/n): ";
                char confirm; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    sys.markAppointmentCompleteByID(id);
                } else {
                    cout << "\nOperation cancelled\n" << endl;
                }
            } else {
                cout << "\n[ERROR] Appointment not found!\n" << endl;
            }
        } else if (ac == 8) {
            printTitle("Delete Appointment Record");
            cout << "[WARNING] This will permanently delete the appointment record!" << endl;
            cout << "Enter Appointment ID to delete: "; cin >> id;
            if (sys.appointmentExists(id)) {
                sys.displayAppointmentDetails(id);
                cout << "\nConfirm deletion? (y/n): ";
                char confirm; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    sys.deleteAppointment(id);
                } else {
                    cout << "\nOperation cancelled\n" << endl;
                }
            } else {
                cout << "\n[ERROR] Appointment not found!\n" << endl;
            }
        } else if (ac == 9) {
            sys.sortAppointmentsByDate();
            cout << "\n[SUCCESS] Sorting completed!\n" << endl;
        } else if (ac != 0) {
            cout << "\n[ERROR] Invalid option, please try again!\n" << endl;
        }
    } while (ac != 0);
}

void adminMenu(HospitalSystem& system) {
    int c;
    do {
        printTitle("Administrator Console");
        cout << "  [1] Doctor Management" << endl;
        cout << "  [2] Patient Management" << endl;
        cout << "  [3] Appointment Management" << endl;
        cout << "  [4] System Statistics" << endl;
        cout << "  [5] View Activity Logs" << endl;
        cout << "  [0] Logout" << endl;
        printSeparator();
        cout << "Please select an option: ";
        cin >> c;
        
        if (c == 1) {
            adminDoctorMgmt(system);
        } else if (c == 2) {
            adminPatientMgmt(system);
        } else if (c == 3) {
            adminAppointmentMgmt(system);
        } else if (c == 4) {
            printTitle("System Statistics");
            system.displayStatistics();
            cout << endl;
        } else if (c == 5) {
            system.displaySystemLogs();
        } else if (c != 0) {
            cout << "\n[ERROR] Invalid option, please try again!\n" << endl;
        }
    } while (c != 0);
    cout << "\n[SUCCESS] Logged out from administrator account\n" << endl;
}

void doctorMenu(HospitalSystem& system, Doctor* doc) {
    int c;
    do {
        printTitle("Doctor Workspace - " + doc->getName());
        cout << "  Doctor ID: " << doc->getDoctorID() << endl;
        cout << "  Specialization: " << doc->getSpecialization() << endl;
        cout << "  Room: " << doc->getRoom() << endl;
        printSeparator();
        cout << "  [1] My All Appointments" << endl;
        cout << "  [2] Today's Schedule" << endl;
        cout << "  [3] Mark Appointment as Completed" << endl;
        cout << "  [4] Manage Unavailable Dates" << endl;
        cout << "  [5] Change Password" << endl;
        cout << "  [0] Logout" << endl;
        printSeparator();
        cout << "Please select an option: ";
        cin >> c;
        
        if (c == 1) {
            printTitle("My All Appointments");
            cout << left << setw(10) << "Appt ID" << setw(12) << "Doctor ID" 
                 << setw(15) << "Patient ID" << setw(15) << "Date" 
                 << setw(10) << "Time" << setw(15) << "Status" << endl;
            cout << string(74, '-') << endl;
            system.searchAppointmentByDoctorID(doc->getDoctorID());
            cout << endl;
        } else if (c == 2) {
            printTitle("Today's Schedule");
            string today = system.getCurrentDate();
            cout << "Date: " << today << endl;
            cout << left << setw(10) << "Appt ID" << setw(12) << "Doctor ID" 
                 << setw(12) << "Patient ID" << setw(15) << "Date" 
                 << setw(10) << "Time" << setw(15) << "Status" << endl;
            cout << string(74, '-') << endl;
            system.viewTodayScheduleByDoctorID(doc->getDoctorID());
            cout << endl;
        } else if (c == 3) {
            printTitle("Mark Appointment as Completed");
            cout << "Enter Appointment ID: "; 
            string id; cin >> id;
            if (system.appointmentExists(id)) {
                cout << "\nAppointment Details:" << endl;
                system.displayAppointmentDetails(id);
                cout << "\nConfirm mark as completed? (y/n): ";
                char confirm; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    system.markAppointmentCompleteByID(id);
                } else {
                    cout << "\nOperation cancelled\n" << endl;
                }
            } else {
                cout << "\n[ERROR] Appointment not found!\n" << endl;
            }
        } else if (c == 4) {
            int availChoice;
            do {
                printTitle("Manage Unavailable Dates");
                cout << "Current Unavailable Dates:" << endl;
                doc->displayUnavailableDates();
                cout << "\n  [1] Add Unavailable Date" << endl;
                cout << "  [2] Remove Unavailable Date" << endl;
                cout << "  [0] Back" << endl;
                printSeparator();
                cout << "Please select an option: ";
                cin >> availChoice;
                
                if (availChoice == 1) {
                    string d;
                    cout << "\nEnter unavailable date (YYYY-MM-DD): "; cin >> d;
                    if (system.isValidDate(d)) {
                        doc->addUnavailableDate(d);
                        cout << "\n[SUCCESS] Date added successfully\n" << endl;
                    } else {
                        cout << "\n[ERROR] Invalid date format! Please use YYYY-MM-DD format\n" << endl;
                    }
                } else if (availChoice == 2) {
                    string d;
                    cout << "\nEnter date to remove (YYYY-MM-DD): "; cin >> d;
                    doc->removeUnavailableDate(d);
                    cout << "\n[SUCCESS] Date removed successfully\n" << endl;
                } else if (availChoice != 0) {
                    cout << "\n[ERROR] Invalid option, please try again!\n" << endl;
                }
            } while (availChoice != 0);
        } else if (c == 5) {
            printTitle("Change Password");
            if (system.changeDoctorPassword(doc)) {
                cout << "\n[SUCCESS] Password changed successfully!\n" << endl;
            } else {
                cout << "\n[ERROR] Password change failed! Passwords do not match!\n" << endl;
            }
        } else if (c != 0) {
            cout << "\n[ERROR] Invalid option, please try again!\n" << endl;
        }
    } while (c != 0);
    cout << "\n[SUCCESS] Logged out from doctor account\n" << endl;
}

void patientMenu(HospitalSystem& system, Patient* pat) {
    int c;
    do {
        printTitle("Patient Service Desk - " + pat->getName());
        cout << "  Patient ID: " << pat->getPatientID() << endl;
        cout << "  Name: " << pat->getName() << endl;
        cout << "  Age: " << pat->getAge() << endl;
        cout << "  Phone: " << pat->getPhone() << endl;
        printSeparator();
        cout << "  [1] Book Appointment" << endl;
        cout << "  [2] My Appointments" << endl;
        cout << "  [3] Cancel Appointment" << endl;
        cout << "  [4] View All Doctors" << endl;
        cout << "  [5] Search Doctors by Specialization" << endl;
        cout << "  [6] Change Password" << endl;
        cout << "  [0] Logout" << endl;
        printSeparator();
        cout << "Please select an option: ";
        cin >> c;
        
        if (c == 1) {
            printTitle("Book Appointment");
            cout << "Please select a doctor:" << endl;
            cout << left << setw(10) << "ID" << setw(25) << "Name" 
                 << setw(25) << "Specialization" << endl;
            cout << string(60, '-') << endl;
            system.displayDoctorsBrief();
            cout << "\nEnter Doctor ID: "; 
            string docID; cin >> docID;
            
            if (system.doctorExists(docID)) {
                Doctor* doc = system.searchDoctorByID(docID);
                cout << "\nSelected Doctor: " << doc->getName() 
                     << " - " << doc->getSpecialization() << endl;
                cout << "Enter appointment date (YYYY-MM-DD): "; 
                string date; cin >> date;
                
                if (system.isValidDate(date)) {
                    if (doc->isAvailableOnDate(date)) {
                        string slots = system.showDoctorScheduleForDate(docID, date);
                        if (slots != "0") {
                            int s;
                            cout << "\nSelect time slot number: "; cin >> s;
                            string time = system.getTimeBySlotNumber(docID, date, s);
                            if (!time.empty()) {
                                // Check for time conflict
                                if (!system.hasTimeConflict(docID, date, time)) {
                                    string apptID = system.generateAppointmentID();
                                    Appointment a(apptID, docID, pat->getPatientID(), date, time);
                                    system.addAppointment(a);
                                    cout << "\n[SUCCESS] Appointment booked successfully!" << endl;
                                    cout << "Appointment ID: " << apptID << endl;
                                    cout << "Doctor: " << doc->getName() << endl;
                                    cout << "Date: " << date << endl;
                                    cout << "Time: " << time << endl;
                                    cout << endl;
                                } else {
                                    cout << "\n[ERROR] Time slot is already booked!\n" << endl;
                                }
                            } else {
                                cout << "\n[ERROR] Invalid time slot number!\n" << endl;
                            }
                        } else {
                            cout << "\n[ERROR] No available time slots on this date!\n" << endl;
                        }
                    } else {
                        cout << "\n[ERROR] Doctor is unavailable on this date!\n" << endl;
                        doc->displayUnavailableDates();
                    }
                } else {
                    cout << "\n[ERROR] Invalid date format! Please use YYYY-MM-DD format\n" << endl;
                }
            } else {
                cout << "\n[ERROR] Invalid Doctor ID!\n" << endl;
            }
        } else if (c == 2) {
            printTitle("My Appointments");
            cout << left << setw(10) << "Appt ID" << setw(12) << "Doctor ID" 
                 << setw(15) << "Patient ID" << setw(15) << "Date" 
                 << setw(10) << "Time" << setw(15) << "Status" << endl;
            cout << string(74, '-') << endl;
            system.searchAppointmentByPatientID(pat->getPatientID());
            cout << endl;
        } else if (c == 3) {
            printTitle("Cancel Appointment");
            cout << "My Appointments:" << endl;
            cout << left << setw(10) << "Appt ID" << setw(12) << "Doctor ID" 
                 << setw(12) << "Patient ID" << setw(15) << "Date" 
                 << setw(10) << "Time" << setw(15) << "Status" << endl;
            cout << string(74, '-') << endl;
            system.searchAppointmentByPatientID(pat->getPatientID());
            cout << "\nEnter Appointment ID to cancel: "; 
            string id; cin >> id;
            
            if (system.appointmentExists(id)) {
                cout << "\nAppointment Details:" << endl;
                system.displayAppointmentDetails(id);
                cout << "\nConfirm cancellation? (y/n): ";
                char confirm; cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    system.cancelAppointmentByID(id);
                } else {
                    cout << "\nOperation cancelled\n" << endl;
                }
            } else {
                cout << "\n[ERROR] Appointment not found!\n" << endl;
            }
        } else if (c == 4) {
            printTitle("All Doctors List");
            cout << left << setw(10) << "ID" << setw(25) << "Name"
                 << setw(25) << "Specialization" << endl;
            cout << string(65, '-') << endl;
            system.displayDoctorsBrief();
            cout << endl;
        } else if (c == 5) {
            printTitle("Search Doctors by Specialization");
            system.displayAvailableSpecializations();
            cout << "\nEnter Specialization name or number: ";
            cin.ignore();
            string specInput;
            getline(cin, specInput);
            string spec = specInput;
            // If user entered a number, map to the displayed specialization list
            LinkedList<string> specs;
            system.getAllSpecializations(specs);
            bool isNumber = !specInput.empty() && all_of(specInput.begin(), specInput.end(), [](char ch){ return isdigit((unsigned char)ch); });
            if (isNumber) {
                int idx = stoi(specInput);
                Node<string>* node = specs.getHead();
                int i = 1;
                while (node && i < idx) { node = node->next; ++i; }
                if (node) spec = node->data;
            }
            cout << "\nSearch Results:" << endl;
            cout << left << setw(10) << "ID" << setw(25) << "Name"
                 << setw(25) << "Specialization" << endl;
            cout << string(65, '-') << endl;
            system.searchDoctorBySpecializationForPatient(spec);
            cout << endl;
        } else if (c == 6) {
            printTitle("Change Password");
            if (system.changePatientPassword(pat)) {
                cout << "\n[SUCCESS] Password changed successfully!\n" << endl;
            } else {
                cout << "\n[ERROR] Password change failed! Passwords do not match!\n" << endl;
            }
        } else if (c != 0) {
            cout << "\n[ERROR] Invalid option, please try again!\n" << endl;
        }
    } while (c != 0);
    cout << "\n[SUCCESS] Logged out from patient account\n" << endl;
}
