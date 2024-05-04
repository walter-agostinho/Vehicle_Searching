#ifndef LOGIN_H
#define LOGIN_H

#include "databasemanager.h"
#include <memory.h>
#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr, std::shared_ptr<DatabaseManager> databaseManager = nullptr);
    ~Login();


private:
    Ui::Login *ui;
    std::shared_ptr<DatabaseManager> databaseManager;
    void ShowConnectionStatus();

};

#endif // LOGIN_H
