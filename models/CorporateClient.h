#pragma once
#include "Client.h"

class CorporateClient : public Client {
private:
    std::string cui;
    std::string contactPerson;
    std::string companyName;

public:
    CorporateClient(int id, const std::string& name, const std::string& phone,
                    const std::string& email, const std::string& cui,
                    const std::string& contactPerson, const std::string& companyName)
        : Client(id, name, phone, email) {
        this->cui = cui;
        this->contactPerson = contactPerson;
        this->companyName = companyName;
    }

    std::string getClientType() const override {
        return "Corporate";
    }
    std::string getIdentifier() const override {
        return cui;
    }
    std::string getCUI() const {
        return cui;
    }
    std::string getContactPerson() const {
        return contactPerson;
    }
    std::string getCompanyName() const {
        return companyName;
    }
    void setCUI(const std::string& c) {
        cui = c;
    }
    void setContactPerson(const std::string& p) {
        contactPerson = p;
    }
    void setCompanyName(const std::string& c) {
        companyName = c;
    }
};
