#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, std::shared_ptr<ApiManager> api)
    : QMainWindow(parent)
    , api(api)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer::singleShot(2000, this, &MainWindow::enableConnects);

    this->SetupMonthReferences();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnLoginSuccessful()
{
    this->show();
}

void MainWindow::enableConnects()
{
    connect(ui->monthReferenceComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::YearReferenceChosen);
    connect(ui->vehicleTypeComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::VehicleTypeChosen);
    connect(ui->brandComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::BrandChosen);

    this->SetupVehicleType();
}

void MainWindow::VehicleTypeChosen(int index)
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        FillBrands(answer);
    };

    QString vehicleType = GetVehicleTypeTranslated(ui->vehicleTypeComboBox->currentText());
    this->api->GetBrands(vehicleType, callback);
}

void MainWindow::BrandChosen(int index)
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        FillModels(answer);
    };

    QString vehicleType = GetVehicleTypeTranslated(ui->vehicleTypeComboBox->currentText());
    QString brandId = ui->brandComboBox->currentData(Qt::UserRole).toString();
    if(!vehicleType.isEmpty() && !brandId.isEmpty())
    {
        this->api->GetModels(vehicleType, brandId, callback);
    }
}

void MainWindow::YearReferenceChosen(int index)
{
    this->VehicleTypeChosen(index);
    this->BrandChosen(index);
}

void MainWindow::SetupMonthReferences()
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        FillMonthReferences(answer);
    };
    this->api->GetMonthReferences(callback);
}

void MainWindow::SetupVehicleType()
{
    ui->vehicleTypeComboBox->addItems(QStringList() << "Carros" << "Motos" << "Caminhões");
    ui->vehicleTypeComboBox->setCurrentIndex(0);
}

QString MainWindow::GetVehicleTypeTranslated(const QString &vehicleType)
{
    if(vehicleType == "Motos")
    {
        return "motorcycles";
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

void MainWindow::FillBrands(QJsonDocument &brands)
{
    ui->brandComboBox->clear();
    if (!brands.isNull() && brands.isArray())
    {
        QJsonArray jsonArray = brands.array();

        QJsonValue value = jsonArray.first();
        for (const QJsonValue &value : jsonArray)
        {
            QString name = value["name"].toString();
            QString code = value["code"].toString();

            ui->brandComboBox->addItem(name, code);
        }
    }
}

void MainWindow::FillModels(QJsonDocument &models)
{
    ui->modelComboBox->clear();
    if (!models.isNull() && models.isArray())
    {
        QJsonArray jsonArray = models.array();

        QJsonValue value = jsonArray.first();
        for (const QJsonValue &value : jsonArray)
        {
            QString name = value["name"].toString();
            QString code = value["code"].toString();

            ui->modelComboBox->addItem(name, code);
        }
    }
}

void MainWindow::FillMonthReferences(QJsonDocument &yearReferences)
{
    ui->monthReferenceComboBox->clear();
    if (!yearReferences.isNull() && yearReferences.isArray())
    {
        ui->monthReferenceComboBox->addItem(QString("Todos"), QString(""));

        QJsonArray jsonArray = yearReferences.array();

        QJsonValue value = jsonArray.first();
        for (const QJsonValue &value : jsonArray)
        {
            QString month = value["month"].toString();
            QString code = value["code"].toString();

            ui->monthReferenceComboBox->addItem(month, code);
        }
    }
}
