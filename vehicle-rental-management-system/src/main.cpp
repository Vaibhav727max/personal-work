#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// ---------------------- Input helpers ------------------------
string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    // '|' is our file delimiter, so remove it from user text
    s.erase(remove(s.begin(), s.end(), '|'), s.end());
    return s;
}

int readInt(const string& prompt, int minVal = 0, int maxVal = 1000000) {
    while (true) {
        try {
            int v = stoi(readLine(prompt));
            if (v >= minVal && v <= maxVal) return v;
        } catch (...) {}
        cout << "  Invalid input. Enter a number between " << minVal
             << " and " << maxVal << ".\n";
    }
}

double readDouble(const string& prompt, double minVal = 0) {
    while (true) {
        try {
            double v = stod(readLine(prompt));
            if (v > minVal) return v;
        } catch (...) {}
        cout << "  Invalid input. Enter a number greater than " << minVal << ".\n";
    }
}

// ------------------------- Vehicle ---------------------------
class Vehicle {
protected:
    int id;
    string brand, model;
    double ratePerDay;
    bool available;

public:
    Vehicle(int id, string brand, string model, double rate, bool avail = true)
        : id(id), brand(move(brand)), model(move(model)),
          ratePerDay(rate), available(avail) {}
    virtual ~Vehicle() = default;

    int getId() const { return id; }
    bool isAvailable() const { return available; }
    void setAvailable(bool a) { available = a; }
    string getName() const { return brand + " " + model; }

    virtual string type() const = 0;
    virtual double extraCharges(int days) const = 0;   // type specific
    virtual string extraInfo() const = 0;              // for display
    virtual string extraField() const = 0;             // for file saving

    // 10% discount for rentals of 7+ days
    double calculateCost(int days) const {
        double base = ratePerDay * days;
        if (days >= 7) base *= 0.90;
        return base + extraCharges(days);
    }

    void display() const {
        cout << left << setw(5) << id << setw(8) << type() << setw(24) << getName()
             << setw(10) << fixed << setprecision(2) << ratePerDay
             << setw(12) << (available ? "Available" : "Rented")
             << extraInfo() << "\n";
    }

    string serialize() const {
        ostringstream o;
        o << type() << '|' << id << '|' << brand << '|' << model << '|'
          << ratePerDay << '|' << available << '|' << extraField();
        return o.str();
    }
};

class Car : public Vehicle {
    int seats;
public:
    Car(int id, string b, string m, double r, int seats, bool a = true)
        : Vehicle(id, move(b), move(m), r, a), seats(seats) {}
    string type() const override { return "Car"; }
    double extraCharges(int days) const override { return ratePerDay * days * 0.05; } // insurance 5%
    string extraInfo() const override { return to_string(seats) + " seats (+5% insurance)"; }
    string extraField() const override { return to_string(seats); }
};

class Bike : public Vehicle {
    bool helmet;
public:
    Bike(int id, string b, string m, double r, bool helmet, bool a = true)
        : Vehicle(id, move(b), move(m), r, a), helmet(helmet) {}
    string type() const override { return "Bike"; }
    double extraCharges(int days) const override { return helmet ? 20.0 * days : 0.0; } // helmet fee
    string extraInfo() const override { return helmet ? "Helmet included (+20/day)" : "No helmet"; }
    string extraField() const override { return helmet ? "1" : "0"; }
};

class Truck : public Vehicle {
    double capacityTons;
public:
    Truck(int id, string b, string m, double r, double cap, bool a = true)
        : Vehicle(id, move(b), move(m), r, a), capacityTons(cap) {}
    string type() const override { return "Truck"; }
    double extraCharges(int) const override { return 500.0; } // flat permit fee
    string extraInfo() const override {
        ostringstream o; o << capacityTons << " tons (+500 permit)"; return o.str();
    }
    string extraField() const override { return to_string(capacityTons); }
};

// ------------------------ Customer ---------------------------
struct Customer {
    int id;
    string name, phone;
    string serialize() const { return to_string(id) + "|" + name + "|" + phone; }
};

// ------------------------- Rental ----------------------------
struct Rental {
    int id, vehicleId, customerId, days;
    double total;
    bool active;
    string serialize() const {
        ostringstream o;
        o << id << '|' << vehicleId << '|' << customerId << '|' << days << '|'
          << total << '|' << active;
        return o.str();
    }
};

// ---------------------- Rental System ------------------------
class RentalSystem {
    vector<unique_ptr<Vehicle>> vehicles;
    vector<Customer> customers;
    vector<Rental> rentals;
    int nextVehicleId = 1, nextCustomerId = 1, nextRentalId = 1;

    const string VFILE = "vehicles.txt", CFILE = "customers.txt", RFILE = "rentals.txt";

    static vector<string> split(const string& line) {
        vector<string> parts;
        string item;
        stringstream ss(line);
        while (getline(ss, item, '|')) parts.push_back(item);
        return parts;
    }

