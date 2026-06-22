#pragma once
#include "Vehicle.h"

class Truck : public Vehicle {
private:
    double loadCapacity;
    bool hasRefrigeration;

public:
    Truck(int id, const std::string& plate, const std::string& brand,
          const std::string& model, int year, double dailyRate,
          double capacity, bool refrigeration, bool available)
        : Vehicle(id, plate, brand, model, year, dailyRate, available) {
          this->loadCapacity     = capacity;
          this->hasRefrigeration = refrigeration;
    }

    std::string getType() const override { return "Truck"; }

    double getLoadCapacity() const {
        return loadCapacity;
    }
    bool getHasRefrigeration() const {
        return hasRefrigeration;
    }
    void setLoadCapacity(double c) {
        loadCapacity = c;
    }
    void setHasRefrigeration(bool r) {
        hasRefrigeration = r;
    }
};
