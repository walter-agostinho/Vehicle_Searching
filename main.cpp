#include "databasemanager.h"
#include "login.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<DatabaseManager> databaseManager = std::make_shared<DatabaseManager>();
    Login login(nullptr, databaseManager);
    login.show();
    // MainWindow w;
    // w.show();
    return a.exec();
}
