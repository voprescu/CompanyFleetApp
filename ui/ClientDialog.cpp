#include "ClientDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>

ClientDialog::ClientDialog(std::shared_ptr<Client> client, QWidget* parent)
    : QDialog(parent) {
    editId = (client != nullptr) ? client->getId() : 0;
    setWindowTitle(client != nullptr ? "Edit Client" : "Add Client");
    setMinimumWidth(380);
    setupUI();
    if (client != nullptr)
        populateFields(client);
}

void ClientDialog::setupUI() {
    typeCombo = new QComboBox();
    typeCombo->addItems({"Individual", "Corporate"});
    nameEdit  = new QLineEdit();
    nameEdit->setPlaceholderText("Full name");
    phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("e.g. 07xx xxx xxx");
    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("email@example.com");

    QFormLayout* commonForm = new QFormLayout();
    commonForm->addRow("Client Type:", typeCombo);
    commonForm->addRow("Name:", nameEdit);
    commonForm->addRow("Phone:", phoneEdit);
    commonForm->addRow("Email:", emailEdit);

    cnpEdit = new QLineEdit();
    cnpEdit->setPlaceholderText("13 digits");
    cnpEdit->setMaxLength(13);
    licenseEdit = new QLineEdit();
    licenseEdit->setPlaceholderText("e.g. B xxx xxx");

    QFormLayout* indForm = new QFormLayout();
    indForm->addRow("CNP:", cnpEdit);
    indForm->addRow("Driver License:", licenseEdit);
    individualGroup = new QGroupBox("Individual Details");
    individualGroup->setLayout(indForm);


    cuiEdit = new QLineEdit();
    cuiEdit->setPlaceholderText("e.g. RO12345678");

    contactEdit = new QLineEdit();
    contactEdit->setPlaceholderText("Contact person name");

    companyEdit = new QLineEdit();
    companyEdit->setPlaceholderText("e.g. Company SRL");

    QFormLayout* corpForm = new QFormLayout();
    corpForm->addRow("Tax ID (CUI):", cuiEdit);
    corpForm->addRow("Contact Person:", contactEdit);
    corpForm->addRow("Company Name:", companyEdit);
    corporateGroup = new QGroupBox("Corporate Details");
    corporateGroup->setLayout(corpForm);
    corporateGroup->setVisible(false);

    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (validate())
            accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(commonForm);
    layout->addWidget(individualGroup);
    layout->addWidget(corporateGroup);
    layout->addStretch();
    layout->addWidget(buttons);

    connect(typeCombo, &QComboBox::currentTextChanged,
            this, &ClientDialog::onTypeChanged);
}

void ClientDialog::populateFields(std::shared_ptr<Client> c) {
    typeCombo->setCurrentText(QString::fromStdString(c->getClientType()));
    typeCombo->setEnabled(false);
    nameEdit->setText(QString::fromStdString(c->getName()));
    phoneEdit->setText(QString::fromStdString(c->getPhone()));
    emailEdit->setText(QString::fromStdString(c->getEmail()));

    if (IndividualClient* ind = dynamic_cast<IndividualClient*>(c.get())) {
        cnpEdit->setText(QString::fromStdString(ind->getCNP()));
        licenseEdit->setText(QString::fromStdString(ind->getDriverLicense()));
    } else if (CorporateClient* corp = dynamic_cast<CorporateClient*>(c.get())) {
        cuiEdit->setText(QString::fromStdString(corp->getCUI()));
        contactEdit->setText(QString::fromStdString(corp->getContactPerson()));
        companyEdit->setText(QString::fromStdString(corp->getCompanyName()));
    }
}

void ClientDialog::onTypeChanged(const QString& type) {
    individualGroup->setVisible(type == "Individual");
    corporateGroup->setVisible(type == "Corporate");
    adjustSize();
}

bool ClientDialog::validate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Name is required.");
        return false;
    }
    if (phoneEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Phone is required.");
        return false;
    }
    if (typeCombo->currentText() == "Individual") {
        if (cnpEdit->text().trimmed().length() != 13) {
            QMessageBox::warning(this, "Validation", "CNP must be exactly 13 digits.");
            return false;
        }
    } else {
        if (cuiEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Tax ID (CUI) is required.");
            return false;
        }
        if (companyEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Validation", "Company name is required.");
            return false;
        }
    }
    return true;
}

std::shared_ptr<Client> ClientDialog::getClient() const {
    std::string name  = nameEdit->text().trimmed().toStdString();
    std::string phone = phoneEdit->text().trimmed().toStdString();
    std::string email = emailEdit->text().trimmed().toStdString();

    if (typeCombo->currentText() == "Individual") {
        return std::make_shared<IndividualClient>(
            editId, name, phone, email,
            cnpEdit->text().trimmed().toStdString(),
            licenseEdit->text().trimmed().toStdString());
    } else {
        return std::make_shared<CorporateClient>(
            editId, name, phone, email,
            cuiEdit->text().trimmed().toStdString(),
            contactEdit->text().trimmed().toStdString(),
            companyEdit->text().trimmed().toStdString());
    }
}
