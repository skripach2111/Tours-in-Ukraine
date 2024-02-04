#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!dbm.connect())
    {
        QMessageBox::critical(nullptr, "Критична помилка!", "Неможливо під'єднатися до бази даних.\nПрограма буде завершена.");
        close();
    }

    ui->stackedWidget->setCurrentIndex(PAGES::HELLO);
    dbm.selectTables();

    clientsFilterModel = new ClientsFilterModel(this);
    touristSitesFilterModel = new TouristSitesFilterModel(this);
    toursFilterModel = new ToursFilterModel(this);
    vauchersFilterModel = new VauchersFilterModel(this);

    clientsFilterModel->setSourceModel(dbm.clientsModel);
    touristSitesFilterModel->setSourceModel(dbm.touristSitesModel);
    toursFilterModel->setSourceModel(dbm.toursModel);
    vauchersFilterModel->setSourceModel(dbm.vauchersModel);

    ui->tableView_clients->setModel(clientsFilterModel);
    ui->listView_tourist_sites->setModel(touristSitesFilterModel);
    ui->tableView_tours->setModel(toursFilterModel);
    ui->tableView_vauchers->setModel(vauchersFilterModel);

    ui->tableView_zap_1->setModel(dbm.zap1);
    ui->tableView_zap_2->setModel(dbm.zap2);
    ui->tableView_zap_3->setModel(dbm.zap3);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_clients_triggered()
{
    ui->stackedWidget->setCurrentIndex(CLIENTS);

    ui->action_tours->setChecked(false);
    ui->action_tours_objects->setChecked(false);
    ui->action_vauchers->setChecked(false);
    ui->action_stats->setChecked(false);
}


void MainWindow::on_action_tours_objects_triggered()
{
    ui->stackedWidget->setCurrentIndex(TOURIST_SITES);

    ui->action_tours->setChecked(false);
    ui->action_clients->setChecked(false);
    ui->action_vauchers->setChecked(false);
    ui->action_stats->setChecked(false);
}


void MainWindow::on_action_tours_triggered()
{
    ui->stackedWidget->setCurrentIndex(TOURS);

    ui->action_clients->setChecked(false);
    ui->action_tours_objects->setChecked(false);
    ui->action_vauchers->setChecked(false);
    ui->action_stats->setChecked(false);
}


void MainWindow::on_action_vauchers_triggered()
{
    ui->stackedWidget->setCurrentIndex(VAUCHERS);

    ui->action_tours->setChecked(false);
    ui->action_tours_objects->setChecked(false);
    ui->action_clients->setChecked(false);
    ui->action_stats->setChecked(false);
}


void MainWindow::on_pushButton_clients_search_clicked()
{
    if(ui->lineEdit_pib_clients_search->text().size() == 0)
        clientsFilterModel->setEnabledFilterParam(ClientsFilterModel::FilterParam::PIB, false);
    else
    {
        clientsFilterModel->setEnabledFilterParam(ClientsFilterModel::FilterParam::PIB, true);
        clientsFilterModel->setFilterParam(ClientsFilterModel::FilterParam::PIB, ui->lineEdit_pib_clients_search->text());
    }
}


void MainWindow::on_pushButton_tourist_sites_search_clicked()
{
    if(ui->lineEdit_tourist_sites_title_search->text().size() > 0)
    {
        touristSitesFilterModel->setEnabledFilterParam(TouristSitesFilterModel::FilterParam::TITLE, true);
        touristSitesFilterModel->setFilterParam(TouristSitesFilterModel::FilterParam::TITLE, ui->lineEdit_tourist_sites_title_search->text());
    }
    else
        touristSitesFilterModel->setEnabledFilterParam(TouristSitesFilterModel::FilterParam::TITLE, false);
}


void MainWindow::on_pushButton_tours_search_clicked()
{
    if(ui->lineEdit_tours_title_search->text().size() > 0)
    {
        toursFilterModel->setEnabledFilterParam(ToursFilterModel::FilterParam::TITLE, true);
        toursFilterModel->setFilterParam(ToursFilterModel::FilterParam::TITLE, ui->lineEdit_tours_title_search->text());
    }
    else
        toursFilterModel->setEnabledFilterParam(ToursFilterModel::FilterParam::TITLE, false);
}


