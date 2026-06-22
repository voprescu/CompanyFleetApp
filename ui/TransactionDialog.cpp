#include "TransactionDialog.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QDate>

TransactionDialog::TransactionDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("New Rental");
    setMinimumWidth(420);
    setupUI();
    loadData();
}

void TransactionDialog::setupUI() {
    vehicleCombo = new QComboBox();
    clientCombo = new QComboBox();

    startDateEdit = new QDateEdit(QDate::currentDate());
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("yyyy-MM-dd");

    endDateEdit = new QDateEdit(QDate::currentDate().addDays(3));
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("yyyy-MM-dd");

    costSpin = new QDoubleSpinBox();
    costSpin->setRange(0, 999999);
    costSpin->setDecimals(2);
    costSpin->setSuffix(" RON");
    costSpin->setReadOnly(true);
    costSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);

    costInfoLabel = new QLabel("Select a vehicle and dates to calculate cost.");

    QFormLayout* form = new QFormLayout();
    form->addRow("Vehicle:", vehicleCombo);
    form->addRow("Client:", clientCombo);
    form->addRow("Start Date:", startDateEdit);
    form->addRow("End Date:", endDateEdit);
    form->addRow("Total Cost:", costSpin);
    form->addRow("", costInfoLabel);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (validate())
            accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addStretch();
    layout->addWidget(buttons);

    connect(vehicleCombo, &QComboBox::currentIndexChanged,
            this, &TransactionDialog::recalculateCost);
    connect(startDateEdit, &QDateEdit::dateChanged,
            this, &TransactionDialog::recalculateCost);
    connect(endDateEdit, &QDateEdit::dateChanged,
            this, &TransactionDialog::recalculateCost);
}

void TransactionDialog::loadData() {
    vehicleCombo->clear();
    clientCombo->clear();
    availableVehicles = DatabaseManager::getInstance()->getAvailableVehicles();
    clients           = DatabaseManager::getInstance()->getAllClients();

    if (availableVehicles.empty()) {
        vehicleCombo->addItem("No vehicles available");
    } else {
        for (std::shared_ptr<Vehicle> v : availableVehicles) {
            vehicleCombo->addItem(
                QString::fromStdString(v->getDisplayName()) +
                QString("  (%1 RON/day)").arg(v->getDailyRate(), 0, 'f', 2));
        }
    }
    if (clients.empty()) {
        clientCombo->addItem("No clients registered");
    } else {
        for (std::shared_ptr<Client> c : clients) {
            clientCombo->addItem(
                QString::fromStdString(c->getName()) +
                "  (" + QString::fromStdString(c->getClientType()) + ")");
        }
    }
    recalculateCost();
}

//automatic cost calculation
void TransactionDialog::recalculateCost() {
    int idx = vehicleCombo->currentIndex();
    if (idx < 0 || idx >= (int)availableVehicles.size()) {
        costSpin->setValue(0);
        costInfoLabel->setText("No vehicle selected.");
        return;
    }
    QDate start = startDateEdit->date();
    QDate end   = endDateEdit->date();
    int days  = start.daysTo(end);

    if (days <= 0) {
        costSpin->setValue(0);
        costInfoLabel->setText("End date must be after start date.");
        return;
    }

    double dailyRate = availableVehicles[idx]->getDailyRate();
    double total = dailyRate * days;
    costSpin->setValue(total);
    costInfoLabel->setText(
        QString("%1 days x %2 RON/day = %3 RON")
            .arg(days)
            .arg(dailyRate, 0, 'f', 2)
            .arg(total, 0, 'f', 2));
}

bool TransactionDialog::validate() {
    if (availableVehicles.empty()) {
        QMessageBox::warning(this, "Validation", "No available vehicles.");
        return false;
    }
    if (clients.empty()) {
        QMessageBox::warning(this, "Validation", "No clients registered.");
        return false;
    }
    int days = startDateEdit->date().daysTo(endDateEdit->date());
    if (days <= 0) {
        QMessageBox::warning(this, "Validation", "End date must be after start date.");
        return false;
    }
    return true;
}

Transaction TransactionDialog::getTransaction() const {
    int vIdx = vehicleCombo->currentIndex();
    int cIdx = clientCombo->currentIndex();
    int vehicleId = availableVehicles[vIdx]->getId();
    int clientId  = clients[cIdx]->getId();
    std::string vehicleName = availableVehicles[vIdx]->getDisplayName();
    std::string clientName  = clients[cIdx]->getName();

    return Transaction(
        0,
        vehicleId,
        clientId,
        vehicleName,
        clientName,
        startDateEdit->date().toString("yyyy-MM-dd").toStdString(),
        endDateEdit->date().toString("yyyy-MM-dd").toStdString(),
        costSpin->value(),
        "Active"
    );
}
