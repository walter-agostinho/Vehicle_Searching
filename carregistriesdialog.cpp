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
    connect(ui->saveCostsButton, &QPushButton::clicked, this, &CarRegistriesDialog::SaveCosts);
    connect(ui->saveVehicleButton, &QPushButton::clicked, this, &CarRegistriesDialog::SaveVehicle);

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


}

void CarRegistriesDialog::UpdateCosts()
{

}

void CarRegistriesDialog::DeleteCosts()
{

}

void CarRegistriesDialog::SaveVehicle()
{
    Vehicle vehicle;
    this->mainwindow->GetVehicleInfo(vehicle);
    if(vehicle.codeFipe.isEmpty())
    {
        QMessageBox::information(this, "Atenção", "Veículo não foi buscado");
        return;
    }
    if(ui->pricePaidSpinBox->value() <= 0)
    {
        QMessageBox::information(this, "Atenção", "Necessário pelo menos informar o valor pago");
        return;
    }
    else
    {
        vehicle.pricePaid = ui->pricePaidSpinBox->value();
        if(ui->priceSoldSpinBox->value() > 0)
        {
            vehicle.soldPrice = ui->priceSoldSpinBox->value();
        }
        int ret = this->databaseManager->SaveVehicle(vehicle, this->mainwindow->GetUser());
        if(ret < 0)
        {
            QMessageBox::warning(this, "Erro", "Não foi possível salvar o veículo");
        }
        else
        {
            QMessageBox::information(this, "Informação", "Veículo salvo");
        }
    }
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