void MainWindow::on_pushButton_vauchers_search_clicked()
{
    if(ui->lineEdit_vauchers_id_search->text().size() > 0)
    {
        vauchersFilterModel->setEnabledFilterParam(VauchersFilterModel::FilterParam::ID, true);
        vauchersFilterModel->setFilterParam(VauchersFilterModel::FilterParam::ID, ui->lineEdit_vauchers_id_search->text());
    }
    else
        vauchersFilterModel->setEnabledFilterParam(VauchersFilterModel::FilterParam::ID, false);
}


void MainWindow::on_pushButton_vauchers_add_clicked()
{
    AddEditVauchersDialog dialog;

    dialog.setDialog(dbm.clientsModel, dbm.toursModel);
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.vauchersModel->appendRow(dialog.getData());
    dbm.vauchersModel->saveChanges();
    dbm.vauchersModel->select();
}


void MainWindow::on_pushButton_vauchers_edit_clicked()
{
    if(ui->tableView_vauchers->currentIndex().row() == -1)
        return;

    AddEditVauchersDialog dialog;

    dialog.setDialog(dbm.vauchersModel->getDataByRow(vauchersFilterModel->mapToSource(ui->tableView_vauchers->currentIndex()).row()), dbm.clientsModel, dbm.toursModel);
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.vauchersModel->updateRow(vauchersFilterModel->mapToSource(ui->tableView_vauchers->currentIndex()).row(), dialog.getData());
    dbm.vauchersModel->saveChanges();
    dbm.vauchersModel->select();
}


void MainWindow::on_pushButton_vauchers_delete_clicked()
{
    if(ui->tableView_vauchers->currentIndex().row() == -1)
        return;

    dbm.vauchersModel->removeRow(vauchersFilterModel->mapToSource(ui->tableView_vauchers->currentIndex()).row());
    dbm.vauchersModel->saveChanges();
    dbm.vauchersModel->select();
}


void MainWindow::on_pushButton_clients_add_clicked()
{
    AddEditClientsDialog dialog;

    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.clientsModel->appendRow(dialog.getData());
    dbm.clientsModel->saveChanges();
    dbm.clientsModel->select();
}


void MainWindow::on_pushButton_clients_edit_clicked()
{
    if(ui->tableView_clients->currentIndex().row() == -1)
        return;

    AddEditClientsDialog dialog;

    dialog.setDialog(dbm.clientsModel->getDataByRow(clientsFilterModel->mapToSource(ui->tableView_clients->currentIndex()).row()));
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.clientsModel->updateRow(clientsFilterModel->mapToSource(ui->tableView_clients->currentIndex()).row(), dialog.getData());
    dbm.clientsModel->saveChanges();
    dbm.clientsModel->select();
}


void MainWindow::on_pushButton_clients_delete_clicked()
{
    if(ui->tableView_clients->currentIndex().row() == -1)
        return;

    dbm.clientsModel->removeRow(clientsFilterModel->mapToSource(ui->tableView_clients->currentIndex()).row());
    dbm.clientsModel->saveChanges();
    dbm.clientsModel->select();
}


void MainWindow::on_pushButton_tourist_sites_add_clicked()
{
    AddEditTouristSitesDialog dialog;

    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.touristSitesModel->appendRow(dialog.getData());
    dbm.touristSitesModel->saveChanges();
    dbm.touristSitesModel->select();
}


void MainWindow::on_pushButton_tourist_sites_edit_clicked()
{
    if(ui->listView_tourist_sites->currentIndex().row() == -1)
        return;

    AddEditTouristSitesDialog dialog;

    dialog.setDialog(dbm.touristSitesModel->getDataByRow(touristSitesFilterModel->mapToSource(ui->listView_tourist_sites->currentIndex()).row()));
    dialog.exec();

    if(!dialog.isSave())
        return;

    dbm.touristSitesModel->updateRow(touristSitesFilterModel->mapToSource(ui->listView_tourist_sites->currentIndex()).row(), dialog.getData());
    dbm.touristSitesModel->saveChanges();
    dbm.touristSitesModel->select();
}


void MainWindow::on_pushButton_tourist_sites_delete_clicked()
{
    if(ui->listView_tourist_sites->currentIndex().row() == -1)
        return;

    dbm.touristSitesModel->removeRow(touristSitesFilterModel->mapToSource(ui->listView_tourist_sites->currentIndex()).row());
    dbm.touristSitesModel->saveChanges();
    dbm.touristSitesModel->select();
}


