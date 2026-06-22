#pragma once

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <memory>

#include "Vehicle.h"
#include "Car.h"
#include "Truck.h"
#include "Motorcycle.h"

class VehicleDialog : public QDialog {
    Q_OBJECT

public:
    // Pass nullptr to open in "Add" mode, or an existing vehicle for "Edit" mode
    explicit VehicleDialog(std::shared_ptr<Vehicle> vehicle, QWidget* parent = nullptr);
    std::shared_ptr<Vehicle> getVehicle() const;

private:
    void setupUI();
    void populateFields(std::shared_ptr<Vehicle> v);
    bool validate();

    QComboBox* typeCombo;
    QLineEdit* plateEdit;
    QLineEdit* brandEdit;
    QLineEdit* modelEdit;
    QSpinBox* yearSpin;
    QDoubleSpinBox* rateSpin;
    QCheckBox* availableCheck;
    QGroupBox* carGroup;
    QSpinBox* doorsSpin;
    QComboBox* fuelCombo;
    QGroupBox* truckGroup;
    QDoubleSpinBox* capacitySpin;
    QCheckBox* refrigCheck;
    QGroupBox* motoGroup;
    QSpinBox* ccSpin;
    QComboBox* motoTypeCombo;

    int editId;

private slots:
    void onTypeChanged(const QString& type);
};
