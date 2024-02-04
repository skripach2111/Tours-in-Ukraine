#include "addeditclientsdialog.h"
#include "ui_addeditclientsdialog.h"

AddEditClientsDialog::AddEditClientsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditClientsDialog)
{
    ui->setupUi(this);
}

AddEditClientsDialog::~AddEditClientsDialog()
{
    delete ui;
}

void AddEditClientsDialog::setDialog(clientObject data)
{
    ui->label_title->setText("Редагування");

    client = data;

    ui->lineEdit_pib->setText(client[ClientsModel::Column::PIB].toString());
    ui->lineEdit_phoneNumber->setText(client[ClientsModel::Column::PHONE_NUMBER].toString());
    ui->lineEdit_email->setText(client[ClientsModel::Column::EMAIL].toString());
}

clientObject AddEditClientsDialog::getData()
{
    return client;
}

bool AddEditClientsDialog::isSave()
{
    return save;
}

void AddEditClientsDialog::on_pushButton_save_clicked()
{
    client[ClientsModel::Column::PIB] = ui->lineEdit_pib->text();
    client[ClientsModel::Column::PHONE_NUMBER] = ui->lineEdit_phoneNumber->text();
    client[ClientsModel::Column::EMAIL] = ui->lineEdit_email->text();

    save = true;
    close();
}


void AddEditClientsDialog::on_pushButton_cancel_clicked()
{
    close();
}

