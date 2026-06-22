#include "MainWindow.h"
#include "VehicleDialog.h"
#include "ClientDialog.h"
#include "TransactionDialog.h"
#include "MaintenanceDialog.h"
#include "DatabaseManager.h"
#include "IndividualClient.h"
#include "CorporateClient.h"
#include "Car.h"
#include "Truck.h"
#include "Motorcycle.h"

#include <QMessageBox>
#include <QApplication>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();

    loadVehicles();
    loadClients();
    loadTransactions();
    loadMaintenance();
}

void MainWindow::setupUI() {
    setWindowTitle("Car Fleet Management System");
    resize(1000, 620);
    setMinimumSize(800, 500);

    qApp->setStyle("Windows");

    tabWidget = new QTabWidget(this);
    tabWidget->addTab(buildVehicleTab(),"Vehicles");
    tabWidget->addTab(buildClientTab(),"Clients");
    tabWidget->addTab(buildTransactionTab(),"Transactions");
    tabWidget->addTab(buildMaintenanceTab(),"Maintenance");

    setCentralWidget(tabWidget);
    statusBar()->showMessage("Ready.");
}

QTableWidget* MainWindow::createTable(const QStringList& headers) {
    QTableWidget* table = new QTableWidget(0, headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setAlternatingRowColors(true);
    table->setSortingEnabled(true);
    return table;
}

QWidget* MainWindow::buildVehicleTab() {
    vehicleTable = createTable({"ID", "Type", "Brand/Model", "License Plate", "Year", "Rate/day", "Status", "Details"});
    vehicleTable->setColumnWidth(0,45);
    vehicleTable->setColumnWidth(1,85);
    vehicleTable->setColumnWidth(3,120);
    vehicleTable->setColumnWidth(4,55);
    vehicleTable->setColumnWidth(5,90);
    vehicleTable->setColumnWidth(6,90);

    btnAddVehicle = new QPushButton("Add");
    btnEditVehicle = new QPushButton("Edit");
    btnDeleteVehicle = new QPushButton("Delete");
    btnVehicleHistory = new QPushButton("Maintenance History");

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAddVehicle);
    btnRow->addWidget(btnEditVehicle);
    btnRow->addWidget(btnDeleteVehicle);
    btnRow->addStretch();
    btnRow->addWidget(btnVehicleHistory);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(vehicleTable);
    layout->addLayout(btnRow);

    QWidget* tab = new QWidget();
    tab->setLayout(layout);

    connect(btnAddVehicle, &QPushButton::clicked, this, &MainWindow::onAddVehicle);
    connect(btnEditVehicle, &QPushButton::clicked, this, &MainWindow::onEditVehicle);
    connect(btnDeleteVehicle, &QPushButton::clicked, this, &MainWindow::onDeleteVehicle);
    connect(btnVehicleHistory, &QPushButton::clicked, this, &MainWindow::onVehicleHistory);

    return tab;
}

QWidget* MainWindow::buildClientTab() {
    clientTable = createTable({"ID", "Type", "Name", "Phone", "Email", "ID Number", "Extra Info"});
    clientTable->setColumnWidth(0, 45);
    clientTable->setColumnWidth(1, 85);
    clientTable->setColumnWidth(3, 120);

    btnAddClient = new QPushButton("Add");
    btnEditClient = new QPushButton("Edit");
    btnDeleteClient = new QPushButton("Delete");

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAddClient);
    btnRow->addWidget(btnEditClient);
    btnRow->addWidget(btnDeleteClient);
    btnRow->addStretch();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(clientTable);
    layout->addLayout(btnRow);

    QWidget* tab = new QWidget();
    tab->setLayout(layout);

    connect(btnAddClient, &QPushButton::clicked, this, &MainWindow::onAddClient);
    connect(btnEditClient, &QPushButton::clicked, this, &MainWindow::onEditClient);
    connect(btnDeleteClient, &QPushButton::clicked, this, &MainWindow::onDeleteClient);

    return tab;
}

