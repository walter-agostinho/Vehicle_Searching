#include "carregistriesdialog.h"
#include "ui_carregistriesdialog.h"
#include "mainwindow.h"
#include "databasemanager.h"

CarRegistriesDialog::CarRegistriesDialog(QWidget *parent, std::shared_ptr<DatabaseManager> databaseManager)
    : QDialog(parent)
    , databaseManager(databaseManager)
    , ui(new Ui::CarRegistriesDialog)
{
    ui->setupUi(this);
    this->mainwindow = qobject_cast<MainWindow *>(parent);
    connect(ui->saveButton, &QPushButton::clicked, this, &CarRegistriesDialog::SaveCosts);

}

CarRegistriesDialog::~CarRegistriesDialog()
{
    delete ui;
}

void CarRegistriesDialog::SaveCosts()
{
    if(ui->descriptionTextEdit->toPlainText().isEmpty() || ui->costPriceSpinBox->value() <= 0)
    {
        QMessageBox::information(this, "Atenção", "Descrição ou custo não preenchido");
        return;
    }

    Vehicle vehicle;
    mainwindow->GetVehicleInfo(vehicle);
}

void CarRegistriesDialog::UpdateCosts()
{

}

void CarRegistriesDialog::DeleteCosts()
{

}

void CarRegistriesDialog::FillCostsTable()
{
    std::vector<Cost> costs;
    this->databaseManager->GetVehicleCosts(costs);

    ui->costsTableWidget->clear();
    ui->costsTableWidget->setRowCount(costs.size());
    ui->costsTableWidget->setHorizontalHeaderLabels(QStringList() << "Id" << "Descrição" << "Preço");

    int row{0};
    for(auto &cost : costs)
    {
        QTableWidgetItem *item;
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, QString::number(cost.id));
        ui->costsTableWidget->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setText(cost.description);
        ui->costsTableWidget->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setText(QString::number(cost.price));
        ui->costsTableWidget->setItem(row, 2, item);

        row++;
    }
}