    Vehicle* findVehicle(int id) {
        for (auto& v : vehicles) if (v->getId() == id) return v.get();
        return nullptr;
    }
    Customer* findCustomer(int id) {
        for (auto& c : customers) if (c.id == id) return &c;
        return nullptr;
    }
    Rental* findRental(int id) {
        for (auto& r : rentals) if (r.id == id) return &r;
        return nullptr;
    }

    void vehicleHeader() const {
        cout << left << setw(5) << "ID" << setw(8) << "Type" << setw(24) << "Vehicle"
             << setw(10) << "Rate/Day" << setw(12) << "Status" << "Details\n"
             << string(80, '-') << "\n";
    }

public:
    // ---------------- Persistence ----------------
    void load() {
        string line;
        ifstream vf(VFILE);
        while (getline(vf, line)) {
            auto p = split(line);
            if (p.size() < 7) continue;
            try {
                int id = stoi(p[1]); double rate = stod(p[4]); bool av = p[5] == "1";
                if (p[0] == "Car")
                    vehicles.push_back(make_unique<Car>(id, p[2], p[3], rate, stoi(p[6]), av));
                else if (p[0] == "Bike")
                    vehicles.push_back(make_unique<Bike>(id, p[2], p[3], rate, p[6] == "1", av));
                else if (p[0] == "Truck")
                    vehicles.push_back(make_unique<Truck>(id, p[2], p[3], rate, stod(p[6]), av));
                nextVehicleId = max(nextVehicleId, id + 1);
            } catch (...) {}
        }
        ifstream cf(CFILE);
        while (getline(cf, line)) {
            auto p = split(line);
            if (p.size() < 3) continue;
            try {
                customers.push_back({stoi(p[0]), p[1], p[2]});
                nextCustomerId = max(nextCustomerId, stoi(p[0]) + 1);
            } catch (...) {}
        }
        ifstream rf(RFILE);
        while (getline(rf, line)) {
            auto p = split(line);
            if (p.size() < 6) continue;
            try {
                rentals.push_back({stoi(p[0]), stoi(p[1]), stoi(p[2]), stoi(p[3]),
                                   stod(p[4]), p[5] == "1"});
                nextRentalId = max(nextRentalId, stoi(p[0]) + 1);
            } catch (...) {}
        }
    }

    void save() const {
        ofstream vf(VFILE), cf(CFILE), rf(RFILE);
        for (auto& v : vehicles) vf << v->serialize() << "\n";
        for (auto& c : customers) cf << c.serialize() << "\n";
        for (auto& r : rentals) rf << r.serialize() << "\n";
        cout << "Data saved.\n";
    }

    // ---------------- Vehicles ----------------
    void addVehicle() {
        cout << "\n1. Car   2. Bike   3. Truck\n";
        int t = readInt("Choose type: ", 1, 3);
        string brand = readLine("Brand: ");
        string model = readLine("Model: ");
        double rate = readDouble("Rate per day: ");
        int id = nextVehicleId++;

        if (t == 1) {
            int seats = readInt("Seats: ", 1, 60);
            vehicles.push_back(make_unique<Car>(id, brand, model, rate, seats));
        } else if (t == 2) {
            int h = readInt("Helmet included? (1=yes, 0=no): ", 0, 1);
            vehicles.push_back(make_unique<Bike>(id, brand, model, rate, h == 1));
        } else {
            double cap = readDouble("Capacity (tons): ");
            vehicles.push_back(make_unique<Truck>(id, brand, model, rate, cap));
        }
        cout << "Vehicle added with ID " << id << ".\n";
    }

    void showVehicles(bool onlyAvailable) const {
        cout << "\n";
        vehicleHeader();
        int count = 0;
        for (auto& v : vehicles) {
            if (onlyAvailable && !v->isAvailable()) continue;
            v->display();
            count++;
        }
        if (!count) cout << "No vehicles to show.\n";
    }

    void searchByType() const {
        string t = readLine("Enter type (Car/Bike/Truck): ");
        transform(t.begin(), t.end(), t.begin(), ::tolower);
        cout << "\n";
        vehicleHeader();
        int count = 0;
        for (auto& v : vehicles) {
            string vt = v->type();
            transform(vt.begin(), vt.end(), vt.begin(), ::tolower);
            if (vt == t) { v->display(); count++; }
        }
        if (!count) cout << "No matching vehicles.\n";
    }

    void removeVehicle() {
        int id = readInt("Vehicle ID to remove: ");
        auto it = find_if(vehicles.begin(), vehicles.end(),
                          [id](const unique_ptr<Vehicle>& v) { return v->getId() == id; });
        if (it == vehicles.end()) { cout << "Vehicle not found.\n"; return; }
        if (!(*it)->isAvailable()) { cout << "Cannot remove: vehicle is currently rented.\n"; return; }
        vehicles.erase(it);
        cout << "Vehicle removed.\n";
    }

    // ---------------- Customers ----------------
    void addCustomer() {
        string name = readLine("Customer name: ");
        string phone = readLine("Phone number: ");
        if (name.empty() || phone.empty()) { cout << "Name and phone are required.\n"; return; }
        customers.push_back({nextCustomerId, name, phone});
        cout << "Customer registered with ID " << nextCustomerId++ << ".\n";
    }

