#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QSql>

class DatabaseManager
{
private:
    QString file;
    QSqlDatabase db;

public:
    DatabaseManager();
    void CreateDatabase();
    void ConnectDatabase();

signals:
    void ConnectionSuccessfull();
    void ConnectionFailed(const QString error);

};

#endif // DATABASEMANAGER_H
