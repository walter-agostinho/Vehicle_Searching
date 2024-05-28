#include "carregistriesdialog.h"
#include "ui_carregistriesdialog.h"

CarRegistriesDialog::CarRegistriesDialog(QWidget *parent, std::shared_ptr<DatabaseManager> databaseManager)
    : QDialog(parent)
    , databaseManager(databaseManager)
    , ui(new Ui::CarRegistriesDialog)
{
    ui->setupUi(this);
}

CarRegistriesDialog::~CarRegistriesDialog()
{
    delete ui;
}
