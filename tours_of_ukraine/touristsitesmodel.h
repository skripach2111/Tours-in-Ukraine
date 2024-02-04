#ifndef TOURISTSITESMODEL_H
#define TOURISTSITESMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class TouristSitesModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Column {
        ID = 0,
        TITLE,
        IMAGE,
        DESCRIPTION,
        IS_DELETE,
        LAST,
        STATE_ROW
    };

    typedef QHash< TouristSitesModel::Column, QVariant > touristSiteObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const touristSiteObject& newObject );
    void updateRow( int row, const touristSiteObject& object );
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
    touristSiteObject getDataByRow(int row);

private:

    typedef QList< touristSiteObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit TouristSitesModel(QObject *parent = nullptr);
};

typedef QHash< TouristSitesModel::Column, QVariant > touristSiteObject;

#endif // TOURISTSITESMODEL_H
