#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QSql>

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

private:
    void CreateDatabase();
    void ConnectDatabase();

};

#endif // DATABASEMANAGER_H
