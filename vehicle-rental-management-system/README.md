# Vehicle Rental Management System

A menu-driven, console-based Vehicle Rental Management System written in **C++**. It supports cars, bikes, and trucks, customer registration, vehicle rentals and returns, and a basic revenue report. Records are stored locally in text files so data can be loaded again when the program restarts.

## Features

- Add, view, search, and remove vehicles
- Support for **Car**, **Bike**, and **Truck** vehicle types
- Register and view customers
- Rent available vehicles and process returns
- Calculate rental charges, including type-specific fees
- Apply a 10% discount for rentals of 7 days or more
- Apply a late-return penalty when a vehicle is returned after the booked period
- View rental records and revenue summary
- Save and load data using `vehicles.txt`, `customers.txt`, and `rentals.txt`

## Requirements

- A C++ compiler supporting **C++17** (for example, GCC or Clang)
- Terminal / command prompt

## Build and run

### Linux / macOS

From the repository root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o vehicle_rental
./vehicle_rental
```

### Windows (MinGW-w64)

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o vehicle_rental.exe
vehicle_rental.exe
```

### Using CMake

```bash
cmake -S . -B build
cmake --build build
```

Run the executable produced in the `build` directory.

## Usage

1. Start the program.
2. Register a customer using menu option **11**.
3. Add a vehicle using menu option **1**.
4. Rent a vehicle using option **7**.
5. Return it using option **8**.
6. View rental records or the revenue summary using options **9** and **10**.
7. Select **0** to save all records and exit.

The program creates its data files in the **current working directory** when you save.

## Data files

The application uses pipe-delimited text files:

- `vehicles.txt` — vehicle details and availability
- `customers.txt` — customer details
- `rentals.txt` — rental details and status

These files are generated at runtime and are excluded from Git by `.gitignore`, so personal/test records are not accidentally committed.

## Project structure

```text
vehicle-rental-management-system/
├── src/
│   └── main.cpp
├── .gitignore
├── CMakeLists.txt
└── README.md
```

## Notes

- Vehicle rental pricing is calculated by the rules implemented in `src/main.cpp`.
- This is a console-based educational project; it does not use a database or graphical interface.
