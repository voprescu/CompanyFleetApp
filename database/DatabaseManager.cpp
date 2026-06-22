#include "DatabaseManager.h"
#include <QDebug>

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("fleet.db");

    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return;
    }

    qDebug() << "Database opened successfully.";

    QSqlQuery q;
    q.exec("PRAGMA foreign_keys = ON");

    createTables();
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen())
        db.close();
}

DatabaseManager* DatabaseManager::getInstance() {
    if (instance == nullptr)
        instance = new DatabaseManager();
    return instance;
}

bool DatabaseManager::isOpen() const {
    return db.isOpen();
}

void DatabaseManager::createTables() {
    QSqlQuery q;

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS vehicles (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            license_plate TEXT UNIQUE NOT NULL,
            brand TEXT NOT NULL,
            model TEXT NOT NULL,
            year INTEGER NOT NULL,
            daily_rate REAL NOT NULL,
            available INTEGER NOT NULL DEFAULT 1,
            type TEXT NOT NULL,
            num_doors INTEGER,
            fuel_type TEXT,
            load_capacity REAL,
            has_refrig INTEGER DEFAULT 0,
            engine_cc INTEGER,
            moto_type TEXT
        )
    )");

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS clients (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            phone TEXT NOT NULL,
            email TEXT DEFAULT '',
            client_type TEXT NOT NULL,
            cnp TEXT,
            driver_license TEXT,
            cui TEXT,
            contact_person TEXT,
            company_name TEXT
        )
    )");

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            vehicle_id INTEGER NOT NULL,
            client_id INTEGER NOT NULL,
            start_date TEXT NOT NULL,
            end_date TEXT NOT NULL,
            total_cost REAL NOT NULL,
            status TEXT NOT NULL DEFAULT 'Active',
            FOREIGN KEY (vehicle_id) REFERENCES vehicles(id),
            FOREIGN KEY (client_id) REFERENCES clients(id)
        )
    )");

    q.exec(R"(
        CREATE TABLE IF NOT EXISTS maintenance (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            vehicle_id INTEGER NOT NULL,
            maint_date TEXT NOT NULL,
            description TEXT NOT NULL,
            cost REAL NOT NULL DEFAULT 0,
            type TEXT NOT NULL,
            FOREIGN KEY (vehicle_id) REFERENCES vehicles(id)
        )
    )");

    if (q.lastError().isValid())
        qDebug() << "Error creating tables:" << q.lastError().text();
}

