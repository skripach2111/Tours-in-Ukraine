#ifndef TOURISTSITESINTOURMODEL_H
#define TOURISTSITESINTOURMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class TouristSitesInTourModel : public QAbstractTableModel
{
    Q_OBJECT

    QVariant id_for_del;

public:

    enum Column {
        ID_TOUR = 0,
        ID_TOURIST_SITE,
        NUMBER,
        LAST,
        STATE_ROW
    };

    typedef QHash< TouristSitesInTourModel::Column, QVariant > touristSitesInTourObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const touristSitesInTourObject& newObject );
    void updateRow( int row, const touristSitesInTourObject& object );
    void removeRow( QVariant id_tour );

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
    touristSitesInTourObject getDataByRow(int row);
    QList <touristSitesInTourObject> getDataByTourId(int id);

private:

    typedef QList< touristSitesInTourObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit TouristSitesInTourModel(QObject *parent = nullptr);
};

typedef QHash< TouristSitesInTourModel::Column, QVariant > touristSitesInTourObject;

#endif // TOURISTSITESINTOURMODEL_H
