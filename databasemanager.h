#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "carregistriesdialog.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QCryptographicHash>

const int TABLE_COUNT = 3;

class DatabaseManager : public QObject
{
    Q_OBJECT

private:
    QString file;
    QSqlDatabase db;
    QString connectionStatus;

public:
    void extracted();
    DatabaseManager();
    void GetConnectionStatus(QString &status);
    bool CreateAccount(const QString &user, const QString &password);
    bool CheckCredentials(const QString &user, const QString &password);
    void GetVehicleCosts(std::vector<Cost> costs);
    int SaveVehicle(const Vehicle &vehicle, const QString &user);
    int GetCurrentUserId(QString user);

private:
    void CreateDatabase();
    void ConnectDatabase();
    bool AreTablesCreated();
    void CreateTables();

};

#endif // DATABASEMANAGER_H
