#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, std::shared_ptr<ApiManager> api)
    : QMainWindow(parent)
    , api(api)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer::singleShot(2000, this, &MainWindow::enableConnects);

    ui->vehiclePlainTextEdit->setReadOnly(true);
    ui->priceHistoryPlainTextEdit->setReadOnly(true);
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
    connect(ui->searchFipeButton, &QPushButton::clicked, this, &MainWindow::GetFipeInfo);
    connect(ui->priceHistoryButton, &QPushButton::clicked, this, &MainWindow::GetModelPriceHistory);

    this->SetupVehicleType();
}

void MainWindow::VehicleTypeChosen(int index)
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        this->FillBrands(answer);
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
        this->FillModels(answer);
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
        this->FillYearsByModel(answer);
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

void MainWindow::GetFipeInfo()
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        this->FillFipeInfo(answer);
        this->GetCarImageLinks();
    };

    QString vehicleType = GetVehicleTypeTranslated(ui->vehicleTypeComboBox->currentText());
    QString brandId = ui->brandComboBox->currentData(Qt::UserRole).toString();
    QString modelId = ui->modelComboBox->currentData(Qt::UserRole).toString();
    QString yearId = ui->yearsByModelComboBox->currentData(Qt::UserRole).toString();
    QString monthReference = ui->monthReferenceComboBox->currentData(Qt::UserRole).toString();

    if(!vehicleType.isEmpty() && !brandId.isEmpty() && !modelId.isEmpty() && !yearId.isEmpty())
    {
        this->api->GetFipeInfo(vehicleType, brandId, modelId, monthReference, yearId, callback);
    }
}

void MainWindow::GetModelPriceHistory()
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        this->FillModelPriceHistory(answer);
    };

    QString vehicleType = GetVehicleTypeTranslated(ui->vehicleTypeComboBox->currentText());
    QString yearId = ui->yearsByModelComboBox->currentData(Qt::UserRole).toString();
    QString monthReference = ui->monthReferenceComboBox->currentData(Qt::UserRole).toString();

    if(this->currentFipeCode.isEmpty())
    {
        QMessageBox::information(this, "ATENÇÃO", "Necessário buscar a FIPE antes");
    }

    if(!vehicleType.isEmpty() && !yearId.isEmpty())
    {
        this->api->GetModelPriceHistory(vehicleType, this->currentFipeCode,  yearId, monthReference, callback);
    }
}

void MainWindow::SetupMonthReferences()
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        this->FillMonthReferences(answer);
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

void MainWindow::GetCarImageLinks()
{
    ApiManager::ResponseCallback callback = [this](QJsonDocument answer)
    {
        this->FillCarImageLinks(answer);
    };
    QString search = ui->brandComboBox->currentText() + " " + ui->modelComboBox->currentText();
    this->api->GetCarImageLinks(search, callback);
}

void MainWindow::GetCarImageFromLink()
{
    ApiManager::ImageResponseCallback callback = [this](QByteArray answer)
    {
        this->ShowCarImage(answer);
    };
    this->api->GetCarImage(carImageLinks.at(0), callback);
}

void MainWindow::FillBrands(QJsonDocument &brands)
{
    ui->brandComboBox->clear();
    if (!brands.isNull() && brands.isArray())
    {
        QJsonArray jsonArray = brands.array();

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

        for (const QJsonValue &value : jsonArray)
        {
            QString month = value["month"].toString();
            QString code = value["code"].toString();

            ui->monthReferenceComboBox->addItem(month, code);
        }
    }
}

void MainWindow::FillFipeInfo(QJsonDocument &fipeInfo)
{
    ui->vehiclePlainTextEdit->clear();
    if (!fipeInfo.isNull() && fipeInfo.isObject())
    {
        QJsonObject jsonObject = fipeInfo.object();

        this->currentFipeCode = jsonObject.value("codeFipe").toString();

        ui->vehiclePlainTextEdit->appendPlainText("Marca: " + jsonObject.value("brand").toString());
        ui->vehiclePlainTextEdit->appendPlainText("Codigo da Fipe: " + this->currentFipeCode);
        ui->vehiclePlainTextEdit->appendPlainText("Combustível: " + jsonObject.value("fuel").toString());
        ui->vehiclePlainTextEdit->appendPlainText("Modelo: " + jsonObject.value("model").toString());
        ui->vehiclePlainTextEdit->appendPlainText("Ano do modelo: " + QString::number(jsonObject.value("modelYear").toInt()));
        ui->vehiclePlainTextEdit->appendPlainText("Preço: " + jsonObject.value("price").toString());
        ui->vehiclePlainTextEdit->appendPlainText("Mês de Referência: " + jsonObject.value("referenceMonth").toString());
    }
}

void MainWindow::FillModelPriceHistory(QJsonDocument &fipeInfo)
{
    ui->priceHistoryPlainTextEdit->clear();
    if (!fipeInfo.isNull() && fipeInfo.isObject())
    {

        QJsonObject jsonObject = fipeInfo.object();
        QJsonArray priceHistory = jsonObject["priceHistory"].toArray();

        ui->priceHistoryPlainTextEdit->appendPlainText("Histórico de Preço");
        ui->priceHistoryPlainTextEdit->appendPlainText("");

        for (const QJsonValue &value : priceHistory)
        {
            QString price = value["price"].toString();
            QString month = value["month"].toString();

            ui->priceHistoryPlainTextEdit->appendPlainText("Preço: " + price);
            ui->priceHistoryPlainTextEdit->appendPlainText("Mês: " + month);
            ui->priceHistoryPlainTextEdit->appendPlainText("");
        }
    }
}

void MainWindow::FillCarImageLinks(QJsonDocument carImage)
{
    this->carImageLinks.clear();
    if (!carImage.isNull() && carImage.isObject())
    {
        QJsonObject jsonObject = carImage.object();
        QJsonArray items = jsonObject["items"].toArray();

        for (const QJsonValue &value : items)
        {
            QString itemLink = value["link"].toString();
            this->carImageLinks.push_back(itemLink);
        }
        this->GetCarImageFromLink();
    }
}

void MainWindow::ShowCarImage(QByteArray img)
{
    QPixmap pixmap;
    pixmap.loadFromData(img);
    ui->carImageLabel->setPixmap(pixmap);
}
