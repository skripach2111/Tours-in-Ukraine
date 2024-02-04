#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H


#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>

#include "clientsmodel.h"
#include "imageintoursmodel.h"
#include "touristsitesintourmodel.h"
#include "touristsitesmodel.h"
#include "toursmodel.h"
#include "vauchersmodel.h"

class DatabaseModule : public QObject
{
    Q_OBJECT

    QSqlDatabase db;

public:
    DatabaseModule(QObject *parent = nullptr);

    bool connect();
    void disconnect() { db.close(); }

    QSqlError lastError() { return db.lastError(); }

    void setDbName(QString name) { db.setDatabaseName(name); }
    void setHostAddress(QString address) { db.setHostName(address); }
    void setHostPort(int port) { db.setPort(port); }

    void selectTables();

    ClientsModel* clientsModel;
    ImageInToursModel* imageInToursModel;
    TouristSitesInTourModel* touristSitesInTourModel;
    TouristSitesModel* touristSitesModel;
    ToursModel* toursModel;
    VauchersModel* vauchersModel;

    QSqlTableModel* zap1;
    QSqlTableModel* zap2;
    QSqlTableModel* zap3;

    QSqlQuery query_1;
    QSqlQuery query_2;
    QSqlQuery query_3;
};

#endif // DATABASEMODULE_H
