#include "touristsitesmodel.h"

TouristSitesModel::TouristSitesModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int TouristSitesModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int TouristSitesModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant TouristSitesModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case ID:
        return "ID";
    case TITLE:
        return "Назва";
    case IMAGE:
        return "Зображення";
    case DESCRIPTION:
        return "Опис";
    }

    return QVariant();
}

QVariant TouristSitesModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case TouristSitesModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags TouristSitesModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void TouristSitesModel::appendRow( const touristSiteObject& newObject ) {
    touristSiteObject record;
    record[ ID ] = newObject[ ID ];
    record[ TITLE ] = newObject[ TITLE ];
    record[ IMAGE ] = newObject[ IMAGE ];
    record[ DESCRIPTION ] = newObject[ DESCRIPTION ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void TouristSitesModel::updateRow( int row, const touristSiteObject& object )
{
    beginResetModel();

    model[ row ][ ID ] = object[ ID ];
    model[ row ][ TITLE ] = object[ TITLE ];
    model[ row ][ IMAGE ] = object[ IMAGE ];
    model[ row ][ DESCRIPTION ] = object[ DESCRIPTION ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void TouristSitesModel::removeRow(int row)
{
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool TouristSitesModel::select()
{
    beginResetModel();
    beginRemoveRows(createIndex(0, 0), 0, model.count());
    while(model.count() != 0)
        model.removeFirst();
    endRemoveRows();

    query.prepare(QString("SELECT * FROM %1").arg(table));
    query.exec();
    if(query.next())
    {
        int row = model.count();
        beginInsertRows( createIndex(0, 0), row, row+query.size()-1 );

        touristSiteObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ TITLE ] = query.value( TITLE );
            record[ IMAGE ] = query.value( IMAGE );
            record[ DESCRIPTION ] = query.value( DESCRIPTION );
            record[ IS_DELETE ] = query.value( IS_DELETE );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool TouristSitesModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (title, image, description, isDelete) "
                                      "VALUES (:title, :image, :description, 0)").arg(table));
                query.bindValue(":title", model[ i ][ TITLE ]);
                query.bindValue(":image", model[ i ][ IMAGE ].toByteArray(), QSql::In | QSql::Binary);
                query.bindValue(":description", model[ i ][ DESCRIPTION ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET title = :title, image = :image, description = :description WHERE id = :id").arg(table));
                query.bindValue(":title", model[ i ][ TITLE ]);
                query.bindValue(":image", model[ i ][ IMAGE ].toByteArray(), QSql::In | QSql::Binary);
                query.bindValue(":description", model[ i ][ DESCRIPTION ]);
                query.bindValue(":id", model[ i ][ ID ]);

                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::DELETED)
            {
                qDebug() << "DELETED";
                query.prepare(QString("UPDATE %1 SET isDelete = 1 WHERE id = :id").arg(table));
                query.bindValue(":id", model[ i ][ ID ]);

                query.exec();
            }
        }
    }

    qDebug() << query.lastError();

    return true;
}

void TouristSitesModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant TouristSitesModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

TouristSitesModel::touristSiteObject TouristSitesModel::getDataByRow(int row)
{
    return model[row];
}

bool TouristSitesModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