QWidget* MainWindow::buildTransactionTab() {
    transactionTable = createTable({"ID", "Vehicle", "Client", "Start Date", "End Date", "Cost (RON)", "Status"});
    transactionTable->setColumnWidth(0, 45);
    transactionTable->setColumnWidth(3, 95);
    transactionTable->setColumnWidth(4, 95);
    transactionTable->setColumnWidth(5, 90);
    transactionTable->setColumnWidth(6, 85);

    btnAddTransaction = new QPushButton("New Rental");
    btnCompleteTransaction = new QPushButton("Mark Completed");
    btnCancelTransaction = new QPushButton("Cancel Rental");
    btnDeleteTransaction = new QPushButton("Delete");

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAddTransaction);
    btnRow->addWidget(btnCompleteTransaction);
    btnRow->addWidget(btnCancelTransaction);
    btnRow->addStretch();
    btnRow->addWidget(btnDeleteTransaction);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(transactionTable);
    layout->addLayout(btnRow);

    QWidget* tab = new QWidget();
    tab->setLayout(layout);

    connect(btnAddTransaction, &QPushButton::clicked, this, &MainWindow::onAddTransaction);
    connect(btnCompleteTransaction, &QPushButton::clicked, this, &MainWindow::onCompleteTransaction);
    connect(btnCancelTransaction, &QPushButton::clicked, this, &MainWindow::onCancelTransaction);
    connect(btnDeleteTransaction, &QPushButton::clicked, this, &MainWindow::onDeleteTransaction);

    return tab;
}

QWidget* MainWindow::buildMaintenanceTab() {
    maintenanceTable = createTable({"ID", "Vehicle", "Date", "Type", "Description", "Cost (RON)"});
    maintenanceTable->setColumnWidth(0, 45);
    maintenanceTable->setColumnWidth(2, 95);
    maintenanceTable->setColumnWidth(3, 100);
    maintenanceTable->setColumnWidth(5, 90);

    btnAddMaintenance = new QPushButton("Add Record");
    btnDeleteMaintenance = new QPushButton("Delete");

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAddMaintenance);
    btnRow->addWidget(btnDeleteMaintenance);
    btnRow->addStretch();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(maintenanceTable);
    layout->addLayout(btnRow);

    QWidget* tab = new QWidget();
    tab->setLayout(layout);

    connect(btnAddMaintenance, &QPushButton::clicked, this, &MainWindow::onAddMaintenance);
    connect(btnDeleteMaintenance, &QPushButton::clicked, this, &MainWindow::onDeleteMaintenance);

    return tab;
}

//helpers
int MainWindow::getSelectedId(QTableWidget* table) const {
    int row = table->currentRow();
    if (row < 0)
        return -1;
    QTableWidgetItem* item = table->item(row, 0);
    if (item == nullptr)
        return -1;
    return item->data(Qt::UserRole).toInt();
}

void MainWindow::showInfo(const QString& msg) {
    QMessageBox::information(this, "Info", msg);
}

void MainWindow::showError(const QString& msg) {
    QMessageBox::warning(this, "Error", msg);
}

bool MainWindow::askConfirm(const QString& question) {
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "Confirm", question, QMessageBox::Yes | QMessageBox::No);
    return reply == QMessageBox::Yes;
}

