#ifndef CARREGISTRIESDIALOG_H
#define CARREGISTRIESDIALOG_H

#include <QDialog>

namespace Ui {
class CarRegistriesDialog;
}

class CarRegistriesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarRegistriesDialog(QWidget *parent = nullptr);
    ~CarRegistriesDialog();

private:
    Ui::CarRegistriesDialog *ui;
};

#endif // CARREGISTRIESDIALOG_H
