#include "VehicleDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QDate>

VehicleDialog::VehicleDialog(std::shared_ptr<Vehicle> vehicle, QWidget* parent)
    : QDialog(parent) {
        editId = (vehicle != nullptr) ? vehicle->getId() : 0;
        setWindowTitle(vehicle != nullptr ? "Edit Vehicle" : "Add Vehicle");
        setMinimumWidth(400);
        setupUI();
    if (vehicle != nullptr)
        populateFields(vehicle);
}

void VehicleDialog::setupUI() {
    typeCombo = new QComboBox();
    typeCombo->addItems({"Car", "Truck", "Motorcycle"});

    plateEdit = new QLineEdit();
    plateEdit->setPlaceholderText("e.g. B 123 ABC");

    brandEdit = new QLineEdit();
    brandEdit->setPlaceholderText("e.g. Dacia");

    modelEdit = new QLineEdit();
    modelEdit->setPlaceholderText("e.g. Logan");

    yearSpin = new QSpinBox();
    yearSpin->setRange(1990, QDate::currentDate().year());
    yearSpin->setValue(QDate::currentDate().year());

    rateSpin = new QDoubleSpinBox();
    rateSpin->setRange(1.0, 9999.0);
    rateSpin->setValue(100.0);
    rateSpin->setSuffix(" RON/day");
    rateSpin->setDecimals(2);

    availableCheck = new QCheckBox("Available for rental");
    availableCheck->setChecked(true);

    QFormLayout* commonForm = new QFormLayout();
    commonForm->addRow("Type:", typeCombo);
    commonForm->addRow("License Plate:", plateEdit);
    commonForm->addRow("Brand:", brandEdit);
    commonForm->addRow("Model:", modelEdit);
    commonForm->addRow("Year:", yearSpin);
    commonForm->addRow("Daily Cost:", rateSpin);
    commonForm->addRow("", availableCheck);

    doorsSpin = new QSpinBox();
    doorsSpin->setRange(2, 5);
    doorsSpin->setValue(4);

    fuelCombo = new QComboBox();
    fuelCombo->addItems({"Petrol", "Diesel", "Electric", "Hybrid", "LPG"});

    QFormLayout* carForm = new QFormLayout();
    carForm->addRow("Number of Doors:", doorsSpin);
    carForm->addRow("Fuel Type:", fuelCombo);
    carGroup = new QGroupBox("Car Details");
    carGroup->setLayout(carForm);

    capacitySpin = new QDoubleSpinBox();
    capacitySpin->setRange(0.1, 50.0);
    capacitySpin->setValue(3.5);
    capacitySpin->setSuffix(" tons");
    capacitySpin->setDecimals(1);

    refrigCheck = new QCheckBox("Has refrigeration unit");

    QFormLayout* truckForm = new QFormLayout();
    truckForm->addRow("Load Capacity:", capacitySpin);
    truckForm->addRow("", refrigCheck);
    truckGroup = new QGroupBox("Truck Details");
    truckGroup->setLayout(truckForm);
    truckGroup->setVisible(false);

    ccSpin = new QSpinBox();
    ccSpin->setRange(50, 2000);
    ccSpin->setValue(600);
    ccSpin->setSuffix(" cc");

    motoTypeCombo = new QComboBox();
    motoTypeCombo->addItems({"Sport", "Naked", "Touring", "Scooter"});

    QFormLayout* motoForm = new QFormLayout();
    motoForm->addRow("Engine Size:", ccSpin);
    motoForm->addRow("Type:", motoTypeCombo);
    motoGroup = new QGroupBox("Motorcycle Details");
    motoGroup->setLayout(motoForm);
    motoGroup->setVisible(false);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    //Lambda to check before accepting
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (validate())
            accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(commonForm);
    layout->addWidget(carGroup);
    layout->addWidget(truckGroup);
    layout->addWidget(motoGroup);
    layout->addStretch();
    layout->addWidget(buttons);

    connect(typeCombo, &QComboBox::currentTextChanged,
            this, &VehicleDialog::onTypeChanged);
}

void VehicleDialog::populateFields(std::shared_ptr<Vehicle> v) {
    typeCombo->setCurrentText(QString::fromStdString(v->getType()));
    typeCombo->setEnabled(false);  //dont allow changing type when editing

    plateEdit->setText(QString::fromStdString(v->getLicensePlate()));
    brandEdit->setText(QString::fromStdString(v->getBrand()));
    modelEdit->setText(QString::fromStdString(v->getModel()));
    yearSpin->setValue(v->getYear());
    rateSpin->setValue(v->getDailyRate());
    availableCheck->setChecked(v->isAvailable());

    if (Car* car = dynamic_cast<Car*>(v.get())) {
        doorsSpin->setValue(car->getNumDoors());
        fuelCombo->setCurrentText(QString::fromStdString(car->getFuelType()));
    } else if (Truck* truck = dynamic_cast<Truck*>(v.get())) {
        capacitySpin->setValue(truck->getLoadCapacity());
        refrigCheck->setChecked(truck->getHasRefrigeration());
    } else if (Motorcycle* moto = dynamic_cast<Motorcycle*>(v.get())) {
        ccSpin->setValue(moto->getEngineCC());
        motoTypeCombo->setCurrentText(QString::fromStdString(moto->getMotorcycleType()));
    }
}

void VehicleDialog::onTypeChanged(const QString& type) {
    carGroup->setVisible(type == "Car");
    truckGroup->setVisible(type == "Truck");
    motoGroup->setVisible(type == "Motorcycle");
    adjustSize();
}

bool VehicleDialog::validate() {
    if (plateEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "License plate is required.");
        return false;
    }
    if (brandEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Brand is required.");
        return false;
    }
    if (modelEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Model is required.");
        return false;
    }
    return true;
}

std::shared_ptr<Vehicle> VehicleDialog::getVehicle() const {
    std::string plate = plateEdit->text().trimmed().toStdString();
    std::string brand = brandEdit->text().trimmed().toStdString();
    std::string model = modelEdit->text().trimmed().toStdString();
    int year  = yearSpin->value();
    double rate  = rateSpin->value();
    bool avail = availableCheck->isChecked();
    QString type = typeCombo->currentText();

    if (type == "Car") {
        return std::make_shared<Car>(
            editId, plate, brand, model, year, rate,
            doorsSpin->value(),
            fuelCombo->currentText().toStdString(),
            avail);
    } else if (type == "Truck") {
        return std::make_shared<Truck>(
            editId, plate, brand, model, year, rate,
            capacitySpin->value(),
            refrigCheck->isChecked(),
            avail);
    } else {
        return std::make_shared<Motorcycle>(
            editId, plate, brand, model, year, rate,
            ccSpin->value(),
            motoTypeCombo->currentText().toStdString(),
            avail);
    }
}
