#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "apimanager.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMessageBox>

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
    MainWindow(QWidget *parent = nullptr, std::shared_ptr<ApiManager> api = nullptr);
    ~MainWindow();

public slots:
    void OnLoginSuccessful();
    void enableConnects();
    void VehicleTypeChosen(int index);
    void BrandChosen(int index);
    void ModelChosen(int index);
    void YearReferenceChosen(int index);
    void GetFipeInfo();
    void GetModelPriceHistory();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<ApiManager> api;
    QString currentFipeCode;

    void SetupMonthReferences();
    void SetupVehicleType();
    QString GetVehicleTypeTranslated(const QString &vehicleType);
    void FillBrands(QJsonDocument &brands);
    void FillModels(QJsonDocument &models);
    void FillYearsByModel(QJsonDocument &years);
    void FillMonthReferences(QJsonDocument &monthReferences);
    void FillFipeInfo(QJsonDocument &fipeInfo);
    void FillModelPriceHistory(QJsonDocument &fipeInfo);

};
#endif // MAINWINDOW_H
