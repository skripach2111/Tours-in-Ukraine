#ifndef IMAGEINTOURSMODEL_H
#define IMAGEINTOURSMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class ImageInToursModel : public QAbstractTableModel
{
    Q_OBJECT

    QVariant id_for_del;

public:

    enum Column {
        ID = 0,
        ID_TOUR,
        IMAGE,
        LAST,
        STATE_ROW
    };

    typedef QHash< ImageInToursModel::Column, QVariant > imageInTourObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const imageInTourObject& newObject );
    void updateRow( int& row, const imageInTourObject& object );
    void removeRow( QVariant id );

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
    QList<imageInTourObject> getDataByIdTour(int id);

private:

    typedef QList< imageInTourObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit ImageInToursModel(QObject *parent = nullptr);
};

typedef QHash< ImageInToursModel::Column, QVariant > imageInTourObject;

#endif // IMAGEINTOURSMODEL_H
