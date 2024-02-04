#ifndef VAUCHERSMODEL_H
#define VAUCHERSMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

#include "clientsmodel.h"
#include "toursmodel.h"

class VauchersModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Column {
        ID = 0,
        ID_TOUR,
        ID_CLIENT,
        NUMBER_SEATS,
        SUM,
        PAYMENT_STATUS,
        BOOKING_DATE,
        IS_DELETE,
        LAST,
        STATE_ROW
    };

    typedef QHash< VauchersModel::Column, QVariant > vaucherObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const vaucherObject& newObject );
    void updateRow( int row, const vaucherObject& object );
    void removeRow( int row );

    bool select();
    bool saveChanges();
    void setTable(QString t, QSqlDatabase *database);

    enum Role {
        Display = 0x1,
        Read
    };

    enum StatesRows {
        NOT_EDITED = 0x1,
        ADDED,
        EDITED,
        DELETED
    };

    QVariant getDataById(int id, Column column);
    vaucherObject getDataByRow(int row);

    ClientsModel* clients;
    ToursModel* tours;

private:

    typedef QList< vaucherObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit VauchersModel(QObject *parent = nullptr);
};

typedef QHash< VauchersModel::Column, QVariant > vaucherObject;

#endif // VAUCHERSMODEL_H
