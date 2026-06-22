#include "MaintenanceDialog.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QDate>

MaintenanceDialog::MaintenanceDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Add Maintenance Record");
    setMinimumWidth(380);
    setupUI();
    loadVehicles();
}

void MaintenanceDialog::setupUI() {
    vehicleCombo = new QComboBox();
    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    typeCombo = new QComboBox();
    typeCombo->addItems({"Service", "Repair", "Technical Inspection", "Tire Change", "Oil Change", "Other"});
    descEdit = new QLineEdit();
    descEdit->setPlaceholderText("Describe the work done..........");
    costSpin = new QDoubleSpinBox();
    costSpin->setRange(0, 999999);
    costSpin->setDecimals(2);
    costSpin->setSuffix(" RON");

    QFormLayout* form = new QFormLayout();
    form->addRow("Vehicle:", vehicleCombo);
    form->addRow("Date:", dateEdit);
    form->addRow("Type:", typeCombo);
    form->addRow("Description:", descEdit);
    form->addRow("Cost:", costSpin);

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
}

void MaintenanceDialog::loadVehicles() {
    vehicleCombo->clear();
    vehicleIds.clear();
    vehicleNames.clear();

    std::vector<std::shared_ptr<Vehicle>> vehicles =
        DatabaseManager::getInstance()->getAllVehicles();

    for (std::shared_ptr<Vehicle> v : vehicles) {
        vehicleCombo->addItem(QString::fromStdString(v->getDisplayName()));
        vehicleIds.push_back(v->getId());
        vehicleNames.push_back(v->getDisplayName());
    }

    if (vehicles.empty())
        vehicleCombo->addItem("No vehicles in system");
}

bool MaintenanceDialog::validate() {
    if (vehicleIds.empty()) {
        QMessageBox::warning(this, "Validation", "No vehicles in the system.");
        return false;
    }
    if (descEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Description is required.");
        return false;
    }
    return true;
}

Maintenance MaintenanceDialog::getMaintenance() const {
    int idx = vehicleCombo->currentIndex();
    int vid  = vehicleIds[idx];
    std::string vname = vehicleNames[idx];

    return Maintenance(
        0,
        vid,
        vname,
        dateEdit->date().toString("yyyy-MM-dd").toStdString(),
        descEdit->text().trimmed().toStdString(),
        costSpin->value(),
        typeCombo->currentText().toStdString()
    );
}
