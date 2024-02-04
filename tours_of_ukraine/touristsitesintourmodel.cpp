#include "touristsitesintourmodel.h"

TouristSitesInTourModel::TouristSitesInTourModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int TouristSitesInTourModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int TouristSitesInTourModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant TouristSitesInTourModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case ID_TOUR:
        return "ID";
    case ID_TOURIST_SITE:
        return "ID_TOURIST_SITE";
    case NUMBER:
        return "NUMBER";
    }

    return QVariant();
}

QVariant TouristSitesInTourModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case TouristSitesInTourModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags TouristSitesInTourModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void TouristSitesInTourModel::appendRow( const touristSitesInTourObject& newObject ) {
    touristSitesInTourObject record;
    record[ ID_TOUR ] = newObject[ ID_TOUR ];
    record[ ID_TOURIST_SITE ] = newObject[ ID_TOURIST_SITE ];
    record[ NUMBER ] = newObject[ NUMBER ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void TouristSitesInTourModel::updateRow( int row, const touristSitesInTourObject& object )
{
    beginResetModel();

    model[ row ][ ID_TOUR ] = object[ ID_TOUR ];
    model[ row ][ ID_TOURIST_SITE ] = object[ ID_TOURIST_SITE ];
    model[ row ][ NUMBER ] = object[ NUMBER ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void TouristSitesInTourModel::removeRow(QVariant id_tour)
{
    for(int i = 0; i < model.size(); i++)
        if( model[ i ][ ID_TOUR ] == id_tour)
            model[ i ][ STATE_ROW ] = (int)StatesRows::DELETED;
}

bool TouristSitesInTourModel::select()
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

        touristSitesInTourObject record;
        do
        {
            record[ ID_TOUR ] = query.value( ID_TOUR );
            record[ ID_TOURIST_SITE ] = query.value( ID_TOURIST_SITE );
            record[ NUMBER ] = query.value( NUMBER );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool TouristSitesInTourModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (idTour, idTouristSite, number) "
                                      "VALUES (:idTour, :idTouristSite, :number)").arg(table));
                query.bindValue(":idTour", model[ i ][ ID_TOUR ]);
                query.bindValue(":idTouristSite", model[ i ][ ID_TOURIST_SITE ]);
                query.bindValue(":number", model[ i ][ NUMBER ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET number = :number, WHERE idTour = :idTour AND idTouristSite = :idTouristSite").arg(table));
                query.bindValue(":idTour", model[ i ][ ID_TOUR ]);
                query.bindValue(":idTouristSite", model[ i ][ ID_TOURIST_SITE ]);
                query.bindValue(":number", model[ i ][ NUMBER ]);

                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::DELETED)
            {
                qDebug() << "DELETED";
                query.prepare(QString("DELETE FROM %1 WHERE idTour = :idTour AND idTouristSite = :idTouristSite").arg(table));
                query.bindValue(":idTour", model[ i ][ ID_TOUR ]);
                query.bindValue(":idTouristSite", model[ i ][ ID_TOURIST_SITE ]);

                query.exec();
            }
        }
    }

    qDebug() << query.lastError();

    return true;
}

void TouristSitesInTourModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant TouristSitesInTourModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID_TOUR].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

TouristSitesInTourModel::touristSitesInTourObject TouristSitesInTourModel::getDataByRow(int row)
{
    return model[row];
}

QList<TouristSitesInTourModel::touristSitesInTourObject> TouristSitesInTourModel::getDataByTourId(int id)
{
    QList <touristSitesInTourObject> result;

    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID_TOUR].toInt() == id)
            result.append(model[i]);
    }

    return result;
}

bool TouristSitesInTourModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