void MainWindow::on_pushButton_tours_add_clicked()
{
    AddEditToursDialog dialog;

    dialog.setDialog(dbm.touristSitesModel, dbm.touristSitesInTourModel, dbm.imageInToursModel);
    dialog.exec();

    dbm.toursModel->appendRow(dialog.getData());
    QVariant id = dbm.toursModel->saveChanges();
    dbm.toursModel->select();

    QList <touristSitesInTourObject> listToursSites = dialog.getTouristSitesData();
    for(int i = 0; i < listToursSites.size(); i++)
    {
        listToursSites[i][TouristSitesInTourModel::Column::ID_TOUR] = id;
        dbm.touristSitesInTourModel->appendRow(listToursSites[i]);
    }
    dbm.touristSitesInTourModel->saveChanges();
    dbm.touristSitesInTourModel->select();

    QList <imageInTourObject> listImages = dialog.getImagesData();
    for(int i = 0; i < listImages.size(); i++)
    {
        listImages[i][ImageInToursModel::Column::ID_TOUR] = id;
        dbm.imageInToursModel->appendRow(listImages[i]);
    }
    dbm.imageInToursModel->saveChanges();
    dbm.imageInToursModel->select();
}


void MainWindow::on_pushButton_tours_edit_clicked()
{
    if(ui->tableView_tours->currentIndex().row() == -1)
        return;

    AddEditToursDialog dialog;

    dialog.setDialog(dbm.toursModel->getDataByRow(toursFilterModel->mapToSource(ui->tableView_tours->currentIndex()).row()), dbm.touristSitesModel, dbm.touristSitesInTourModel, dbm.imageInToursModel);
    dialog.exec();

    QVariant id = dbm.toursModel->getDataByRow(toursFilterModel->mapToSource(ui->tableView_tours->currentIndex()).row())[ToursModel::Column::ID];
    dbm.toursModel->updateRow(toursFilterModel->mapToSource(ui->tableView_tours->currentIndex()).row(), dialog.getData());
    qDebug() << toursFilterModel->mapToSource(ui->tableView_tours->currentIndex()).row();
    dbm.toursModel->saveChanges();
    dbm.toursModel->select();

    dbm.touristSitesInTourModel->removeRow(id);
    dbm.touristSitesInTourModel->saveChanges();
    dbm.touristSitesInTourModel->select();
    QList <touristSitesInTourObject> listToursSites = dialog.getTouristSitesData();
    for(int i = 0; i < listToursSites.size(); i++)
    {
        listToursSites[i][TouristSitesInTourModel::Column::ID_TOUR] = id;
        dbm.touristSitesInTourModel->appendRow(listToursSites[i]);
    }
    dbm.touristSitesInTourModel->saveChanges();
    dbm.touristSitesInTourModel->select();

    dbm.imageInToursModel->removeRow(id);
    dbm.imageInToursModel->saveChanges();
    dbm.imageInToursModel->select();
    QList <imageInTourObject> listImages = dialog.getImagesData();
    for(int i = 0; i < listImages.size(); i++)
    {
        listImages[i][ImageInToursModel::Column::ID_TOUR] = id;
        dbm.imageInToursModel->appendRow(listImages[i]);
    }
    dbm.imageInToursModel->saveChanges();
    dbm.imageInToursModel->select();
}


void MainWindow::on_pushButton_tours_delete_clicked()
{
    if(ui->tableView_tours->currentIndex().row() == -1)
        return;

    dbm.toursModel->removeRow(toursFilterModel->mapToSource(ui->tableView_tours->currentIndex()).row());
    dbm.toursModel->saveChanges();
    dbm.toursModel->select();
}


void MainWindow::on_action_stats_triggered()
{
    ui->action_tours->setChecked(false);
    ui->action_tours_objects->setChecked(false);
    ui->action_clients->setChecked(false);
    ui->action_vauchers->setChecked(false);

    dbm.query_1.exec();
    dbm.query_2.exec();
    dbm.query_3.exec();

    qDebug() << dbm.zap1->lastError();

    ui->stackedWidget->setCurrentIndex(PAGES::STATISTIC);
}

