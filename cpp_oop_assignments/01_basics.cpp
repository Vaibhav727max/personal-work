#include <iostream>
#include <iomanip>
#include <string>

int main() {
    // Fundamental data types, variables, and a constant
    int age = 20;
    double cgpa = 8.75;
    char grade = 'A';
    bool enrolled = true;
    std::string name = "Aarav";
    constexpr double PI = 3.141592653589793;

    // Operators
    int nextAge = age + 1;
    double radius = 2.0;
    double circumference = 2 * PI * radius;
    bool eligible = (age >= 18) && enrolled;

    // Input and output
    std::cout << "Enter your city: ";
    std::string city;
    std::getline(std::cin, city);

    std::cout << std::boolalpha << std::fixed << std::setprecision(2);
    std::cout << "\n--- Student Information ---\n"
              << "Name: " << name << "\nAge: " << age
              << "\nNext year age: " << nextAge
              << "\nCGPA: " << cgpa << "\nGrade: " << grade
              << "\nEnrolled: " << enrolled
              << "\nEligible: " << eligible
              << "\nCity: " << city
              << "\nCircle circumference: " << circumference << '\n';
    return 0;
}
