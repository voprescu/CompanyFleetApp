#pragma once
#include "Vehicle.h"

class Car : public Vehicle {
private:
    int numDoors;
    std::string fuelType;

public:
    Car(int id, const std::string& plate, const std::string& brand,
        const std::string& model, int year, double dailyRate,
        int doors, const std::string& fuel, bool available)
        : Vehicle(id, plate, brand, model, year, dailyRate, available) {
        this->numDoors = doors;
        this->fuelType = fuel;
    }

    std::string getType() const override {
        return "Car";
    }
    int getNumDoors() const {
        return numDoors;
    }
    std::string getFuelType() const {
        return fuelType;
    }
    void setNumDoors(int d) {
        numDoors = d;
    }
    void setFuelType(const std::string& f) {
        fuelType = f;
    }
};
