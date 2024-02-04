#include "addedittoursdialog.h"
#include "ui_addedittoursdialog.h"

AddEditToursDialog::AddEditToursDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditToursDialog)
{
    ui->setupUi(this);
}

AddEditToursDialog::~AddEditToursDialog()
{
    delete ui;
}

void AddEditToursDialog::setDialog(TouristSitesModel *touristSitesModel, TouristSitesInTourModel *touristSitesInTourModel, ImageInToursModel *imageInToursModel)
{
    touristSites = touristSitesModel;
    touristSitesInTour = touristSitesInTourModel;
    imageInTours = imageInToursModel;
}

void AddEditToursDialog::setDialog(tourObject data, TouristSitesModel *touristSitesModel, TouristSitesInTourModel *touristSitesInTourModel, ImageInToursModel *imageInToursModel)
{
    ui->label_title->setText("Редагування");
    tour = data;
    setDialog(touristSitesModel, touristSitesInTourModel, imageInToursModel);

    listTouristSites = touristSitesInTour->getDataByTourId(data[ToursModel::Column::ID].toInt());
    listImages = imageInToursModel->getDataByIdTour(data[ToursModel::Column::ID].toInt());

    updateImagesList();
    updateTouristSitesList();

    ui->plainTextEdit->setPlainText(data[ToursModel::Column::DESCRIPTION].toString());
    ui->dateEdit_date_event->setDate(data[ToursModel::Column::DATE_EVENT].toDate());
    ui->spinBox_seats->setValue(data[ToursModel::Column::NUMBER_SEATS].toInt());
    ui->comboBox_type->setCurrentText(data[ToursModel::Column::TYPE].toString());
    ui->doubleSpinBox_duration->setValue(data[ToursModel::Column::DURATION].toDouble());
    ui->doubleSpinBox_cost->setValue(data[ToursModel::Column::COST].toDouble());
    ui->lineEdit->setText(data[ToursModel::Column::TITLE].toString());
}

tourObject AddEditToursDialog::getData()
{
    return tour;
}

QList<touristSitesInTourObject> AddEditToursDialog::getTouristSitesData()
{
    return listTouristSites;
}

QList<imageInTourObject> AddEditToursDialog::getImagesData()
{
    return listImages;
}

void AddEditToursDialog::updateTouristSitesList()
{
    ui->listWidget_toursSites->clear();

    for(int i = 0; i < listTouristSites.size(); i++)
        ui->listWidget_toursSites->insertItem(i, touristSites->getDataById(listTouristSites[i][TouristSitesInTourModel::Column::ID_TOURIST_SITE].toInt(), TouristSitesModel::Column::TITLE).toString());
}

void AddEditToursDialog::updateImagesList()
{
    ui->listWidget_image->clear();

    for(int i = 0; i < listImages.size(); i++)
    {
        QPixmap pixmap;
        pixmap.loadFromData(listImages[i][ImageInToursModel::Column::IMAGE].toByteArray());
        pixmap = pixmap.scaled(100, 100);
        ui->listWidget_image->insertItem(i, "");
        ui->listWidget_image->item(i)->setIcon(QIcon(pixmap));
    }
}

void AddEditToursDialog::on_pushButton_toursSites_add_clicked()
{
    SelectToursSitesDialog dialog;

    dialog.setDialog(touristSites);
    dialog.exec();

    if(!dialog.isSave())
        return;

    touristSitesInTourObject tmp;
    tmp[TouristSitesInTourModel::Column::ID_TOURIST_SITE] = dialog.getData()[TouristSitesModel::Column::ID];
    tmp[TouristSitesInTourModel::Column::NUMBER] = 0;
    listTouristSites.append(tmp);
    updateTouristSitesList();
}


void AddEditToursDialog::on_pushButton_toursSites_del_clicked()
{
    if(ui->listWidget_toursSites->currentIndex().row() == -1)
        return;

    listTouristSites.removeAt(ui->listWidget_toursSites->currentIndex().row());
    updateTouristSitesList();
}


void AddEditToursDialog::on_pushButton_image_del_clicked()
{
    if(ui->listWidget_image->currentIndex().row() == -1)
        return;

    listImages.removeAt(ui->listWidget_image->currentIndex().row());
    updateImagesList();
}


void AddEditToursDialog::on_pushButton_image_add_clicked()
{
    QFileDialog fileDialog;

    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("Images (*.png *.jpg *.bmp *.gif)");
    fileDialog.exec();

    if(!fileDialog.selectedFiles().empty())
    {
        QPixmap image = QPixmap(fileDialog.selectedFiles().at(0)).scaled(150, 150);
        QByteArray arr;
        QBuffer buffer(&arr);
        buffer.open(QIODevice::ReadWrite);
        image.save(&buffer, "PNG");
        imageInTourObject tmp;
        tmp[ImageInToursModel::Column::IMAGE] = arr;
        listImages.append(tmp);
        updateImagesList();
    }
}


void AddEditToursDialog::on_pushButton_save_clicked()
{
    tour[ToursModel::Column::TITLE] = ui->lineEdit->text();
    tour[ToursModel::Column::DESCRIPTION] = ui->plainTextEdit->toPlainText();
    tour[ToursModel::Column::DATE_EVENT] = ui->dateEdit_date_event->date();
    tour[ToursModel::Column::DURATION] = ui->doubleSpinBox_duration->value();
    tour[ToursModel::Column::NUMBER_SEATS] = ui->spinBox_seats->value();
    tour[ToursModel::Column::TYPE] = ui->comboBox_type->currentText();
    tour[ToursModel::Column::COST] = ui->doubleSpinBox_cost->value();

    save = true;
    close();
}


void AddEditToursDialog::on_pushButton_cancel_clicked()
{
    close();
}

