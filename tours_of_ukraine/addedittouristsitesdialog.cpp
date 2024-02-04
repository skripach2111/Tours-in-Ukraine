#include "addedittouristsitesdialog.h"
#include "ui_addedittouristsitesdialog.h"

AddEditTouristSitesDialog::AddEditTouristSitesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditTouristSitesDialog)
{
    ui->setupUi(this);
}

AddEditTouristSitesDialog::~AddEditTouristSitesDialog()
{
    delete ui;
}

void AddEditTouristSitesDialog::setDialog(touristSiteObject data)
{
    ui->label_title->setText("Редагування");

    touristSite = data;

    QPixmap pixmap;
    pixmap.loadFromData(touristSite[TouristSitesModel::Column::IMAGE].toByteArray());
    pixmap = pixmap.scaled(150, 150);
    ui->label_image->setPixmap(pixmap);

    ui->lineEdit->setText(touristSite[TouristSitesModel::Column::TITLE].toString());
    ui->plainTextEdit->setPlainText(touristSite[TouristSitesModel::Column::DESCRIPTION].toString());
}

touristSiteObject AddEditTouristSitesDialog::getData()
{
    return touristSite;
}

bool AddEditTouristSitesDialog::isSave()
{
    return save;
}

void AddEditTouristSitesDialog::on_pushButton_3_clicked()
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
        touristSite[TouristSitesModel::Column::IMAGE] = arr;

        ui->label_image->setPixmap(image);
    }
}


void AddEditTouristSitesDialog::on_pushButton_save_clicked()
{
    touristSite[TouristSitesModel::Column::TITLE] = ui->lineEdit->text();
    touristSite[TouristSitesModel::Column::DESCRIPTION] = ui->plainTextEdit->toPlainText();

    save = true;
    close();
}


void AddEditTouristSitesDialog::on_pushButton_cancel_clicked()
{
    close();
}

