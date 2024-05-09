#include "login.h"
#include "ui_login.h"
#include <QSqlRelation>

Login::Login(QWidget *parent, std::shared_ptr<DatabaseManager> databaseManager)
    : QDialog(parent), databaseManager(databaseManager)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    this->ShowConnectionStatus();
}

Login::~Login()
{
    delete ui;
}

void Login::ShowConnectionStatus()
{
    QString statusConnection;
    this->databaseManager->GetConnectionStatus(statusConnection);
    ui->statusConnection->setText(statusConnection);
}

