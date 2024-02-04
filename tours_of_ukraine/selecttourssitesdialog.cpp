#include "selecttourssitesdialog.h"
#include "ui_selecttourssitesdialog.h"

SelectToursSitesDialog::SelectToursSitesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectToursSitesDialog)
{
    ui->setupUi(this);

    filterModel = new TouristSitesFilterModel(this);
    ui->listView->setModel(filterModel);
}

SelectToursSitesDialog::~SelectToursSitesDialog()
{
    delete ui;
}

void SelectToursSitesDialog::setDialog(TouristSitesModel* touristSitesModel)
{
    model = touristSitesModel;
    filterModel->setSourceModel(model);
}

touristSiteObject SelectToursSitesDialog::getData()
{
    return object;
}

bool SelectToursSitesDialog::isSave()
{
    return save;
}

void SelectToursSitesDialog::on_pushButton_add_clicked()
{
    if(ui->listView->currentIndex().row() == -1)
        return;

    object = model->getDataByRow(filterModel->mapToSource(ui->listView->currentIndex()).row());

    save = true;
    close();
}


void SelectToursSitesDialog::on_pushButton_cancel_clicked()
{
    close();
}