void MainWindow::loadVehicles() {
    vehicleTable->setSortingEnabled(false);
    vehicleTable->setRowCount(0);

    std::vector<std::shared_ptr<Vehicle>> vehicles =
        DatabaseManager::getInstance()->getAllVehicles();

    for (std::shared_ptr<Vehicle> v : vehicles) {
        int row = vehicleTable->rowCount();
        vehicleTable->insertRow(row);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(v->getId()));
        idItem->setData(Qt::UserRole, v->getId());
        vehicleTable->setItem(row, 0, idItem);
        vehicleTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(v->getType())));
        vehicleTable->setItem(row, 2, new QTableWidgetItem(
            QString::fromStdString(v->getBrand() + " " + v->getModel())));
        vehicleTable->setItem(row, 3, new QTableWidgetItem(
            QString::fromStdString(v->getLicensePlate())));
        vehicleTable->setItem(row, 4, new QTableWidgetItem(QString::number(v->getYear())));
        vehicleTable->setItem(row, 5, new QTableWidgetItem(
            QString::number(v->getDailyRate(), 'f', 2)));
        vehicleTable->setItem(row, 6, new QTableWidgetItem(
            v->isAvailable() ? "Available" : "Rented"));

        QString details;
        if (Car* car = dynamic_cast<Car*>(v.get()))
            details = QString("%1 doors, %2").arg(car->getNumDoors())
                          .arg(QString::fromStdString(car->getFuelType()));
        else if (Truck* truck = dynamic_cast<Truck*>(v.get()))
            details = QString("%1t%2").arg(truck->getLoadCapacity())
                          .arg(truck->getHasRefrigeration() ? ", refrigerated" : "");
        else if (Motorcycle* moto = dynamic_cast<Motorcycle*>(v.get()))
            details = QString("%1cc, %2").arg(moto->getEngineCC())
                          .arg(QString::fromStdString(moto->getMotorcycleType()));
        vehicleTable->setItem(row, 7, new QTableWidgetItem(details));
    }

    vehicleTable->setSortingEnabled(true);
    statusBar()->showMessage(QString("Vehicles loaded: %1").arg(vehicles.size()));
}

void MainWindow::loadClients() {
    clientTable->setSortingEnabled(false);
    clientTable->setRowCount(0);

    std::vector<std::shared_ptr<Client>> clients =
        DatabaseManager::getInstance()->getAllClients();

    for (std::shared_ptr<Client> c : clients) {
        int row = clientTable->rowCount();
        clientTable->insertRow(row);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(c->getId()));
        idItem->setData(Qt::UserRole, c->getId());
        clientTable->setItem(row, 0, idItem);
        clientTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c->getClientType())));
        clientTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(c->getName())));
        clientTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(c->getPhone())));
        clientTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(c->getEmail())));
        clientTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(c->getIdentifier())));

        QString extra;
        if (IndividualClient* ind = dynamic_cast<IndividualClient*>(c.get()))
            extra = "License: " + QString::fromStdString(ind->getDriverLicense());
        else if (CorporateClient* corp = dynamic_cast<CorporateClient*>(c.get()))
            extra = QString::fromStdString(corp->getCompanyName());
        clientTable->setItem(row, 6, new QTableWidgetItem(extra));
    }

    clientTable->setSortingEnabled(true);
}

void MainWindow::loadTransactions() {
    transactionTable->setSortingEnabled(false);
    transactionTable->setRowCount(0);

    std::vector<Transaction> transactions =
        DatabaseManager::getInstance()->getAllTransactions();

    for (const Transaction& t : transactions) {
        int row = transactionTable->rowCount();
        transactionTable->insertRow(row);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(t.getId()));
        idItem->setData(Qt::UserRole, t.getId());
        transactionTable->setItem(row, 0, idItem);
        transactionTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(t.getVehicleName())));
        transactionTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(t.getClientName())));
        transactionTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(t.getStartDate())));
        transactionTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(t.getEndDate())));
        transactionTable->setItem(row, 5, new QTableWidgetItem(QString::number(t.getTotalCost(), 'f', 2)));
        transactionTable->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(t.getStatus())));
    }

    transactionTable->setSortingEnabled(true);
}

void MainWindow::loadMaintenance() {
    maintenanceTable->setSortingEnabled(false);
    maintenanceTable->setRowCount(0);

    std::vector<Maintenance> records =
        DatabaseManager::getInstance()->getAllMaintenance();

    for (const Maintenance& m : records) {
        int row = maintenanceTable->rowCount();
        maintenanceTable->insertRow(row);

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(m.getId()));
        idItem->setData(Qt::UserRole, m.getId());
        maintenanceTable->setItem(row, 0, idItem);
        maintenanceTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(m.getVehicleName())));
        maintenanceTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(m.getDate())));
        maintenanceTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(m.getType())));
        maintenanceTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(m.getDescription())));
        maintenanceTable->setItem(row, 5, new QTableWidgetItem(QString::number(m.getCost(), 'f', 2)));
    }

    maintenanceTable->setSortingEnabled(true);
}

