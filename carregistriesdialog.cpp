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

    this->FillVehiclesTable();

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

    vehicle.pricePaid = ui->pricePaidSpinBox->value();
    vehicle.soldPrice = ui->priceSoldSpinBox->value();
    int ret = this->databaseManager->SaveVehicle(vehicle, this->mainwindow->GetUser());
    if(ret < 0)
    {
        QMessageBox::warning(this, "Erro", "Não foi possível salvar o veículo");
        return;
    }
    else
    {
        QMessageBox::information(this, "Informação", "Veículo salvo");
    }
    this->FillVehiclesTable();
}

void CarRegistriesDialog::FillVehiclesTable()
{
    std::vector<Vehicle> vehicles;
    Vehicle vehicleFromMainwindow;

    this->mainwindow->GetVehicleInfo(vehicleFromMainwindow);

    int ret = this->databaseManager->GetVehiclesByFipeCode(vehicles, vehicleFromMainwindow.codeFipe, this->mainwindow->GetUser());
    if(ret < 0)
    {
        QMessageBox::warning(this, "Erro", "Não foi possível buscar os registro de veículo");
        return;
    }
    ui->vehiclesTableWidget->clear();
    ui->vehiclesTableWidget->setRowCount(0);
    ui->vehiclesTableWidget->setColumnCount(5);
    ui->vehiclesTableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "Modelo" <<
                                                       "Preço Fipe" << "Preço Pago" << "Preço Vendido");

    int row{0};
    for(auto &vehicle : vehicles)
    {
        ui->vehiclesTableWidget->insertRow(row);

        QTableWidgetItem *item;
        item = new QTableWidgetItem();
        item->setData(Qt::DisplayRole, QString::number(vehicle.id));
        ui->vehiclesTableWidget->setItem(row, 0, item);

        item = new QTableWidgetItem();
        item->setText(vehicle.model);
        ui->vehiclesTableWidget->setItem(row, 1, item);

        item = new QTableWidgetItem();
        item->setText((vehicle.price));
        ui->vehiclesTableWidget->setItem(row, 2, item);

        item = new QTableWidgetItem();
        item->setText(QString::number(vehicle.pricePaid.value_or(0.00),'g', 10));
        ui->vehiclesTableWidget->setItem(row, 3, item);

        item = new QTableWidgetItem();
        item->setText(QString::number(vehicle.soldPrice.value_or(0.00),'g', 10));
        ui->vehiclesTableWidget->setItem(row, 4, item);

        row++;
    }
    ui->vehiclesTableWidget->resizeColumnsToContents();
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
