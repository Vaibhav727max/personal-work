cmake_minimum_required(VERSION 3.16)
project(CppOOPAssignments LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROGRAMS
    01_basics
    02_bank_account
    03_rule_of_five
    04_university_inheritance
    05_runtime_polymorphism
    06_compile_time_polymorphism
    07_template_stack
    08_stl_student_marks
    09_banking_exceptions
)
foreach(program IN LISTS PROGRAMS)
    add_executable(${program} ${program}.cpp)
endforeach()
