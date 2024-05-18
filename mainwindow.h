#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "apimanager.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

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
    void YearReferenceChosen(int index);

private:
    Ui::MainWindow *ui;
    std::shared_ptr<ApiManager> api;

    void SetupMonthReferences();
    void SetupVehicleType();
    QString GetVehicleTypeTranslated(const QString &vehicleType);
    void FillBrands(QJsonDocument &brands);
    void FillModels(QJsonDocument &models);
    void FillMonthReferences(QJsonDocument &yearReferences);


};
#endif // MAINWINDOW_H
