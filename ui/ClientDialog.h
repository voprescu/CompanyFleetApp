#pragma once

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QGroupBox>
#include <memory>

#include "Client.h"
#include "IndividualClient.h"
#include "CorporateClient.h"

class ClientDialog : public QDialog {
    Q_OBJECT

public:
    explicit ClientDialog(std::shared_ptr<Client> client, QWidget* parent = nullptr);

    std::shared_ptr<Client> getClient() const;

private:
    void setupUI();
    void populateFields(std::shared_ptr<Client> c);
    bool validate();

    QComboBox* typeCombo;
    QLineEdit* nameEdit;
    QLineEdit* phoneEdit;
    QLineEdit* emailEdit;
    QGroupBox* individualGroup;
    QLineEdit* cnpEdit;
    QLineEdit* licenseEdit;
    QGroupBox* corporateGroup;
    QLineEdit* cuiEdit;
    QLineEdit* contactEdit;
    QLineEdit* companyEdit;

    int editId;

private slots:
    void onTypeChanged(const QString& type);
};
