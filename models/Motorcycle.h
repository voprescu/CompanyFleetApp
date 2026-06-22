#pragma once
#include "Vehicle.h"

class Motorcycle : public Vehicle {
private:
    int engineCC;
    std::string motorcycleType;  //"Sport", "Naked", "Touring", "Scooter"

public:
    Motorcycle(int id, const std::string& plate, const std::string& brand,
               const std::string& model, int year, double dailyRate,
               int cc, const std::string& motoType, bool available)
        : Vehicle(id, plate, brand, model, year, dailyRate, available) {
        this->engineCC = cc;
        this->motorcycleType = motoType;
    }

    std::string getType() const override {
        return "Motorcycle";
    }
    int getEngineCC() const {
        return engineCC;
    }
    std::string getMotorcycleType() const {
        return motorcycleType;
    }
    void setEngineCC(int cc) {
        engineCC = cc;
    }
    void setMotorcycleType(const std::string& t) {
        motorcycleType = t;
    }
};