bool DatabaseManager::addVehicle(Vehicle* v) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO vehicles "
        "(license_plate, brand, model, year, daily_rate, available, type, "
        " num_doors, fuel_type, load_capacity, has_refrig, engine_cc, moto_type) "
        "VALUES "
        "(:plate, :brand, :model, :year, :rate, :avail, :type, "
        " :doors, :fuel, :cap, :refrig, :cc, :mtype)"
    );

    q.bindValue(":plate", QString::fromStdString(v->getLicensePlate()));
    q.bindValue(":brand", QString::fromStdString(v->getBrand()));
    q.bindValue(":model", QString::fromStdString(v->getModel()));
    q.bindValue(":year", v->getYear());
    q.bindValue(":rate", v->getDailyRate());
    q.bindValue(":avail", v->isAvailable() ? 1 : 0);
    q.bindValue(":type", QString::fromStdString(v->getType()));

    q.bindValue(":doors", QVariant());
    q.bindValue(":fuel", QVariant());
    q.bindValue(":cap", QVariant());
    q.bindValue(":refrig", QVariant());
    q.bindValue(":cc", QVariant());
    q.bindValue(":mtype", QVariant());

    if (Car* car = dynamic_cast<Car*>(v)) {
        q.bindValue(":doors", car->getNumDoors());
        q.bindValue(":fuel", QString::fromStdString(car->getFuelType()));
    } else if (Truck* truck = dynamic_cast<Truck*>(v)) {
        q.bindValue(":cap", truck->getLoadCapacity());
        q.bindValue(":refrig", truck->getHasRefrigeration() ? 1 : 0);
    } else if (Motorcycle* moto = dynamic_cast<Motorcycle*>(v)) {
        q.bindValue(":cc", moto->getEngineCC());
        q.bindValue(":mtype", QString::fromStdString(moto->getMotorcycleType()));
    }

    if (!q.exec()) {
        qDebug() << "addVehicle failed:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateVehicle(Vehicle* v) {
    QSqlQuery q;
    q.prepare(
        "UPDATE vehicles SET "
        "license_plate=:plate, brand=:brand, model=:model, year=:year, "
        "daily_rate=:rate, available=:avail, "
        "num_doors=:doors, fuel_type=:fuel, "
        "load_capacity=:cap, has_refrig=:refrig, "
        "engine_cc=:cc, moto_type=:mtype "
        "WHERE id=:id"
    );

    q.bindValue(":plate", QString::fromStdString(v->getLicensePlate()));
    q.bindValue(":brand", QString::fromStdString(v->getBrand()));
    q.bindValue(":model", QString::fromStdString(v->getModel()));
    q.bindValue(":year", v->getYear());
    q.bindValue(":rate", v->getDailyRate());
    q.bindValue(":avail", v->isAvailable() ? 1 : 0);
    q.bindValue(":id", v->getId());

    q.bindValue(":doors", QVariant());
    q.bindValue(":fuel", QVariant());
    q.bindValue(":cap", QVariant());
    q.bindValue(":refrig", QVariant());
    q.bindValue(":cc", QVariant());
    q.bindValue(":mtype", QVariant());

    if (Car* car = dynamic_cast<Car*>(v)) {
        q.bindValue(":doors", car->getNumDoors());
        q.bindValue(":fuel", QString::fromStdString(car->getFuelType()));
    } else if (Truck* truck = dynamic_cast<Truck*>(v)) {
        q.bindValue(":cap", truck->getLoadCapacity());
        q.bindValue(":refrig", truck->getHasRefrigeration() ? 1 : 0);
    } else if (Motorcycle* moto = dynamic_cast<Motorcycle*>(v)) {
        q.bindValue(":cc", moto->getEngineCC());
        q.bindValue(":mtype", QString::fromStdString(moto->getMotorcycleType()));
    }

    if (!q.exec()) {
        qDebug() << "updateVehicle failed:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteVehicle(int id) {
    QSqlQuery q;
    q.prepare("DELETE FROM vehicles WHERE id=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteVehicle failed:" << q.lastError().text();
        return false;
    }
    return true;
}

std::vector<std::shared_ptr<Vehicle>> DatabaseManager::getAllVehicles() {
    std::vector<std::shared_ptr<Vehicle>> list;
    QSqlQuery q("SELECT * FROM vehicles ORDER BY brand, model");

    while (q.next()) {
        int id = q.value("id").toInt();
        std::string plate = q.value("license_plate").toString().toStdString();
        std::string brand = q.value("brand").toString().toStdString();
        std::string model = q.value("model").toString().toStdString();
        int year = q.value("year").toInt();
        double rate  = q.value("daily_rate").toDouble();
        bool avail = q.value("available").toInt() == 1;
        std::string type  = q.value("type").toString().toStdString();

        //shared_ptr - smart ptr that automatically deletes the object
        std::shared_ptr<Vehicle> v;

        if (type == "Car") {
            int doors = q.value("num_doors").toInt();
            std::string fuel = q.value("fuel_type").toString().toStdString();
            v = std::make_shared<Car>(id, plate, brand, model, year, rate, doors, fuel, avail);

        } else if (type == "Truck") {
            double cap = q.value("load_capacity").toDouble();
            bool refrig = q.value("has_refrig").toInt() == 1;
            v = std::make_shared<Truck>(id, plate, brand, model, year, rate, cap, refrig, avail);

        } else if (type == "Motorcycle") {
            int cc = q.value("engine_cc").toInt();
            std::string mType = q.value("moto_type").toString().toStdString();
            v = std::make_shared<Motorcycle>(id, plate, brand, model, year, rate, cc, mType, avail);
        }

        if (v != nullptr)
            list.push_back(v);
    }
    return list;
}

std::vector<std::shared_ptr<Vehicle>> DatabaseManager::getAvailableVehicles() {
    std::vector<std::shared_ptr<Vehicle>> available;
    std::vector<std::shared_ptr<Vehicle>> all = getAllVehicles();

    for (std::shared_ptr<Vehicle> v : all) {
        if (v->isAvailable())
            available.push_back(v);
    }
    return available;
}

bool DatabaseManager::addClient(Client* c) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO clients "
        "(name, phone, email, client_type, cnp, driver_license, cui, contact_person, company_name) "
        "VALUES "
        "(:name, :phone, :email, :type, :cnp, :lic, :cui, :contact, :company)"
    );

    q.bindValue(":name", QString::fromStdString(c->getName()));
    q.bindValue(":phone", QString::fromStdString(c->getPhone()));
    q.bindValue(":email", QString::fromStdString(c->getEmail()));
    q.bindValue(":type",  QString::fromStdString(c->getClientType()));
    q.bindValue(":cnp", QVariant());
    q.bindValue(":lic", QVariant());
    q.bindValue(":cui", QVariant());
    q.bindValue(":contact", QVariant());
    q.bindValue(":company", QVariant());

    if (IndividualClient* ind = dynamic_cast<IndividualClient*>(c)) {
        q.bindValue(":cnp", QString::fromStdString(ind->getCNP()));
        q.bindValue(":lic", QString::fromStdString(ind->getDriverLicense()));
    } else if (CorporateClient* corp = dynamic_cast<CorporateClient*>(c)) {
        q.bindValue(":cui", QString::fromStdString(corp->getCUI()));
        q.bindValue(":contact", QString::fromStdString(corp->getContactPerson()));
        q.bindValue(":company", QString::fromStdString(corp->getCompanyName()));
    }

    if (!q.exec()) {
        qDebug() << "addClient failed:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateClient(Client* c) {
    QSqlQuery q;
    q.prepare(
        "UPDATE clients SET "
        "name=:name, phone=:phone, email=:email, "
        "cnp=:cnp, driver_license=:lic, "
        "cui=:cui, contact_person=:contact, company_name=:company "
        "WHERE id=:id"
    );

    q.bindValue(":name", QString::fromStdString(c->getName()));
    q.bindValue(":phone", QString::fromStdString(c->getPhone()));
    q.bindValue(":email", QString::fromStdString(c->getEmail()));
    q.bindValue(":id", c->getId());
    q.bindValue(":cnp", QVariant());
    q.bindValue(":lic", QVariant());
    q.bindValue(":cui", QVariant());
    q.bindValue(":contact", QVariant());
    q.bindValue(":company", QVariant());

    if (IndividualClient* ind = dynamic_cast<IndividualClient*>(c)) {
        q.bindValue(":cnp", QString::fromStdString(ind->getCNP()));
        q.bindValue(":lic", QString::fromStdString(ind->getDriverLicense()));
    } else if (CorporateClient* corp = dynamic_cast<CorporateClient*>(c)) {
        q.bindValue(":cui", QString::fromStdString(corp->getCUI()));
        q.bindValue(":contact", QString::fromStdString(corp->getContactPerson()));
        q.bindValue(":company", QString::fromStdString(corp->getCompanyName()));
    }

    if (!q.exec()) {
        qDebug() << "updateClient failed:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteClient(int id) {
    QSqlQuery q;
    q.prepare("DELETE FROM clients WHERE id=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteClient failed:" << q.lastError().text();
        return false;
    }
    return true;
}

std::vector<std::shared_ptr<Client>> DatabaseManager::getAllClients() {
    std::vector<std::shared_ptr<Client>> list;
    QSqlQuery q("SELECT * FROM clients ORDER BY name");

    while (q.next()) {
        int id = q.value("id").toInt();
        std::string name  = q.value("name").toString().toStdString();
        std::string phone = q.value("phone").toString().toStdString();
        std::string email = q.value("email").toString().toStdString();
        std::string type  = q.value("client_type").toString().toStdString();

        std::shared_ptr<Client> c;

        if (type == "Individual") {
            std::string cnp = q.value("cnp").toString().toStdString();
            std::string lic = q.value("driver_license").toString().toStdString();
            c = std::make_shared<IndividualClient>(id, name, phone, email, cnp, lic);

        } else if (type == "Corporate") {
            std::string cui = q.value("cui").toString().toStdString();
            std::string contact = q.value("contact_person").toString().toStdString();
            std::string company = q.value("company_name").toString().toStdString();
            c = std::make_shared<CorporateClient>(id, name, phone, email, cui, contact, company);
        }

        if (c != nullptr)
            list.push_back(c);
    }
    return list;
}

bool DatabaseManager::addTransaction(const Transaction& t) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO transactions (vehicle_id, client_id, start_date, end_date, total_cost, status) "
        "VALUES (:vid, :cid, :start, :end, :cost, :status)"
    );
    q.bindValue(":vid", t.getVehicleId());
    q.bindValue(":cid", t.getClientId());
    q.bindValue(":start", QString::fromStdString(t.getStartDate()));
    q.bindValue(":end", QString::fromStdString(t.getEndDate()));
    q.bindValue(":cost", t.getTotalCost());
    q.bindValue(":status", QString::fromStdString(t.getStatus()));

    if (!q.exec()) {
        qDebug() << "addTransaction failed:" << q.lastError().text();
        return false;
    }

    QSqlQuery update;
    update.prepare("UPDATE vehicles SET available=0 WHERE id=:id");
    update.bindValue(":id", t.getVehicleId());
    update.exec();

    return true;
}

