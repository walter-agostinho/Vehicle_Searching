#include "databasemanager.h"
#include "login.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DatabaseManager sqlite;

    Login login;
    login.show();
    // MainWindow w;
    // w.show();
    return a.exec();
}
