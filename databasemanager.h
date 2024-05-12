#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString.h>
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

private:
    void CreateDatabase();
    void ConnectDatabase();
    bool AreTablesCreated();
    void CreateTables();

};

#endif // DATABASEMANAGER_H
