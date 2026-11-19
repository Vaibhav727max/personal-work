# Vehicle Rental Management System

A beginner-friendly C++ console project demonstrating Object-Oriented Programming.

## Features
- Display available and rented vehicles
- Rent a vehicle by entering its ID and rental duration
- Calculate the rental amount
- Return a rented vehicle
- Validate invalid IDs, rental days, and unavailable vehicles

## OOP concepts used
- **Classes and objects:** `Vehicle` and `RentalSystem`
- **Encapsulation:** private data members with public methods
- **Constructors:** initialize vehicle details
- **Abstraction:** rental operations are handled through class methods
- **STL vector:** stores the vehicle objects

## Requirements
- C++ compiler (g++, MinGW, or a compatible IDE)

## Compile and run

### Windows (MinGW)
```bash
g++ main.cpp -o rental
rental.exe
```

### macOS / Linux
```bash
g++ -std=c++17 main.cpp -o rental
./rental
```

## Note
This is a console-based academic project. Vehicle availability is stored in memory and resets when the program is restarted. It does not use a database or file storage.
