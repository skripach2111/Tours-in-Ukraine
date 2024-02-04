#include "addeditvauchersdialog.h"
#include "ui_addeditvauchersdialog.h"

AddEditVauchersDialog::AddEditVauchersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditVauchersDialog)
{
    ui->setupUi(this);

    clientsFilter = new ClientsFilterModel(this);
    toursFilter = new ToursFilterModel(this);

    ui->comboBox_tours->setModel(toursFilter);
    ui->comboBox_clients->setModel(clientsFilter);
}

AddEditVauchersDialog::~AddEditVauchersDialog()
{
    delete ui;
}

void AddEditVauchersDialog::setDialog(ClientsModel *clientsModel, ToursModel *toursModel)
{
    clients = clientsModel;
    tours = toursModel;

    clientsFilter->setSourceModel(clients);
    toursFilter->setSourceModel(tours);
}

void AddEditVauchersDialog::setDialog(vaucherObject data, ClientsModel *clientsModel, ToursModel *toursModel)
{
    ui->label_title->setText("Редагування");

    vaucher = data;
    setDialog(clientsModel, toursModel);

    ui->comboBox_clients->setCurrentIndex(clientsFilter->mapFromSource(clients->index(clients->getRowByData(vaucher[VauchersModel::Column::ID_CLIENT], ClientsModel::Column::ID), ClientsModel::Column::PIB)).row());
    ui->comboBox_tours->setCurrentIndex(toursFilter->mapFromSource(tours->index(tours->getRowByData(vaucher[VauchersModel::Column::ID_TOUR], ToursModel::Column::ID), ToursModel::Column::TITLE)).row());
    ui->comboBox_3->setCurrentText(vaucher[VauchersModel::Column::PAYMENT_STATUS].toString());
    ui->spinBox->setValue(vaucher[VauchersModel::Column::NUMBER_SEATS].toInt());
    ui->doubleSpinBox->setValue(vaucher[VauchersModel::Column::SUM].toDouble());
    ui->dateEdit->setDate(vaucher[VauchersModel::Column::BOOKING_DATE].toDate());
}

vaucherObject AddEditVauchersDialog::getData()
{
    return vaucher;
}

bool AddEditVauchersDialog::isSave()
{
    return save;
}

void AddEditVauchersDialog::on_pushButton_clicked()
{
    vaucher[VauchersModel::Column::ID_CLIENT] = clients->index(clientsFilter->mapToSource(clientsFilter->index(ui->comboBox_clients->currentIndex(), 0)).row(), ClientsModel::Column::ID).data(ClientsModel::Role::Read);
    vaucher[VauchersModel::Column::ID_TOUR] = tours->index(toursFilter->mapToSource(toursFilter->index(ui->comboBox_tours->currentIndex(), 0)).row(), ToursModel::Column::ID).data(ToursModel::Role::Read);
    vaucher[VauchersModel::Column::PAYMENT_STATUS] = ui->comboBox_3->currentText();
    vaucher[VauchersModel::Column::NUMBER_SEATS] = ui->spinBox->value();
    vaucher[VauchersModel::Column::SUM] = ui->doubleSpinBox->value();
    vaucher[VauchersModel::Column::BOOKING_DATE] = ui->dateEdit->date();

    save = true;
    close();
}


void AddEditVauchersDialog::on_pushButton_2_clicked()
{
    close();
}

