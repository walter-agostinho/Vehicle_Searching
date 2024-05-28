#ifndef CARREGISTRIESDIALOG_H
#define CARREGISTRIESDIALOG_H

#include "databasemanager.h"
#include <QDialog>

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
