#ifndef ADDEDITCLIENTSDIALOG_H
#define ADDEDITCLIENTSDIALOG_H

#include <QDialog>

#include "clientsmodel.h"

namespace Ui {
class AddEditClientsDialog;
}

class AddEditClientsDialog : public QDialog
{
    Q_OBJECT

    bool save = false;
    clientObject client;

public:
    explicit AddEditClientsDialog(QWidget *parent = nullptr);
    ~AddEditClientsDialog();

    void setDialog(clientObject data);
    clientObject getData();

    bool isSave();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddEditClientsDialog *ui;
};

#endif // ADDEDITCLIENTSDIALOG_H
