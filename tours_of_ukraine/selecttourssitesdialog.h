#ifndef SELECTTOURSSITESDIALOG_H
#define SELECTTOURSSITESDIALOG_H

#include <QDialog>

#include "touristsitesfiltermodel.h"

namespace Ui {
class SelectToursSitesDialog;
}

class SelectToursSitesDialog : public QDialog
{
    Q_OBJECT

    touristSiteObject object;
    TouristSitesFilterModel* filterModel;
    TouristSitesModel* model;

    bool save = false;

public:
    explicit SelectToursSitesDialog(QWidget *parent = nullptr);
    ~SelectToursSitesDialog();

    void setDialog(TouristSitesModel* touristSitesModel);
    touristSiteObject getData();

    bool isSave();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::SelectToursSitesDialog *ui;
};

#endif // SELECTTOURSSITESDIALOG_H
