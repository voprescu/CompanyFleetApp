#pragma once
#include <string>


//vehicleName populated with sql join
class Maintenance {
private:
    int id;
    int vehicleId;
    std::string vehicleName;
    std::string date;
    std::string description;
    double cost;
    std::string type; //"Service", "Repair", "Inspection", "Tire Change"

public:
    Maintenance(int id, int vehicleId, const std::string& vehicleName,
                const std::string& date, const std::string& description,
                double cost, const std::string& type) {
        this->id = id;
        this->vehicleId = vehicleId;
        this->vehicleName = vehicleName;
        this->date = date;
        this->description = description;
        this->cost = cost;
        this->type = type;
    }

    int getId() const {
        return id;
    }
    int getVehicleId() const {
        return vehicleId;
    }
    std::string getVehicleName() const {
        return vehicleName;
    }
    std::string getDate() const {
        return date;
    }
    std::string getDescription() const {
        return description;
    }
    double getCost() const {
        return cost;
    }
    std::string getType() const {
        return type;
    }
};
