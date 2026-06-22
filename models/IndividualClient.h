#pragma once
#include "Client.h"

class IndividualClient : public Client {
private:
    std::string cnp;
    std::string driverLicense;

public:
    IndividualClient(int id, const std::string& name, const std::string& phone,
                     const std::string& email, const std::string& cnp,
                     const std::string& license)
        : Client(id, name, phone, email) {
        this->cnp = cnp;
        this->driverLicense = license;
    }

    std::string getClientType() const override {
        return "Individual";
    }
    std::string getIdentifier() const override {
        return cnp;
    }
    std::string getCNP() const {
        return cnp;
    }
    std::string getDriverLicense() const {
        return driverLicense;
    }
    void setCNP(const std::string& c) {
        cnp = c;
    }
    void setDriverLicense(const std::string& l) {
        driverLicense = l;
    }
};