void MainWindow::onAddVehicle() {
    VehicleDialog dialog(nullptr, this);
    if (dialog.exec() == QDialog::Accepted) {
        std::shared_ptr<Vehicle> v = dialog.getVehicle();
        if (v != nullptr && DatabaseManager::getInstance()->addVehicle(v.get())) {
            loadVehicles();
            statusBar()->showMessage("Vehicle added.");
        } else {
            showError("Could not add vehicle. License already exists.");
        }
    }
}

void MainWindow::onEditVehicle() {
    int id = getSelectedId(vehicleTable);
    if (id == -1) {
        showError("Please select a vehicle first.");
        return;
    }

    std::vector<std::shared_ptr<Vehicle>> all = DatabaseManager::getInstance()->getAllVehicles();
    std::shared_ptr<Vehicle> found = nullptr;
    for (std::shared_ptr<Vehicle> v : all) {
        if (v->getId() == id) {
            found = v;
            break;
        }
    }
    if (found == nullptr)
        return;

    VehicleDialog dialog(found, this);
    if (dialog.exec() == QDialog::Accepted) {
        std::shared_ptr<Vehicle> updated = dialog.getVehicle();
        if (updated != nullptr && DatabaseManager::getInstance()->updateVehicle(updated.get())) {
            loadVehicles();
            statusBar()->showMessage("Vehicle updated.");
        } else {
            showError("Update failed.");
        }
    }
}

void MainWindow::onDeleteVehicle() {
    int id = getSelectedId(vehicleTable);
    if (id == -1) {
        showError("Please select a vehicle first.");
        return;
    }
    if (!askConfirm("Are you sure you want to delete this vehicle?"))
        return;

    if (DatabaseManager::getInstance()->deleteVehicle(id)) {
        loadVehicles();
        loadTransactions();
        loadMaintenance();
        statusBar()->showMessage("Vehicle deleted.");
    } else {
        showError("Delete failed. The vehicle has active transactions.");
    }
}

void MainWindow::onVehicleHistory() {
    int id = getSelectedId(vehicleTable);
    if (id == -1) {
        showError("Please select a vehicle first.");
        return;
    }

    std::vector<Maintenance> records =
        DatabaseManager::getInstance()->getMaintenanceForVehicle(id);

    // Show a simple popup dialog with the vehicle's maintenance history
    QDialog histDialog(this);
    histDialog.setWindowTitle("Maintenance History");
    histDialog.resize(650, 350);

    QTableWidget* table = new QTableWidget(0, 5, &histDialog);
    table->setHorizontalHeaderLabels({"Date", "Type", "Description", "Cost (RON)", "ID"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    table->setAlternatingRowColors(true);

    for (const Maintenance& m : records) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(m.getDate())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(m.getType())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(m.getDescription())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(m.getCost(), 'f', 2)));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(m.getId())));
    }

    QLabel* countLabel = new QLabel(
        QString("Records found: %1").arg(records.size()), &histDialog);
    QPushButton* closeBtn = new QPushButton("Close", &histDialog);
    connect(closeBtn, &QPushButton::clicked, &histDialog, &QDialog::accept);

    QVBoxLayout* layout = new QVBoxLayout(&histDialog);
    layout->addWidget(countLabel);
    layout->addWidget(table);
    layout->addWidget(closeBtn);

    histDialog.exec();
}

void MainWindow::onAddClient() {
    ClientDialog dialog(nullptr, this);
    if (dialog.exec() == QDialog::Accepted) {
        std::shared_ptr<Client> c = dialog.getClient();
        if (c != nullptr && DatabaseManager::getInstance()->addClient(c.get())) {
            loadClients();
            statusBar()->showMessage("Client added.");
        } else {
            showError("Could not add client.");
        }
    }
}

