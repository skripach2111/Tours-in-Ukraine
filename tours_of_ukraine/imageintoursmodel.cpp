#include "imageintoursmodel.h"

ImageInToursModel::ImageInToursModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int ImageInToursModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int ImageInToursModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant ImageInToursModel::headerData( int section, Qt::Orientation orientation, int role ) const
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
    case ID_TOUR:
        return "ID_TOUR";
    case IMAGE:
        return "Зображення";
    }

    return QVariant();
}

QVariant ImageInToursModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case ImageInToursModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags ImageInToursModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void ImageInToursModel::appendRow( const imageInTourObject& newObject ) {
    imageInTourObject record;
    record[ ID ] = newObject[ ID ];
    record[ ID_TOUR ] = newObject[ ID_TOUR ];
    record[ IMAGE ] = newObject[ IMAGE ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void ImageInToursModel::updateRow( int& row, const imageInTourObject& object )
{
    beginResetModel();

    model[ row ][ ID ] = object[ ID ];
    model[ row ][ ID_TOUR ] = object[ ID_TOUR ];
    model[ row ][ IMAGE ] = object[ IMAGE ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void ImageInToursModel::removeRow(QVariant id)
{
    for(int i = 0; i < model.size(); i++)
        if( model[ i ][ ID_TOUR ] == id)
            model[ i ][ STATE_ROW ] = (int)StatesRows::DELETED;
}

bool ImageInToursModel::select()
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

        imageInTourObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ ID_TOUR ] = query.value( ID_TOUR );
            record[ IMAGE ] = query.value( IMAGE );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool ImageInToursModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (idTour, image) "
                                      "VALUES (:idTour, :image)").arg(table));
                query.bindValue(":idTour", model[ i ][ ID_TOUR ]);
                query.bindValue(":image", model[ i ][ IMAGE ].toByteArray(), QSql::In | QSql::Binary);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET idTour = :idTour, image = :image, WHERE id = :id").arg(table));
                query.bindValue(":idTour", model[ i ][ ID_TOUR ]);
                query.bindValue(":image", model[ i ][ IMAGE ].toByteArray(), QSql::In | QSql::Binary);
                query.bindValue(":id", model[ i ][ ID ]);

                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::DELETED)
            {
                qDebug() << "DELETED";
                query.prepare(QString("DELETE FROM %1 WHERE id = :id AND idTour = :idTour").arg(table));
                query.bindValue(":id", model[ i ][ ID ]);
                query.bindValue(":idTour", model[ i ][ ID_TOUR ]);

                query.exec();
            }
        }
    }

    qDebug() << query.lastError();

    return true;
}

void ImageInToursModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant ImageInToursModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

QList<ImageInToursModel::imageInTourObject> ImageInToursModel::getDataByIdTour(int id)
{
    QList <imageInTourObject> result;

    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID_TOUR].toInt() == id)
            result.append(model[i]);
    }

    return result;
}

bool ImageInToursModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
