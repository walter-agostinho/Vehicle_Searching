#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "carregistriesdialog.h"

MainWindow::MainWindow(QWidget *parent, std::shared_ptr<ApiManager> api, std::shared_ptr<DatabaseManager> databaseManager)
    : QMainWindow(parent)
    , api(api)
    , databaseManager(databaseManager)
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
    connect(ui->nextImageButton, &QPushButton::clicked, this, &MainWindow::nextCarImage);
    connect(ui->previousImageButton, &QPushButton::clicked, this, &MainWindow::previousCarImage);
    connect(ui->registriesButton, &QPushButton::clicked, this, &MainWindow::OpenRegistries);

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

void MainWindow::nextCarImage()
{
    auto it =  std::find(this->carImageLinks.begin(), this->carImageLinks.end(), currentImageLink);

    if(it == this->carImageLinks.end() || std::next(it) == this->carImageLinks.end())
    {
        QMessageBox::information(this, "Informação", "Chegou no limite de máximo de imagem");
        return;
    }
    auto next = std::next(it);

    currentImageLink = *next;
    if(!this->imagesMap.contains(currentImageLink))
    {
        this->GetCarImageFromLink();
    }
    else
    {
        this->ShowCarImage(this->imagesMap.value(*next));
    }
}

void MainWindow::previousCarImage()
{
    auto it =  std::find(this->carImageLinks.begin(), this->carImageLinks.end(), currentImageLink);

    if(it == this->carImageLinks.end() || it == this->carImageLinks.begin())
    {
        QMessageBox::information(this, "Informação", "Não é possível selecionar o anterior");
        return;
    }
    auto prev = std::prev(it);

    currentImageLink = *prev;
    if(!this->imagesMap.contains(currentImageLink))
    {
        this->GetCarImageFromLink();
    }
    else
    {
        this->ShowCarImage(this->imagesMap.value(*prev));
    }
}

void MainWindow::OpenRegistries()
{
    CarRegistriesDialog *carRegistries = new CarRegistriesDialog(this, databaseManager);
    carRegistries->show();
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
    QString search = ui->brandComboBox->currentText() + " " +
                     ui->modelComboBox->currentText() + " " +
                     ui->yearsByModelComboBox->currentText();

    this->api->GetCarImageLinks(search, callback);
}

void MainWindow::GetCarImageFromLink()
{
    ApiManager::ImageResponseCallback callback = [this](QByteArray answer)
    {
        this->ShowCarImage(answer);
        this->imagesMap.insert(currentImageLink, answer);
    };

    this->api->GetCarImage(currentImageLink, callback);
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

        Vehicle vehicle;
        vehicle.vehicleType = ui->vehicleTypeComboBox->currentText();
        vehicle.brand = jsonObject.value("brand").toString();
        vehicle.codeFipe = jsonObject.value("codeFipe").toString();
        vehicle.fuel = jsonObject.value("fuel").toString();
        vehicle.model = jsonObject.value("model").toString();
        vehicle.modelYear = QString::number(jsonObject.value("modelYear").toInt());
        vehicle.price = jsonObject.value("price").toString();
        vehicle.monthReference = jsonObject.value("referenceMonth").toString();
        this->SetVehicleInfo(vehicle);


        ui->vehiclePlainTextEdit->appendPlainText("Marca: " + vehicle.brand);
        ui->vehiclePlainTextEdit->appendPlainText("Codigo da Fipe: " + vehicle.codeFipe);
        ui->vehiclePlainTextEdit->appendPlainText("Combustível: " + vehicle.fuel);
        ui->vehiclePlainTextEdit->appendPlainText("Modelo: " + vehicle.model);
        ui->vehiclePlainTextEdit->appendPlainText("Ano do modelo: " + vehicle.modelYear);
        ui->vehiclePlainTextEdit->appendPlainText("Preço: " + vehicle.price);
        ui->vehiclePlainTextEdit->appendPlainText("Mês de Referência: " + vehicle.monthReference);

        this->currentFipeCode = vehicle.codeFipe;
        ui->priceLineEdit->setText(jsonObject.value("price").toString());
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
    this->currentImageLink.clear();
    if (!carImage.isNull() && carImage.isObject())
    {
        QJsonObject jsonObject = carImage.object();
        QJsonArray items = jsonObject["items"].toArray();

        for (const QJsonValue &value : items)
        {
            QString itemLink = value["link"].toString();
            this->carImageLinks.push_back(itemLink);
        }
        currentImageLink = this->carImageLinks.at(0);
        this->GetCarImageFromLink();
    }
}

void MainWindow::ShowCarImage(QByteArray img)
{
    QPixmap pixmap;
    pixmap.loadFromData(img);
    QSize desiredSize(400, 300);
    QPixmap scaledPixmap = pixmap.scaled(desiredSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->carImageLabel->setPixmap(scaledPixmap);
}

void MainWindow::SetVehicleInfo(const Vehicle &vehicle)
{
    this->currentVehicleInfo = vehicle;
}

void MainWindow::GetVehicleInfo(Vehicle &vehicle)
{
    vehicle = this->currentVehicleInfo;
}
