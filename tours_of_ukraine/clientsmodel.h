#ifndef CLIENTSMODEL_H
#define CLIENTSMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class ClientsModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Column {
        ID = 0,
        PIB,
        PHONE_NUMBER,
        EMAIL,
        IS_DELETE,
        LAST,
        STATE_ROW
    };

    typedef QHash< ClientsModel::Column, QVariant > clientObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const clientObject& newClient );
    void updateRow( int row, const clientObject& client );
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
    clientObject getDataByRow(int row);
    int getRowByData(QVariant data, Column column);

private:

    typedef QList< clientObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit ClientsModel(QObject *parent = nullptr);
};

typedef QHash< ClientsModel::Column, QVariant > clientObject;

#endif // CLIENTSMODEL_H
