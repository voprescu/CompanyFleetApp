#pragma once
#include <string>

//abstract class for clients.
class Client {
protected:
    int id;
    std::string name;
    std::string phone;
    std::string email;

public:
    Client(int id, const std::string& name,
           const std::string& phone, const std::string& email) {
        this->id = id;
        this->name = name;
        this->phone = phone;
        this->email = email;
    }

    virtual ~Client() {}

    virtual std::string getClientType() const = 0;
    virtual std::string getIdentifier() const = 0;  //CNP for individuals/CUI for companies

    int getId() const {
        return id;
    }
    std::string getName() const {
        return name;
    }
    std::string getPhone() const {
        return phone;
    }
    std::string getEmail() const {
        return email;
    }

    void setId(int i) {
        id = i;
    }
    void setName(const std::string& n) {
        name = n;
    }
    void setPhone(const std::string& p) {
        phone = p;
    }
    void setEmail(const std::string& e) {
        email = e;
    }
};
