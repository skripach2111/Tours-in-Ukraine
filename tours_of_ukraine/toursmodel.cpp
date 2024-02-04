#include "toursmodel.h"

ToursModel::ToursModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int ToursModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int ToursModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant ToursModel::headerData( int section, Qt::Orientation orientation, int role ) const
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
    case DESCRIPTION:
        return "Опис";
    case DATE_EVENT:
        return "Дата проведення";
    case DURATION:
        return "Тривалість";
    case TYPE:
        return "Тип подорожі";
    case NUMBER_SEATS:
        return "Кількість місць";
    case COST:
        return "Вартість";
    }

    return QVariant();
}

QVariant ToursModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case ToursModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags ToursModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void ToursModel::appendRow( const tourObject& newObject ) {
    tourObject record;
    record[ ID ] = newObject[ ID ];
    record[ TITLE ] = newObject[ TITLE ];
    record[ DESCRIPTION ] = newObject[ DESCRIPTION ];
    record[ DATE_EVENT ] = newObject[ DATE_EVENT ];
    record[ DURATION ] = newObject[ DURATION ];
    record[ TYPE ] = newObject[ TYPE ];
    record[ NUMBER_SEATS ] = newObject[ NUMBER_SEATS ];
    record[ COST ] = newObject[ COST ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void ToursModel::updateRow( int row, const tourObject& object )
{
    beginResetModel();

    model[ row ][ ID ] = object[ ID ];
    model[ row ][ TITLE ] = object[ TITLE ];
    model[ row ][ DESCRIPTION ] = object[ DESCRIPTION ];
    model[ row ][ DATE_EVENT ] = object[ DATE_EVENT ];
    model[ row ][ DURATION ] = object[ DURATION ];
    model[ row ][ TYPE ] = object[ TYPE ];
    model[ row ][ NUMBER_SEATS ] = object[ NUMBER_SEATS ];
    model[ row ][ COST ] = object[ COST ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void ToursModel::removeRow(int row)
{
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool ToursModel::select()
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

        tourObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ TITLE ] = query.value( TITLE );
            record[ DESCRIPTION ] = query.value( DESCRIPTION );
            record[ DATE_EVENT ] = query.value( DATE_EVENT );
            record[ DURATION ] = query.value( DURATION );
            record[ TYPE ] = query.value( TYPE );
            record[ NUMBER_SEATS ] = query.value( NUMBER_SEATS );
            record[ COST ] = query.value( COST );
            record[ IS_DELETE ] = query.value( IS_DELETE );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


QVariant ToursModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (title, description, date_event, duration, type, number_seats, cost, isDelete) "
                                      "VALUES (:title, :description, :date_event, :duration, :type, :number_seats, :cost, 0)").arg(table));
                query.bindValue(":title", model[ i ][ TITLE ]);
                query.bindValue(":description", model[ i ][ DESCRIPTION ]);
                query.bindValue(":date_event", model[ i ][ DATE_EVENT ]);
                query.bindValue(":duration", model[ i ][ DURATION ]);
                query.bindValue(":type", model[ i ][ TYPE ]);
                query.bindValue(":number_seats", model[ i ][ NUMBER_SEATS ]);
                query.bindValue(":cost", model[ i ][ COST ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET title = :title, description = :description, date_event = :date_event, duration = :duration, type = :type, number_seats = :number_seats, cost = :cost WHERE id = :id").arg(table));
                query.bindValue(":title", model[ i ][ TITLE ]);
                query.bindValue(":description", model[ i ][ DESCRIPTION ]);
                query.bindValue(":date_event", model[ i ][ DATE_EVENT ]);
                query.bindValue(":duration", model[ i ][ DURATION ]);
                query.bindValue(":type", model[ i ][ TYPE ]);
                query.bindValue(":number_seats", model[ i ][ NUMBER_SEATS ]);
                query.bindValue(":cost", model[ i ][ COST ]);
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
    query.prepare("SELECT LAST_INSERT_ID();");
    query.exec();
    query.next();

    return query.value(0);
}

void ToursModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant ToursModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

tourObject ToursModel::getDataByRow(int row)
{
    return model[row];
}

int ToursModel::getRowByData(QVariant data, Column column)
{
    for(int i = 0; i < model.size(); i++)
        if(model[i][column] == data)
            return i;
}

bool ToursModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
