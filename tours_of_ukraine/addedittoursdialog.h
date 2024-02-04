#ifndef ADDEDITTOURSDIALOG_H
#define ADDEDITTOURSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QBuffer>

#include "touristsitesmodel.h"
#include "imageintoursmodel.h"
#include "touristsitesintourmodel.h"
#include "toursmodel.h"

#include "selecttourssitesdialog.h"

namespace Ui {
class AddEditToursDialog;
}

class AddEditToursDialog : public QDialog
{
    Q_OBJECT

    TouristSitesModel* touristSites;
    TouristSitesInTourModel* touristSitesInTour;
    ImageInToursModel* imageInTours;

    tourObject tour;

    QList <touristSitesInTourObject> listTouristSites;
    QList <imageInTourObject> listImages;

    bool save = true;

public:
    explicit AddEditToursDialog(QWidget *parent = nullptr);
    ~AddEditToursDialog();

    void setDialog(TouristSitesModel* touristSitesModel, TouristSitesInTourModel* touristSitesInTourModel, ImageInToursModel* imageInToursModel);
    void setDialog(tourObject data, TouristSitesModel* touristSitesModel, TouristSitesInTourModel* touristSitesInTourModel, ImageInToursModel* imageInToursModel);

    tourObject getData();
    QList <touristSitesInTourObject> getTouristSitesData();
    QList <imageInTourObject> getImagesData();

    bool isSave();

private slots:
    void on_pushButton_toursSites_add_clicked();

    void on_pushButton_toursSites_del_clicked();

    void on_pushButton_image_del_clicked();

    void on_pushButton_image_add_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddEditToursDialog *ui;

    void updateTouristSitesList();
    void updateImagesList();
};

#endif // ADDEDITTOURSDIALOG_H
