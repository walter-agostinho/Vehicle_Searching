#include "databasemanager.h"
#include "login.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::shared_ptr<DatabaseManager> databaseManager = std::make_shared<DatabaseManager>();

    Login login(nullptr, databaseManager);
    MainWindow w;

    QObject::connect(&login, &Login::LoginSuccessful, &w, &MainWindow::OnLoginSuccessful);

    login.show();

    return a.exec();
}
