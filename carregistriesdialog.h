#ifndef CARREGISTRIESDIALOG_H
#define CARREGISTRIESDIALOG_H

#include <QDialog>
#include <QMessageBox>

struct Vehicle
{
    int id{-1};
    int userId{-1};
    QString brand;
    QString codeFipe;
    QString fuel;
    QString model;
    QString modelYear;
    QString price;
    QString monthReference;
    QString vehicleType;
    std::optional<double> pricePaid;
    std::optional<double> soldPrice;
};

struct Cost
{
    int id;
    int vehicleId;
    QString description;
    int price;
};

class DatabaseManager;
class MainWindow;

namespace Ui {
class CarRegistriesDialog;
}

class CarRegistriesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarRegistriesDialog(QWidget *parent = nullptr, std::shared_ptr<DatabaseManager> databaseManager = nullptr);
    ~CarRegistriesDialog();

public slots:
    void SaveCosts();
    void UpdateCosts();
    void DeleteCosts();
    void SaveVehicle();

private:
    Ui::CarRegistriesDialog *ui;
    std::shared_ptr<DatabaseManager> databaseManager;
    MainWindow *mainwindow;

    void FillVehiclesTable();
    void FillCostsTable();
};

#endif // CARREGISTRIESDIALOG_H
