#pragma once
#include <string>

//vehicle abstract class.
class Vehicle {
protected:
    int id;
    std::string licensePlate;
    std::string brand;
    std::string model;
    int year;
    double dailyRate;
    bool available;

public:
    Vehicle(int id, const std::string& plate, const std::string& brand,
            const std::string& model, int year, double dailyRate, bool available) {
        this->id = id;
        this->licensePlate = plate;
        this->brand = brand;
        this->model = model;
        this->year = year;
        this->dailyRate = dailyRate;
        this->available = available;
    }

    virtual ~Vehicle() {}

    virtual std::string getType() const = 0;

    int getId() const {
        return id;
    }
    std::string getLicensePlate() const {
        return licensePlate;
    }
    std::string getBrand() const {
        return brand;
    }
    std::string getModel() const {
        return model;
    }
    int getYear() const {
        return year;
    }
    double getDailyRate() const {
        return dailyRate;
    }
    bool isAvailable() const {
        return available;
    }

    void setId(int newId) {
        id = newId;
    }
    void setLicensePlate(const std::string& p) {
        licensePlate = p;
    }
    void setBrand(const std::string& b) {
        brand = b;
    }
    void setModel(const std::string& m) {
        model = m;
    }
    void setYear(int y) {
        year = y;
    }
    void setDailyRate(double rate) {
        dailyRate = rate;
    }
    void setAvailable(bool a) {
        available = a;
    }

    std::string getDisplayName() const {
        return brand + " " + model + " [" + licensePlate + "]";
    }
};
