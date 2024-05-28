#ifndef CARREGISTRIESDIALOG_H
#define CARREGISTRIESDIALOG_H

#include "databasemanager.h"
#include <QDialog>

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
    QString priceHistory;
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

namespace Ui {
class CarRegistriesDialog;
}

class CarRegistriesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarRegistriesDialog(QWidget *parent = nullptr, std::shared_ptr<DatabaseManager> databaseManager = nullptr);
    ~CarRegistriesDialog();

private:
    Ui::CarRegistriesDialog *ui;
    std::shared_ptr<DatabaseManager> databaseManager;
};

#endif // CARREGISTRIESDIALOG_H
