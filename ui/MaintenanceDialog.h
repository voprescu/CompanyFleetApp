#pragma once

#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <vector>

#include "Maintenance.h"

class MaintenanceDialog : public QDialog {
    Q_OBJECT

public:
    explicit MaintenanceDialog(QWidget* parent = nullptr);

    Maintenance getMaintenance() const;

private:
    void setupUI();
    void loadVehicles();
    bool validate();

    QComboBox* vehicleCombo;
    QDateEdit* dateEdit;
    QComboBox* typeCombo;
    QLineEdit* descEdit;
    QDoubleSpinBox* costSpin;

    std::vector<int> vehicleIds;
    std::vector<std::string> vehicleNames;
};
