#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent, std::shared_ptr<DatabaseManager> databaseManager)
    : QDialog(parent), databaseManager(databaseManager)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    connect(ui->createAccountButton, &QPushButton::clicked, this, &Login::CreateAccount);
    connect(ui->loginButton, &QPushButton::clicked, this, &Login::CheckCredentials);

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

void Login::CreateAccount()
{
    bool success = this->databaseManager->CreateAccount(ui->userLineEdit->text(), ui->passwordLineEdit->text());
    if(success)
    {
        emit LoginSuccessful();
        this->close();
    }
    else
    {
        QMessageBox::warning(this, "ERRO", "Usuário já existe");
    }
}

void Login::CheckCredentials()
{
    bool success = this->databaseManager->CheckCredentials(ui->userLineEdit->text(), ui->passwordLineEdit->text());
    if(success)
    {
        emit LoginSuccessful();
        this->close();
    }
    else
    {
        QMessageBox::warning(this, "ERRO", "Usuário ou senha inválidos");
    }
}

