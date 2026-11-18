# C++ Programming and OOP Assignments

Nine standalone C++17 programs covering the requested topics.

## Programs

1. `01_basics.cpp` — data types, constants, variables, operators, input/output.
2. `02_bank_account.cpp` — BankAccount/SavingsAccount hierarchy, private/protected/public access, member functions.
3. `03_rule_of_five.cpp` — default, parameterized, copy and move constructors; copy/move assignment; destructor.
4. `04_university_inheritance.cpp` — single and multilevel inheritance: Person → Student → GraduateStudent.
5. `05_runtime_polymorphism.cpp` — abstract Shape and virtual `area()` for Circle, Rectangle and Triangle.
6. `06_compile_time_polymorphism.cpp` — function and operator overloading using Complex.
7. `07_template_stack.cpp` — generic Stack tested with int, float and std::string.
8. `08_stl_student_marks.cpp` — vector, map, unordered_map and sort/find_if/transform on student marks.
9. `09_banking_exceptions.cpp` — custom banking exception hierarchy with specific catches.

## Build all programs

```bash
cmake -S . -B build
cmake --build build
```

Run an executable, for example:

```bash
./build/03_rule_of_five
```

Or compile any single file directly:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic 01_basics.cpp -o basics
./basics
```

## Memory check (Linux with Valgrind)

```bash
g++ -std=c++17 -g -O0 -Wall -Wextra -pedantic 03_rule_of_five.cpp -o rule_of_five
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./rule_of_five
```

A clean run should report **0 bytes in 0 blocks** at exit and no definitely-lost memory. Run Valgrind on a Linux environment where it is installed; this package does not claim a Valgrind run has already been performed.

## GitHub setup and commits

Create an empty GitHub repository named `cpp-oop-assignments`, then from this folder run:

```bash
git init
git add .
git commit -m "Add nine C++ fundamentals, OOP, STL and exception-handling demonstrations"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/cpp-oop-assignments.git
git push -u origin main
```

Replace `YOUR-USERNAME` with your GitHub username. Authenticate with GitHub when Git prompts you. The repository is not pushed automatically by this package.
