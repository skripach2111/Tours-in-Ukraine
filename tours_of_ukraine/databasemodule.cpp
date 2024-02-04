#include "databasemodule.h"

#include <QDebug>

DatabaseModule::DatabaseModule(QObject *parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("tours_of_ukrain");
    db.setUserName("tours_of_ukrain_user");
    db.setPassword("tours_of_ukrain_user");
}

bool DatabaseModule::connect()
{
    if(db.open())
    {
        clientsModel = new ClientsModel(this);
        clientsModel->setTable("clients", &db);

        imageInToursModel = new ImageInToursModel(this);
        imageInToursModel->setTable("image_in_tours", &db);

        touristSitesInTourModel = new TouristSitesInTourModel(this);
        touristSitesInTourModel->setTable("tourist_sites_in_tours", &db);

        touristSitesModel = new TouristSitesModel(this);
        touristSitesModel->setTable("tourist_sites", &db);

        toursModel = new ToursModel(this);
        toursModel->setTable("tours", &db);

        vauchersModel = new VauchersModel(this);
        vauchersModel->setTable("vauchers", &db);
        vauchersModel->clients = clientsModel;
        vauchersModel->tours = toursModel;

        query_1 = QSqlQuery("SELECT\
                                t.title as 'Назва',\
                                COUNT(v.id) AS 'Кількість проданих ваучерів'\
                            FROM tours t\
                            JOIN vauchers v ON t.id = v.idTour\
                            WHERE t.isDelete = 0 AND v.isDelete = 0\
                            GROUP BY t.title\
                            ORDER BY COUNT(v.id) DESC;", db);
        zap1 = new QSqlTableModel(this);
        zap1->setQuery(query_1);

        query_2 = QSqlQuery("SELECT\
                                ts.title as 'Назва',\
                                COUNT(DISTINCT v.id) AS 'Кількість проданих квитків'\
                            FROM tourist_sites ts\
                            JOIN tourist_sites_in_tours tst ON ts.id = tst.idTouristSite\
                            JOIN vauchers v ON tst.idTour = v.idTour\
                            WHERE ts.isDelete = 0 AND v.isDelete = 0\
                            GROUP BY ts.title\
                            ORDER BY COUNT(DISTINCT v.id) DESC;", db);
        zap2 = new QSqlTableModel(this);
        zap2->setQuery(query_2);

        query_3 = QSqlQuery("SELECT\
                                t.title as 'Назва',\
                                SUM(v.sum) AS 'Загальний дохід'\
                            FROM tours t\
                            JOIN vauchers v ON t.id = v.idTour\
                            WHERE t.isDelete = 0 AND v.isDelete = 0\
                            GROUP BY t.title\
                            ORDER BY SUM(v.sum) DESC;", db);
        zap3 = new QSqlTableModel(this);
        zap3->setQuery(query_3);

        return true;
    }
    else
        return false;
}

void DatabaseModule::selectTables()
{
    clientsModel->select();
    imageInToursModel->select();
    touristSitesInTourModel->select();
    touristSitesModel->select();
    toursModel->select();
    vauchersModel->select();
}


