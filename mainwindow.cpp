#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, std::shared_ptr<ApiManager> api)
    : QMainWindow(parent)
    , api(api)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->vehicleTypeComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::VehicleTypeChosen);
    ui->vehicleTypeComboBox->addItems(QStringList() << "Carros" << "Motos" << "Caminhões");
    ui->vehicleTypeComboBox->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnLoginSuccessful()
{
    this->show();
}

void MainWindow::VehicleTypeChosen(int index)
{
    QString vehicleType = GetVehicleTypeTranslated(ui->vehicleTypeComboBox->currentText());
    this->api->GetBrands(vehicleType);
}

QString MainWindow::GetVehicleTypeTranslated(const QString &vehicleType)
{
    if(vehicleType == "Motos")
    {
        return "motocycles";
    }
    else if(vehicleType == "Caminhões")
    {
        return "trucks";
    }
    else
    {
        return "cars";
    }
}
