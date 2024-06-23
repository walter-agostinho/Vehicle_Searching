#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "apimanager.h"
#include "carregistriesdialog.h"
#include "databasemanager.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMessageBox>
#include <QMap>

class ApiManager;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, std::shared_ptr<ApiManager> api = nullptr,
               std::shared_ptr<DatabaseManager> databaseManager = nullptr);

    ~MainWindow();

    void GetVehicleInfo(Vehicle& vehicle);
    QString GetUser() const;

public slots:
    void OnLoginSuccessful(const QString &user);
    void enableConnects();
    void VehicleTypeChosen(int index);
    void BrandChosen(int index);
    void ModelChosen(int index);
    void YearReferenceChosen(int index);
    void GetFipeInfo();
    void GetModelPriceHistory();
    void nextCarImage();
    void previousCarImage();
    void OpenRegistries();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<ApiManager> api;
    std::shared_ptr<DatabaseManager> databaseManager;
    QString currentFipeCode;
    std::vector<QString> carImageLinks;
    QString currentImageLink;
    QMap<QString, QByteArray> imagesMap;
    Vehicle currentVehicleInfo;
    QString user;

    void SetupMonthReferences();
    void SetupVehicleType();
    QString GetVehicleTypeTranslated(const QString &vehicleType);
    void GetCarImageLinks();
    void GetCarImageFromLink();
    void FillBrands(QJsonDocument &brands);
    void FillModels(QJsonDocument &models);
    void FillYearsByModel(QJsonDocument &years);
    void FillMonthReferences(QJsonDocument &monthReferences);
    void FillFipeInfo(QJsonDocument &fipeInfo);
    void FillModelPriceHistory(QJsonDocument &fipeInfo);
    void FillCarImageLinks(QJsonDocument carImage);
    void ShowCarImage(QByteArray img);

    void SetVehicleInfo(const Vehicle& vehicle);
};
#endif // MAINWINDOW_H
