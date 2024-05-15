#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "apimanager.h"

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
    void VehicleTypeChosen(int index);

private:
    Ui::MainWindow *ui;
    std::shared_ptr<ApiManager> api;

    QString GetVehicleTypeTranslated(const QString &vehicleType);


};
#endif // MAINWINDOW_H
