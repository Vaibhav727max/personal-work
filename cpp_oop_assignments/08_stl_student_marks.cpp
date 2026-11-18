#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

struct Student {
    int roll;
    std::string name;
    double marks;
};

int main() {
    std::vector<Student> students = {
        {104, "Asha", 86.5},
        {101, "Rohan", 72.0},
        {103, "Meera", 94.0},
        {102, "Kabir", 65.5},
        {105, "Ira", 78.0}
    };

    // map: roll number -> marks (keys are kept in sorted order)
    std::map<int, double> marksByRoll;
    // unordered_map: student name -> roll number
    std::unordered_map<std::string, int> rollByName;

    for (const auto& student : students) {
        marksByRoll[student.roll] = student.marks;
        rollByName[student.name] = student.roll;
    }

    // sort students by marks, highest first
    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  return a.marks > b.marks;
              });

    // find_if: locate the first student scoring at least 90
    auto topper = std::find_if(students.begin(), students.end(),
                               [](const Student& s) {
                                   return s.marks >= 90.0;
                               });

    // transform: convert marks to percentage strings for display
    std::vector<std::string> summaries(students.size());
    std::transform(students.begin(), students.end(), summaries.begin(),
                   [](const Student& s) {
                       return s.name + ": " + std::to_string(s.marks);
                   });

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Students sorted by marks:\n";
    for (const auto& s : students)
        std::cout << s.roll << " | " << s.name << " | " << s.marks << '\n';

    if (topper != students.end())
        std::cout << "\nFirst student scoring at least 90: "
                  << topper->name << " (" << topper->marks << ")\n";

    std::cout << "\nMarks indexed by roll number (std::map):\n";
    for (const auto& entry : marksByRoll)
        std::cout << entry.first << " -> " << entry.second << '\n';

    std::cout << "\nName lookup (std::unordered_map):\n";
    const std::string lookupName = "Meera";
    auto found = rollByName.find(lookupName);
    if (found != rollByName.end())
        std::cout << lookupName << " has roll number " << found->second << '\n';

    std::cout << "\nTransformed summaries:\n";
    for (const auto& line : summaries)
        std::cout << line << '\n';
}
