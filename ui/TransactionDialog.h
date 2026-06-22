#pragma once

#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <vector>
#include <memory>

#include "Transaction.h"
#include "Vehicle.h"
#include "Client.h"

class TransactionDialog : public QDialog {
    Q_OBJECT

public:
    explicit TransactionDialog(QWidget* parent = nullptr);

    Transaction getTransaction() const;

private:
    void setupUI();
    void loadData();
    bool validate();

    QComboBox* vehicleCombo;
    QComboBox* clientCombo;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QDoubleSpinBox* costSpin;
    QLabel* costInfoLabel;

    std::vector<std::shared_ptr<Vehicle>> availableVehicles;
    std::vector<std::shared_ptr<Client>>  clients;

private slots:
    void recalculateCost();
};