    void showCustomers() const {
        cout << "\n" << left << setw(6) << "ID" << setw(24) << "Name" << "Phone\n"
             << string(50, '-') << "\n";
        if (customers.empty()) cout << "No customers registered.\n";
        for (auto& c : customers)
            cout << left << setw(6) << c.id << setw(24) << c.name << c.phone << "\n";
    }

    // ---------------- Rentals ----------------
    void rentVehicle() {
        showVehicles(true);
        int vid = readInt("\nVehicle ID to rent: ");
        Vehicle* v = findVehicle(vid);
        if (!v) { cout << "Vehicle not found.\n"; return; }
        if (!v->isAvailable()) { cout << "Vehicle is already rented.\n"; return; }

        int cid = readInt("Customer ID: ");
        if (!findCustomer(cid)) { cout << "Customer not found. Register first.\n"; return; }

        int days = readInt("Number of days: ", 1, 365);
        double cost = v->calculateCost(days);

        cout << fixed << setprecision(2)
             << "\nEstimated cost for " << days << " day(s): " << cost << "\n";
        if (readInt("Confirm rental? (1=yes, 0=no): ", 0, 1) == 0) {
            cout << "Cancelled.\n"; return;
        }

        v->setAvailable(false);
        rentals.push_back({nextRentalId, vid, cid, days, cost, true});
        cout << "Rental confirmed. Rental ID: " << nextRentalId++ << "\n";
    }

    void returnVehicle() {
        int rid = readInt("Rental ID: ");
        Rental* r = findRental(rid);
        if (!r) { cout << "Rental not found.\n"; return; }
        if (!r->active) { cout << "This rental is already closed.\n"; return; }

        Vehicle* v = findVehicle(r->vehicleId);
        int actual = readInt("Actual days used: ", 1, 365);

        double cost = v ? v->calculateCost(actual) : r->total;
        // Late return penalty: 25% extra per day beyond booked days
        if (v && actual > r->days) {
            double perDay = v->calculateCost(1);
            cost += perDay * 0.25 * (actual - r->days);
            cout << "Late return penalty applied.\n";
        }
        r->total = cost;
        r->days = actual;
        r->active = false;
        if (v) v->setAvailable(true);

        cout << fixed << setprecision(2) << "Vehicle returned. Final bill: " << cost << "\n";
    }

    void showRentals() const {
        cout << "\n" << left << setw(6) << "ID" << setw(22) << "Vehicle" << setw(20)
             << "Customer" << setw(6) << "Days" << setw(12) << "Total" << "Status\n"
             << string(76, '-') << "\n";
        if (rentals.empty()) cout << "No rental records.\n";
        for (auto& r : rentals) {
            string vn = "(removed)", cn = "(unknown)";
            for (auto& v : vehicles) if (v->getId() == r.vehicleId) vn = v->getName();
            for (auto& c : customers) if (c.id == r.customerId) cn = c.name;
            cout << left << setw(6) << r.id << setw(22) << vn << setw(20) << cn
                 << setw(6) << r.days << setw(12) << fixed << setprecision(2) << r.total
                 << (r.active ? "Active" : "Returned") << "\n";
        }
    }

    void revenueReport() const {
        double total = 0; int done = 0, active = 0;
        for (auto& r : rentals) {
            if (r.active) active++;
            else { done++; total += r.total; }
        }
        cout << fixed << setprecision(2)
             << "\n--- Summary ---\n"
             << "Total vehicles   : " << vehicles.size() << "\n"
             << "Total customers  : " << customers.size() << "\n"
             << "Active rentals   : " << active << "\n"
             << "Completed rentals: " << done << "\n"
             << "Revenue earned   : " << total << "\n";
    }
};

// --------------------------- Main ----------------------------
void menu() {
    cout << "\n========= VEHICLE RENTAL MANAGEMENT SYSTEM =========\n"
         << " 1. Add vehicle            7. Rent a vehicle\n"
         << " 2. View all vehicles      8. Return a vehicle\n"
         << " 3. View available         9. View rentals\n"
         << " 4. Search by type        10. Revenue summary\n"
         << " 5. Remove vehicle        11. Register customer\n"
         << " 6. View customers         0. Save & exit\n"
         << "=====================================================\n";
}

int main() {
    RentalSystem sys;
    sys.load();

    while (true) {
        menu();
        int ch = readInt("Enter choice: ", 0, 11);
        switch (ch) {
            case 1:  sys.addVehicle(); break;
            case 2:  sys.showVehicles(false); break;
            case 3:  sys.showVehicles(true); break;
            case 4:  sys.searchByType(); break;
            case 5:  sys.removeVehicle(); break;
            case 6:  sys.showCustomers(); break;
            case 7:  sys.rentVehicle(); break;
            case 8:  sys.returnVehicle(); break;
            case 9:  sys.showRentals(); break;
            case 10: sys.revenueReport(); break;
            case 11: sys.addCustomer(); break;
            case 0:
                sys.save();
                cout << "Goodbye!\n";
                return 0;
        }
    }
}