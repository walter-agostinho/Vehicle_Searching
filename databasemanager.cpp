#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
    this->CreateDatabase();
    this->ConnectDatabase();
    this->CreateTables();
}

void DatabaseManager::GetConnectionStatus(QString &status)
{
    status = this->connectionStatus;
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
        qDebug() << "DATABASE CONNECTED";
        this->connectionStatus = "CONECTADO";
    }
    else
    {
        qDebug() << "ERROR = " << this->db.lastError().databaseText();
        this->connectionStatus = "DESCONECTADO - ERRO: " + this->db.lastError().databaseText();;
    }
}

void DatabaseManager::CreateTables()
{
    QSqlQuery query(this->db);
    query.prepare("SELECT name FROM sqlite_master WHERE type='table' "
                  "AND name=users AND name=vehicles");
    if(query.exec())
    {
        int tableCount{0};
        for(int i=0; i<TABLE_COUNT; i++)
        {
            query.next();
            tableCount++;
        }

        if(tableCount == TABLE_COUNT)
        {
            qDebug() << "Tables already created";
        }
    }
    else
    {
    query.prepare("CREATE TABLE users ("
                  "id INT PRIMARY KEY AUTO_INCREMENT,"
                  "user VARCHAR NOT NULL UNIQUE, "
                  "pass VARCHAR NOT NULL, "
                  "date_creation DATETIME DEFAULT CURRENT_TIMESTAMP"
                  ");");

    if(query.exec())
    {
        qDebug() << "users table created";
    }

    query.prepare("CREATE TABLE vehicles ("
                  "id INT PRIMARY KEY AUTO_INCREMENT,"
                  "brand VARCHAR NOT NULL UNIQUE, "
                  "code_fipe VARCHAR NOT NULL, "
                  "fuel VARCHAR NOT NULL, "
                  "model VARCHAR NOT NULL, "
                  "model_year VARCHAR NOT NULL, "
                  "price VARCHAR NOT NULL, "
                  "price_history VARCHAR NULL, "
                  "month_reference VARCHAR NOT NULL, "
                  "vehicle_type VARCHAR NOT NULL, "
                  ");");

    if(query.exec())
    {
        qDebug() << "vehicles table created";
    }

    query.prepare("CREATE TABLE costs ("
                  "id INT PRIMARY KEY AUTO_INCREMENT,"
                  "description VARCHAR NOT NULL UNIQUE, "
                  "price INT NOT NULL, "
                  "date_creation DATETIME DEFAULT CURRENT_TIMESTAMP"
                  ");");

    if(query.exec())
    {
        qDebug() << "costs table created";
    }

}
