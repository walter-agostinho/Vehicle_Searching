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
    connect(ui->modelComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::ModelChosen);

    this->SetupVehicleType();
}

void MainWindow::VehicleTypeChosen(int index)
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        FillBrands(answer);
    };

    QString vehicleType = GetVehicleTypeTranslated(ui->vehicleTypeComboBox->currentText());
    QString monthReference = ui->monthReferenceComboBox->currentData(Qt::UserRole).toString();
    if(!vehicleType.isEmpty())
    {
        this->api->GetBrands(vehicleType, monthReference, callback);
    }
}

void MainWindow::BrandChosen(int index)
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        FillModels(answer);
    };

    QString vehicleType = GetVehicleTypeTranslated(ui->vehicleTypeComboBox->currentText());
    QString brandId = ui->brandComboBox->currentData(Qt::UserRole).toString();
    QString monthReference = ui->monthReferenceComboBox->currentData(Qt::UserRole).toString();
    if(!vehicleType.isEmpty() && !brandId.isEmpty())
    {
        this->api->GetModels(vehicleType, brandId, monthReference, callback);
    }
}

void MainWindow::ModelChosen(int index)
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        FillYearsByModel(answer);
    };

    QString vehicleType = GetVehicleTypeTranslated(ui->vehicleTypeComboBox->currentText());
    QString brandId = ui->brandComboBox->currentData(Qt::UserRole).toString();
    QString modelId = ui->modelComboBox->currentData(Qt::UserRole).toString();
    QString monthReference = ui->monthReferenceComboBox->currentData(Qt::UserRole).toString();
    if(!vehicleType.isEmpty() && !brandId.isEmpty() && !modelId.isEmpty())
    {
        this->api->GetYearsByModel(vehicleType, brandId, modelId, monthReference, callback);
    }
}

void MainWindow::YearReferenceChosen(int index)
{
    this->VehicleTypeChosen(index);
    this->BrandChosen(index);
    this->ModelChosen(index);
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

void MainWindow::FillYearsByModel(QJsonDocument &years)
{
    ui->yearsByModelComboBox->clear();
    if (!years.isNull() && years.isArray())
    {
        QJsonArray jsonArray = years.array();

        QJsonValue value = jsonArray.first();
        for (const QJsonValue &value : jsonArray)
        {
            QString name = value["name"].toString();
            QString code = value["code"].toString();

            ui->yearsByModelComboBox->addItem(name, code);
        }
    }
}

void MainWindow::FillMonthReferences(QJsonDocument &monthReferences)
{
    ui->monthReferenceComboBox->clear();
    if (!monthReferences.isNull() && monthReferences.isArray())
    {
        ui->monthReferenceComboBox->addItem(QString("Todos"), QString(""));

        QJsonArray jsonArray = monthReferences.array();

        QJsonValue value = jsonArray.first();
        for (const QJsonValue &value : jsonArray)
        {
            QString month = value["month"].toString();
            QString code = value["code"].toString();

            ui->monthReferenceComboBox->addItem(month, code);
        }
    }
}
