#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <vector>
#include <memory>

#include "Vehicle.h"
#include "Car.h"
#include "Truck.h"
#include "Motorcycle.h"
#include "Client.h"
#include "IndividualClient.h"
#include "CorporateClient.h"
#include "Transaction.h"
#include "Maintenance.h"


class DatabaseManager {
private:
    static DatabaseManager* instance;
    QSqlDatabase db;

    DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void createTables();

public:
    static DatabaseManager* getInstance();
    ~DatabaseManager();

    bool isOpen() const;

    bool addVehicle(Vehicle* v);
    bool updateVehicle(Vehicle* v);
    bool deleteVehicle(int id);
    std::vector<std::shared_ptr<Vehicle>> getAllVehicles();
    std::vector<std::shared_ptr<Vehicle>> getAvailableVehicles();

    bool addClient(Client* c);
    bool updateClient(Client* c);
    bool deleteClient(int id);
    std::vector<std::shared_ptr<Client>> getAllClients();

    bool addTransaction(const Transaction& t);
    bool updateTransactionStatus(int id, const std::string& status);
    bool deleteTransaction(int id);
    std::vector<Transaction> getAllTransactions();

    bool addMaintenance(const Maintenance& m);
    bool deleteMaintenance(int id);
    std::vector<Maintenance> getAllMaintenance();
    std::vector<Maintenance> getMaintenanceForVehicle(int vehicleId);
};
