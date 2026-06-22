# Car Fleet Management System

A desktop fleet management application built with C++, featuring a Qt GUI and SQLite database. Users can manage vehicles, register clients, create rental transactions and log maintenance records.

## Screenshots

<img width="400" height="270" alt="image" src="https://github.com/user-attachments/assets/28a5349b-67b5-4f4f-adb7-5d4cb4e79cbd" />
<img width="400" height="270" alt="image" src="https://github.com/user-attachments/assets/cdd45652-3be1-4dea-b75d-722050b9cea5" />
<img width="400" height="270" alt="image" src="https://github.com/user-attachments/assets/6099b951-1aec-4de8-9305-9e07c2a8214a" />
<img width="400" height="270" alt="image" src="https://github.com/user-attachments/assets/00977306-c5dc-400f-ab4c-c4428a439df4" />
<img width="400" height="270" alt="image" src="https://github.com/user-attachments/assets/4c200bcb-3abc-4b5f-adbb-a023355d077a" />


## Project Structure

```
ParcAuto/
|- models/   - Data classes (Vehicle, Car, Truck, Motorcycle, Client, Transaction, Maintenance)
|- database/ - Database access layer (DatabaseManager)
|- ui/       - Qt windows
|- main.cpp     
```

## Tech Stack

- C++17
- Qt 6 (Widgets + SQL)
- SQLite
- CMake

## Features

- Add, edit and delete three vehicle types: Cars, Trucks and Motorcycles
- Register individual clients (CNP + driver license) and corporate clients (CUI + company)
- Create rental transactions with calculated cost based on daily rate and rental period
- Mark rentals as Completed or Cancelled
- Log maintenance and repair records
- View full maintenance history for any vehicle

## Installing

### Requirements

- Qt 6.x (with MinGW 64-bit on Windows)
- CMake 3.20+
- CLion 

### Steps

1. Clone the repository

```bash
git clone https://github.com/voprescu/CompanyFleetApp.git
```

2. Open the project in CLion

3. Set the Qt path in CLion CMake options

Go to `File - Settings - Build, Execution, Deployment - CMake` and add to CMake options:

```
-DCMAKE_PREFIX_PATH=C:/Qt/6.11.1/mingw_64   
```

Adjust the version number to match your Qt installation. Default Qt install path on Windows is `C:\Qt`.

4. Build and run

Press `Shift + F10`. The SQLite database file (`fleet.db`) is created automatically in the build folder on first launch.