void MainWindow::onEditClient() {
    int id = getSelectedId(clientTable);
    if (id == -1) {
        showError("Please select a client first.");
        return;
    }

    std::vector<std::shared_ptr<Client>> all = DatabaseManager::getInstance()->getAllClients();
    std::shared_ptr<Client> found = nullptr;
    for (std::shared_ptr<Client> c : all) {
        if (c->getId() == id) {
            found = c;
            break;
        }
    }
    if (found == nullptr)
        return;

    ClientDialog dialog(found, this);
    if (dialog.exec() == QDialog::Accepted) {
        std::shared_ptr<Client> updated = dialog.getClient();
        if (updated != nullptr && DatabaseManager::getInstance()->updateClient(updated.get())) {
            loadClients();
            statusBar()->showMessage("Client updated.");
        } else {
            showError("Update failed.");
        }
    }
}

void MainWindow::onDeleteClient() {
    int id = getSelectedId(clientTable);
    if (id == -1) {
        showError("Please select a client first.");
        return;
    }
    if (!askConfirm("Are you sure you want to delete this client?"))
        return;

    if (DatabaseManager::getInstance()->deleteClient(id)) {
        loadClients();
        statusBar()->showMessage("Client deleted.");
    } else {
        showError("Delete failed. The client has existing transactions.");
    }
}

void MainWindow::onAddTransaction() {
    TransactionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Transaction t = dialog.getTransaction();
        if (DatabaseManager::getInstance()->addTransaction(t)) {
            loadTransactions();
            loadVehicles();  // refresh availability status
            statusBar()->showMessage("Rental created.");
        } else {
            showError("Could not create rental.");
        }
    }
}

void MainWindow::onCompleteTransaction() {
    int id = getSelectedId(transactionTable);
    if (id == -1) {
        showError("Please select a transaction first.");
        return;
    }
    if (DatabaseManager::getInstance()->updateTransactionStatus(id, "Completed")) {
        loadTransactions();
        loadVehicles();
        statusBar()->showMessage("Transaction marked as completed.");
    } else {
        showError("Action failed.");
    }
}

void MainWindow::onCancelTransaction() {
    int id = getSelectedId(transactionTable);
    if (id == -1) {
        showError("Please select a transaction first.");
        return;
    }
    if (!askConfirm("Cancel this rental?"))
        return;

    if (DatabaseManager::getInstance()->updateTransactionStatus(id, "Cancelled")) {
        loadTransactions();
        loadVehicles();
        statusBar()->showMessage("Transaction cancelled.");
    } else {
        showError("Action failed.");
    }
}

void MainWindow::onDeleteTransaction() {
    int id = getSelectedId(transactionTable);
    if (id == -1) {
        showError("Please select a transaction first.");
        return;
    }
    if (!askConfirm("Delete this transaction record?"))
        return;

    if (DatabaseManager::getInstance()->deleteTransaction(id)) {
        loadTransactions();
        statusBar()->showMessage("Transaction deleted.");
    } else {
        showError("Delete failed.");
    }
}

void MainWindow::onAddMaintenance() {
    MaintenanceDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Maintenance m = dialog.getMaintenance();
        if (DatabaseManager::getInstance()->addMaintenance(m)) {
            loadMaintenance();
            statusBar()->showMessage("Maintenance record added.");
        } else {
            showError("Could not save maintenance record.");
        }
    }
}

void MainWindow::onDeleteMaintenance() {
    int id = getSelectedId(maintenanceTable);
    if (id == -1) {
        showError("Please select a record first.");
        return;
    }
    if (!askConfirm("Delete this maintenance record?"))
        return;

    if (DatabaseManager::getInstance()->deleteMaintenance(id)) {
        loadMaintenance();
        statusBar()->showMessage("Record deleted.");
    } else {
        showError("Delete failed.");
    }
}
