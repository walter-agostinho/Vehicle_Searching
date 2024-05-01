#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
    this->CreateDatabase();
    this->ConnectDatabase();
}

void DatabaseManager::CreateDatabase()
{
    QDir dir;
    if(!dir.exists("db"))
    {
        dir.mkdir("db");
        qDebug() << "directory created";
    }


    this->file = "db/database.db";
    QFile file(this->file);

    if(!file.exists())
    {
        file.open(QIODevice::ReadWrite);
        qDebug() << "file created";
    }
    else
    {
        qDebug() << "file alrealdy exists";
    }
    file.close();
}

void DatabaseManager::ConnectDatabase()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName("C:/Users/walte/Documents/Dev/Vehicle_Searching/build/Desktop_Qt_6_6_3_MinGW_64_bit-Debug/db/database.db");
    if(db.open())
    {
        qDebug() << " DATABASE CONNECTED";
        //emit ConnectionSuccessfull();
    }
    else
    {
        qDebug() << "ERROR = " << this->db.lastError().databaseText();
        //emit ConnectionFailed(this->db.lastError().databaseText());
    }
}
