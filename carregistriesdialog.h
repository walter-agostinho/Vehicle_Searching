#ifndef CARREGISTRIESDIALOG_H
#define CARREGISTRIESDIALOG_H

#include <QDialog>
#include <QMessageBox>

struct Vehicle
{
    int id;
    int userId;
    QString brand;
    QString codeFipe;
    QString fuel;
    QString model;
    QString modelYear;
    QString price;
    QString monthReference;
    QString vehicleType;
    std::optional<int> pricePaid;
    std::optional<int> soldPrice;
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

private:
    Ui::CarRegistriesDialog *ui;
    std::shared_ptr<DatabaseManager> databaseManager;
    MainWindow *mainwindow;

    void FillCostsTable();
};

#endif // CARREGISTRIESDIALOG_H
