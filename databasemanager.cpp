#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
    this->CreateDatabase();
    this->ConnectDatabase();
    if(!this->AreTablesCreated())
    {
      this->CreateTables();
    }

}

void DatabaseManager::GetConnectionStatus(QString &status)
{
    status = this->connectionStatus;
}

bool DatabaseManager::CreateAccount(const QString &user, const QString &password)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    QString hashedPassword = hash.result().toHex();

    QSqlQuery query(this->db);
    query.prepare("INSERT INTO users(user, password) VALUES (:user, :password);");
    query.bindValue(":user", user);
    query.bindValue(":password", hashedPassword);

    if(!query.exec())
    {
        qDebug() << "CreateAccount - ERRO =" << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::CheckCredentials(const QString &user, const QString &password)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    QString hashedPassword = hash.result().toHex();

    QSqlQuery query(this->db);
    query.prepare("SELECT COUNT(*) FROM users WHERE user=:user AND password=:password;");
    query.bindValue(":user", user);
    query.bindValue(":password", hashedPassword);

    if(query.exec())
    {
        query.next();
        return query.value(0).toBool();
    }
    else
    {
        qDebug() << "CheckCredentials - ERRO =" << query.lastError();
        return false;
    }
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

bool DatabaseManager::AreTablesCreated()
{
    QSqlQuery query(this->db);
    query.prepare("SELECT COUNT(*) FROM sqlite_master WHERE type='table';");

    if(query.exec())
    {
        query.next();
        if(query.value(0) == TABLE_COUNT)
        {
            qDebug() << "Tables already created";
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        qDebug() << "AreTablesCreated - ERRO =" << query.lastError();
        return false;
    }
}

void DatabaseManager::CreateTables()
{
    QSqlQuery query(this->db);
    query.prepare("CREATE TABLE users ("
                  "id INTEGER PRIMARY KEY, "
                  "user VARCHAR NOT NULL UNIQUE, "
                  "password VARCHAR NOT NULL, "
                  "date_creation DATETIME DEFAULT CURRENT_TIMESTAMP "
                  ");");

    if(!query.exec())
    {
        qDebug() << "CreateTables - ERRO =" << query.lastError();
    }

    query.prepare("CREATE TABLE vehicles ("
                  "id INTEGER PRIMARY KEY, "
                  "user_id INTEGER NOT NULL, "
                  "brand VARCHAR NOT NULL UNIQUE, "
                  "code_fipe VARCHAR NOT NULL, "
                  "fuel VARCHAR NOT NULL, "
                  "model VARCHAR NOT NULL, "
                  "model_year VARCHAR NOT NULL, "
                  "price VARCHAR NOT NULL, "
                  "price_history VARCHAR NULL, "
                  "month_reference VARCHAR NOT NULL, "
                  "vehicle_type VARCHAR NOT NULL, "
                  "price_paid INTERGER NULL, "
                  "sold_price INTERGER NULL, "
                  "FOREIGN KEY (user_id) REFERENCES users(id) "
                  ");");

    if(!query.exec())
    {
        qDebug() << "CreateTables - ERRO =" << query.lastError();
    }

    query.prepare("CREATE TABLE costs ("
                  "id INTEGER PRIMARY KEY, "
                  "vehicle_id INTEGER NOT NULL, "
                  "description VARCHAR NOT NULL, "
                  "price INTEGER NOT NULL, "
                  "date_creation DATETIME DEFAULT CURRENT_TIMESTAMP, "
                  "FOREIGN KEY (vehicle_id) REFERENCES vehicles(id) "
                  ");");

    if(!query.exec())
    {
        qDebug() << "CreateTables - ERRO =" << query.lastError();
    }

}
