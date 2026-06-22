#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <QLabel>


class IndividualClient;
class CorporateClient;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override {}

private:
    QTabWidget* tabWidget;
    QTableWidget* vehicleTable;
    QPushButton*  btnAddVehicle;
    QPushButton*  btnEditVehicle;
    QPushButton*  btnDeleteVehicle;
    QPushButton*  btnVehicleHistory;
    QTableWidget* clientTable;
    QPushButton*  btnAddClient;
    QPushButton*  btnEditClient;
    QPushButton*  btnDeleteClient;
    QTableWidget* transactionTable;
    QPushButton*  btnAddTransaction;
    QPushButton*  btnCompleteTransaction;
    QPushButton*  btnCancelTransaction;
    QPushButton*  btnDeleteTransaction;
    QTableWidget* maintenanceTable;
    QPushButton*  btnAddMaintenance;
    QPushButton*  btnDeleteMaintenance;

    void setupUI();
    QWidget*      buildVehicleTab();
    QWidget*      buildClientTab();
    QWidget*      buildTransactionTab();
    QWidget*      buildMaintenanceTab();
    QTableWidget* createTable(const QStringList& headers);

    int getSelectedId(QTableWidget* table) const;
    void showInfo(const QString& msg);
    void showError(const QString& msg);
    bool askConfirm(const QString& question);

private slots:
    void loadVehicles();
    void loadClients();
    void loadTransactions();
    void loadMaintenance();
    void onAddVehicle();
    void onEditVehicle();
    void onDeleteVehicle();
    void onVehicleHistory();
    void onAddClient();
    void onEditClient();
    void onDeleteClient();
    void onAddTransaction();
    void onCompleteTransaction();
    void onCancelTransaction();
    void onDeleteTransaction();
    void onAddMaintenance();
    void onDeleteMaintenance();
};