bool DatabaseManager::updateTransactionStatus(int id, const std::string& status) {
    QSqlQuery getVehicle;
    getVehicle.prepare("SELECT vehicle_id FROM transactions WHERE id=:id");
    getVehicle.bindValue(":id", id);
    int vehicleId = -1;
    if (getVehicle.exec() && getVehicle.next())
        vehicleId = getVehicle.value(0).toInt();

    QSqlQuery q;
    q.prepare("UPDATE transactions SET status=:status WHERE id=:id");
    q.bindValue(":status", QString::fromStdString(status));
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "updateTransactionStatus failed:" << q.lastError().text();
        return false;
    }

    if ((status == "Completed" || status == "Cancelled") && vehicleId != -1) {
        QSqlQuery update;
        update.prepare("UPDATE vehicles SET available=1 WHERE id=:id");
        update.bindValue(":id", vehicleId);
        update.exec();
    }

    return true;
}

bool DatabaseManager::deleteTransaction(int id) {
    QSqlQuery q;
    q.prepare("DELETE FROM transactions WHERE id=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteTransaction failed:" << q.lastError().text();
        return false;
    }
    return true;
}

std::vector<Transaction> DatabaseManager::getAllTransactions() {
    std::vector<Transaction> list;

    QSqlQuery q(
        "SELECT t.id, t.vehicle_id, t.client_id, "
        "       v.brand || ' ' || v.model || ' [' || v.license_plate || ']' AS vehicle_name, "
        "       c.name AS client_name, "
        "       t.start_date, t.end_date, t.total_cost, t.status "
        "FROM transactions t "
        "JOIN vehicles v ON t.vehicle_id = v.id "
        "JOIN clients  c ON t.client_id  = c.id "
        "ORDER BY t.id DESC"
    );

    while (q.next()) {
        list.push_back(Transaction(
            q.value("id").toInt(),
            q.value("vehicle_id").toInt(),
            q.value("client_id").toInt(),
            q.value("vehicle_name").toString().toStdString(),
            q.value("client_name").toString().toStdString(),
            q.value("start_date").toString().toStdString(),
            q.value("end_date").toString().toStdString(),
            q.value("total_cost").toDouble(),
            q.value("status").toString().toStdString()
        ));
    }
    return list;
}

