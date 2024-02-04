#include "vauchersmodel.h"

VauchersModel::VauchersModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int VauchersModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int VauchersModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant VauchersModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case ID:
        return "Номер путівки";
    case ID_TOUR:
        return "Назва туру";
    case ID_CLIENT:
        return "ПІБ Клієнта";
    case NUMBER_SEATS:
        return "Кількість місць";
    case SUM:
        return "Сума";
    case PAYMENT_STATUS:
        return "Статус оплати";
    case BOOKING_DATE:
        return "Дата бронювання";
    }

    return QVariant();
}

QVariant VauchersModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        if(index.column() == ID_TOUR)
            return tours->getDataById(model[index.row()][ID_TOUR].toInt(), ToursModel::Column::TITLE);
        if(index.column() == ID_CLIENT)
            return clients->getDataById(model[index.row()][ID_CLIENT].toInt(), ClientsModel::Column::PIB);
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case VauchersModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags VauchersModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );


    return flags;
}

void VauchersModel::appendRow( const vaucherObject& newObject ) {
    vaucherObject record;
    record[ ID ] = newObject[ ID ];
    record[ ID_TOUR ] = newObject[ ID_TOUR ];
    record[ ID_CLIENT ] = newObject[ ID_CLIENT ];
    record[ NUMBER_SEATS ] = newObject[ NUMBER_SEATS ];
    record[ SUM ] = newObject[ SUM ];
    record[ PAYMENT_STATUS ] = newObject[ PAYMENT_STATUS ];
    record[ BOOKING_DATE ] = newObject[ BOOKING_DATE ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void VauchersModel::updateRow( int row, const vaucherObject& object )
{
    beginResetModel();

    model[ row ][ ID ] = object[ ID ];
    model[ row ][ ID_TOUR ] = object[ ID_TOUR ];
    model[ row ][ ID_CLIENT ] = object[ ID_CLIENT ];
    model[ row ][ NUMBER_SEATS ] = object[ NUMBER_SEATS ];
    model[ row ][ SUM ] = object[ SUM ];
    model[ row ][ PAYMENT_STATUS ] = object[ PAYMENT_STATUS ];
    model[ row ][ BOOKING_DATE ] = object[ BOOKING_DATE ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void VauchersModel::removeRow(int row)
{
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool VauchersModel::select()
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

        vaucherObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ ID_TOUR ] = query.value( ID_TOUR );
            record[ ID_CLIENT ] = query.value( ID_CLIENT );
            record[ NUMBER_SEATS ] = query.value( NUMBER_SEATS );
            record[ SUM ] = query.value( SUM );
            record[ PAYMENT_STATUS ] = query.value( PAYMENT_STATUS );
            record[ BOOKING_DATE ] = query.value( BOOKING_DATE );
            record[ IS_DELETE ] = query.value( IS_DELETE );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool VauchersModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (idTour, idClient, number_seats, sum, payment_status, booking_date, isDelete) "
                                      "VALUES (:idTour, :idClient, :number_seats, :sum, :payment_status, :booking_date, 0)").arg(table));
                query.bindValue(":idTour", model[ i ][ ID_TOUR ]);
                query.bindValue(":idClient", model[ i ][ ID_CLIENT ]);
                query.bindValue(":number_seats", model[ i ][ NUMBER_SEATS ]);
                query.bindValue(":sum", model[ i ][ SUM ]);
                query.bindValue(":payment_status", model[ i ][ PAYMENT_STATUS ]);
                query.bindValue(":booking_date", model[ i ][ BOOKING_DATE ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET idTour = :idTour, idClient = :idClient, number_seats = :number_seats, sum = :sum, payment_status = :payment_status, booking_date = :booking_date WHERE id = :id").arg(table));
                query.bindValue(":idTour", model[ i ][ ID_TOUR ]);
                query.bindValue(":idClient", model[ i ][ ID_CLIENT ]);
                query.bindValue(":number_seats", model[ i ][ NUMBER_SEATS ]);
                query.bindValue(":sum", model[ i ][ SUM ]);
                query.bindValue(":payment_status", model[ i ][ PAYMENT_STATUS ]);
                query.bindValue(":booking_date", model[ i ][ BOOKING_DATE ]);
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

void VauchersModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant VauchersModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

VauchersModel::vaucherObject VauchersModel::getDataByRow(int row)
{
    return model[row];
}

bool VauchersModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
