#include "carregistriesdialog.h"
#include "ui_carregistriesdialog.h"

CarRegistriesDialog::CarRegistriesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CarRegistriesDialog)
{
    ui->setupUi(this);
}

CarRegistriesDialog::~CarRegistriesDialog()
{
    delete ui;
}