bool DatabaseManager::addMaintenance(const Maintenance& m) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO maintenance (vehicle_id, maint_date, description, cost, type) "
        "VALUES (:vid, :date, :desc, :cost, :type)"
    );
    q.bindValue(":vid", m.getVehicleId());
    q.bindValue(":date", QString::fromStdString(m.getDate()));
    q.bindValue(":desc", QString::fromStdString(m.getDescription()));
    q.bindValue(":cost", m.getCost());
    q.bindValue(":type", QString::fromStdString(m.getType()));

    if (!q.exec()) {
        qDebug() << "addMaintenance failed:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteMaintenance(int id) {
    QSqlQuery q;
    q.prepare("DELETE FROM maintenance WHERE id=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "deleteMaintenance failed:" << q.lastError().text();
        return false;
    }
    return true;
}

std::vector<Maintenance> DatabaseManager::getAllMaintenance() {
    std::vector<Maintenance> list;

    QSqlQuery q(
        "SELECT m.id, m.vehicle_id, "
        "       v.brand || ' ' || v.model || ' [' || v.license_plate || ']' AS vehicle_name, "
        "       m.maint_date, m.description, m.cost, m.type "
        "FROM maintenance m "
        "JOIN vehicles v ON m.vehicle_id = v.id "
        "ORDER BY m.maint_date DESC"
    );

    while (q.next()) {
        list.push_back(Maintenance(
            q.value("id").toInt(),
            q.value("vehicle_id").toInt(),
            q.value("vehicle_name").toString().toStdString(),
            q.value("maint_date").toString().toStdString(),
            q.value("description").toString().toStdString(),
            q.value("cost").toDouble(),
            q.value("type").toString().toStdString()
        ));
    }
    return list;
}

std::vector<Maintenance> DatabaseManager::getMaintenanceForVehicle(int vehicleId) {
    std::vector<Maintenance> list;

    QSqlQuery q;
    q.prepare(
        "SELECT m.id, m.vehicle_id, "
        "       v.brand || ' ' || v.model || ' [' || v.license_plate || ']' AS vehicle_name, "
        "       m.maint_date, m.description, m.cost, m.type "
        "FROM maintenance m "
        "JOIN vehicles v ON m.vehicle_id = v.id "
        "WHERE m.vehicle_id = :vid "
        "ORDER BY m.maint_date DESC"
    );
    q.bindValue(":vid", vehicleId);

    if (!q.exec())
        return list;

    while (q.next()) {
        list.push_back(Maintenance(
            q.value("id").toInt(),
            q.value("vehicle_id").toInt(),
            q.value("vehicle_name").toString().toStdString(),
            q.value("maint_date").toString().toStdString(),
            q.value("description").toString().toStdString(),
            q.value("cost").toDouble(),
            q.value("type").toString().toStdString()
        ));
    }
    return list;
}
