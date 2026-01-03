#include <iostream>
#include <iomanip>
#include "HospitalSystem.h"
#include "MenuFunctions.h"

using namespace std;

int main() {
    HospitalSystem system;
    int userType;
    string id, pass;

    do {
        printTitle("Hospital Appointment Management System");
        cout << "  [1] Administrator Login" << endl;
        cout << "  [2] Doctor Login" << endl;
        cout << "  [3] Patient Login" << endl;
        cout << "  [4] Patient Registration" << endl;
        cout << "  [0] Exit System" << endl;
        printSeparator();
        cout << "Please select an option: ";
        cin >> userType;

        switch(userType) {
            case 1: {
                printTitle("Administrator Login");
                cout << "Enter administrator password: ";
                cin >> pass;
                if(system.authenticateAdmin(pass)) {
                    cout << "\n[SUCCESS] Login successful!\n" << endl;
                    adminMenu(system);
                } else {
                    cout << "\n[ERROR] Wrong password!\n" << endl;
                }
                break;
            }
            case 2: {
                printTitle("Doctor Login");
                cout << "Enter Doctor ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> pass;
                if(Doctor* d = system.authenticateDoctor(id, pass)) {
                    cout << "\n[SUCCESS] Login successful! Welcome Dr. " << d->getName() << "!\n" << endl;
                    doctorMenu(system, d);
                } else {
                    cout << "\n[ERROR] Login failed! Invalid ID or password!\n" << endl;
                }
                break;
            }
            case 3: {
                printTitle("Patient Login");
                cout << "Enter Patient ID: ";
                cin >> id;
                cout << "Enter Password: ";
                cin >> pass;
                if(Patient* p = system.authenticatePatient(id, pass)) {
                    cout << "\n[SUCCESS] Login successful! Welcome " << p->getName() << "!\n" << endl;
                    patientMenu(system, p);
                } else {
                    cout << "\n[ERROR] Login failed! Invalid ID or password!\n" << endl;
                }
                break;
            }
            case 4: {
                printTitle("Patient Registration");
                string name, phone; 
                int age;
                cout << "Please fill in registration information:" << endl;
                cout << "  Patient ID: "; cin >> id;
                
                if(system.patientExists(id)) {
                    cout << "\n[ERROR] ID already exists! Please use another ID\n" << endl;
                    break;
                }
                
                cout << "  Name: "; cin.ignore(); getline(cin, name);
                cout << "  Age: "; cin >> age;
                
                if(!system.isValidAge(age)) {
                    cout << "\n[ERROR] Invalid age! Please enter a number between 1-150\n" << endl;
                    break;
                }
                
                cout << "  Phone: "; cin.ignore(); getline(cin, phone);
                
                if(!system.isValidPhone(phone)) {
                    cout << "\n[ERROR] Invalid phone number format!\n" << endl;
                    break;
                }
                
                cout << "  Password: "; cin >> pass;
                
                system.addPatient(Patient(id, name, age, phone, pass));
                cout << "\n[SUCCESS] Registration successful! You can now login with your ID and password\n" << endl;
                break;
            }
            case 0:
                printTitle("Thank You for Using Hospital Appointment System");
                cout << "System is exiting..." << endl;
                break;
            default:
                cout << "\n[ERROR] Invalid option, please try again!\n" << endl;
                break;
        }
    } while (userType != 0);

    return 0;
}