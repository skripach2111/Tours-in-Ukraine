#ifndef TOURSMODEL_H
#define TOURSMODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractTableModel>
#include <QObject>
#include <QDebug>

class ToursModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Column {
        ID = 0,
        TITLE,
        DESCRIPTION,
        DATE_EVENT,
        DURATION,
        TYPE,
        NUMBER_SEATS,
        COST,
        IS_DELETE,
        LAST,
        STATE_ROW
    };

    typedef QHash< ToursModel::Column, QVariant > tourObject;

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void appendRow( const tourObject& newObject );
    void updateRow( int row, const tourObject& object );
    void removeRow( int row );

    bool select();
    QVariant saveChanges();
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
    tourObject getDataByRow(int row);
    int getRowByData(QVariant data, Column column);

private:

    typedef QList< tourObject > DataList;
    DataList model;

    QSqlDatabase *db;
    QSqlQuery query;

    QString table;
public:
    explicit ToursModel(QObject *parent = nullptr);
};

typedef QHash< ToursModel::Column, QVariant > tourObject;

#endif // TOURSMODEL_H
