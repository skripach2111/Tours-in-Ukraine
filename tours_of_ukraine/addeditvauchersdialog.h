#ifndef ADDEDITVAUCHERSDIALOG_H
#define ADDEDITVAUCHERSDIALOG_H

#include <QDialog>

#include "vauchersmodel.h"

#include "clientsfiltermodel.h"
#include "toursfiltermodel.h"

namespace Ui {
class AddEditVauchersDialog;
}

class AddEditVauchersDialog : public QDialog
{
    Q_OBJECT

    vaucherObject vaucher;

    ClientsModel* clients;
    ClientsFilterModel* clientsFilter;
    ToursModel* tours;
    ToursFilterModel* toursFilter;

    bool save = false;

public:
    explicit AddEditVauchersDialog(QWidget *parent = nullptr);
    ~AddEditVauchersDialog();

    void setDialog(ClientsModel* clientsModel, ToursModel* toursModel);
    void setDialog(vaucherObject data, ClientsModel* clientsModel, ToursModel* toursModel);

    vaucherObject getData();

    bool isSave();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddEditVauchersDialog *ui;
};

#endif // ADDEDITVAUCHERSDIALOG_H
