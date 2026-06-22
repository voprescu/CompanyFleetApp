#pragma once
#include <string>


// vehicleName and clientName using sql join when loading from db,
class Transaction {
private:
    int id;
    int vehicleId;
    int clientId;
    std::string vehicleName;
    std::string clientName;
    std::string startDate;
    std::string endDate;
    double totalCost;
    std::string status;     //"Active", "Completed", "Cancelled"

public:
    Transaction(int id, int vehicleId, int clientId,
                const std::string& vehicleName, const std::string& clientName,
                const std::string& startDate, const std::string& endDate,
                double totalCost, const std::string& status) {
        this->id = id;
        this->vehicleId = vehicleId;
        this->clientId = clientId;
        this->vehicleName = vehicleName;
        this->clientName = clientName;
        this->startDate = startDate;
        this->endDate = endDate;
        this->totalCost = totalCost;
        this->status = status;
    }

    int getId() const {
        return id;
    }
    int getVehicleId() const {
        return vehicleId;
    }
    int getClientId() const {
        return clientId;
    }
    std::string getVehicleName() const {
        return vehicleName;
    }
    std::string getClientName() const {
        return clientName;
    }
    std::string getStartDate() const {
        return startDate;
    }
    std::string getEndDate() const {
        return endDate;
    }
    double getTotalCost() const {
        return totalCost;
    }
    std::string getStatus() const {
        return status;
    }

    void setStatus(const std::string& s) { status = s; }
};
