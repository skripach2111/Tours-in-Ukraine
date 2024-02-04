#ifndef ADDEDITTOURISTSITESDIALOG_H
#define ADDEDITTOURISTSITESDIALOG_H

#include <QDialog>
#include <QBuffer>
#include <QFileDialog>

#include "touristsitesmodel.h"

namespace Ui {
class AddEditTouristSitesDialog;
}

class AddEditTouristSitesDialog : public QDialog
{
    Q_OBJECT

    touristSiteObject touristSite;

    bool save = false;

public:
    explicit AddEditTouristSitesDialog(QWidget *parent = nullptr);
    ~AddEditTouristSitesDialog();

    void setDialog(touristSiteObject data);
    touristSiteObject getData();

    bool isSave();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddEditTouristSitesDialog *ui;
};

#endif // ADDEDITTOURISTSITESDIALOG_H
