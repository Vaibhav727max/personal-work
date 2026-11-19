#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Vehicle {
private:
    int id;
    string brand, model;
    double rentPerDay;
    bool available;
public:
    Vehicle(int i, string b, string m, double r)
        : id(i), brand(b), model(m), rentPerDay(r), available(true) {}
    int getId() const { return id; }
    double getRate() const { return rentPerDay; }
    bool isAvailable() const { return available; }
    void setAvailable(bool status) { available = status; }
    void display() const {
        cout << left << setw(6) << id << setw(15) << brand
             << setw(15) << model << "Rs." << rentPerDay
             << (available ? "  Available" : "  Rented") << '\n';
    }
};

class RentalSystem {
private:
    vector<Vehicle> vehicles;
public:
    RentalSystem() {
        vehicles.emplace_back(101, "Toyota", "Innova", 2500);
        vehicles.emplace_back(102, "Honda", "City", 1800);
        vehicles.emplace_back(103, "Hyundai", "Creta", 2200);
    }
    void showVehicles() const {
        cout << "\nID    Brand          Model          Daily Rent\n";
        for (const auto &v : vehicles) v.display();
    }
    void rentVehicle() {
        int id, days;
        cout << "Enter vehicle ID: ";
        cin >> id;
        cout << "Number of rental days: ";
        cin >> days;
        if (days <= 0) {
            cout << "Days must be greater than zero.\n";
            return;
        }
        for (auto &v : vehicles) {
            if (v.getId() == id) {
                if (!v.isAvailable()) {
                    cout << "Vehicle is already rented.\n";
                    return;
                }
                double total = v.getRate() * days;
                v.setAvailable(false);
                cout << "Vehicle rented successfully!\n";
                cout << "Total amount: Rs." << total << '\n';
                return;
            }
        }
        cout << "Invalid vehicle ID.\n";
    }
    void returnVehicle() {
        int id;
        cout << "Enter vehicle ID to return: ";
        cin >> id;
        for (auto &v : vehicles) {
            if (v.getId() == id) {
                if (v.isAvailable()) {
                    cout << "This vehicle was not rented.\n";
                    return;
                }
                v.setAvailable(true);
                cout << "Vehicle returned successfully!\n";
                return;
            }
        }
        cout << "Invalid vehicle ID.\n";
    }
    void menu() {
        int choice;
        do {
            cout << "\n===== VEHICLE RENTAL MANAGEMENT =====\n";
            cout << "1. View vehicles\n2. Rent a vehicle\n";
            cout << "3. Return a vehicle\n4. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;
            switch (choice) {
                case 1: showVehicles(); break;
                case 2: rentVehicle(); break;
                case 3: returnVehicle(); break;
                case 4: cout << "Thank you!\n"; break;
                default: cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 4);
    }
};

int main() {
    RentalSystem system;
    cout << "Welcome to Vehicle Rental Management System\n";
    system.menu();
    return 0;
}
