cmake_minimum_required(VERSION 3.20)
project(ParcAuto VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Sql REQUIRED)

set(SOURCES
    main.cpp
    database/DatabaseManager.cpp
    ui/MainWindow.cpp
    ui/VehicleDialog.cpp
    ui/ClientDialog.cpp
    ui/TransactionDialog.cpp
    ui/MaintenanceDialog.cpp
)

set(HEADERS
    models/Vehicle.h
    models/Car.h
    models/Truck.h
    models/Motorcycle.h
    models/Client.h
    models/IndividualClient.h
    models/CorporateClient.h
    models/Transaction.h
    models/Maintenance.h
    database/DatabaseManager.h
    ui/MainWindow.h
    ui/VehicleDialog.h
    ui/ClientDialog.h
    ui/TransactionDialog.h
    ui/MaintenanceDialog.h
)

add_executable(ParcAuto ${SOURCES} ${HEADERS})

target_include_directories(ParcAuto PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/database
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

target_link_libraries(ParcAuto PRIVATE Qt6::Widgets Qt6::Sql)
