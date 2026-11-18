#include <iostream>
#include <string>

class Person { // Base class
protected:
    std::string name;
    int age;

public:
    Person(std::string n, int a) : name(std::move(n)), age(a) {}
    void showPerson() const {
        std::cout << "Name: " << name << "\nAge: " << age << '\n';
    }
};

class Student : public Person { // Single inheritance: Student -> Person
protected:
    int rollNumber;
    std::string department;

public:
    Student(std::string n, int a, int roll, std::string dept)
        : Person(std::move(n), a), rollNumber(roll),
          department(std::move(dept)) {}

    void showStudent() const {
        showPerson();
        std::cout << "Roll number: " << rollNumber
                  << "\nDepartment: " << department << '\n';
    }
};

class GraduateStudent : public Student { // Multilevel: Person -> Student -> GraduateStudent
private:
    std::string researchTopic;

public:
    GraduateStudent(std::string n, int a, int roll, std::string dept,
                    std::string topic)
        : Student(std::move(n), a, roll, std::move(dept)),
          researchTopic(std::move(topic)) {}

    void display() const {
        showStudent();
        std::cout << "Research topic: " << researchTopic << '\n';
    }
};

int main() {
    GraduateStudent student("Neha", 24, 501, "Computer Science",
                            "Machine Learning");
    student.display();
}
