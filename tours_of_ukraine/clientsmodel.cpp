#include "clientsmodel.h"

ClientsModel::ClientsModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int ClientsModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return model.count();
}

int ClientsModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return LAST;
}

QVariant ClientsModel::headerData( int section, Qt::Orientation orientation, int role ) const
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
    case PIB:
        return "ПІБ";
    case PHONE_NUMBER:
        return "Номер телефону";
    case EMAIL:
        return "Пошта";
    }

    return QVariant();
}

QVariant ClientsModel::data( const QModelIndex& index, int role ) const {
    if(!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    case ClientsModel::Role::Read:
    {
        return model[ index.row() ][ Column( index.column() ) ];
    }
    default:
    {
        return QVariant();
    }
    }
}

Qt::ItemFlags ClientsModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );

    return flags;
}

void ClientsModel::appendRow( const clientObject& newClient ) {
    clientObject record;
    record[ ID ] = newClient[ ID ];
    record[ PIB ] = newClient[ PIB ];
    record[ PHONE_NUMBER ] = newClient[ PHONE_NUMBER ];
    record[ EMAIL ] = newClient[ EMAIL ];
    record[ STATE_ROW ] = (int)StatesRows::ADDED;

    int row = model.count();
    beginInsertRows( QModelIndex(), row, row );
    model.append( record );
    endInsertRows();
}

void ClientsModel::updateRow( int row, const clientObject& client )
{
    beginResetModel();

    model[ row ][ ID ] = client[ ID ];
    model[ row ][ PIB ] = client[ PIB ];
    model[ row ][ PHONE_NUMBER ] = client[ PHONE_NUMBER ];
    model[ row ][ EMAIL ] = client[ EMAIL ];
    model[ row ][ STATE_ROW ] = (int)StatesRows::EDITED;

    endResetModel();
}

void ClientsModel::removeRow(int row)
{
    model[ row ][ STATE_ROW ] = StatesRows::DELETED;
}

bool ClientsModel::select()
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

        clientObject record;
        do
        {
            record[ ID ] = query.value( ID );
            record[ PIB ] = query.value( PIB );
            record[ PHONE_NUMBER ] = query.value( PHONE_NUMBER );
            record[ EMAIL ] = query.value( EMAIL );
            record[ IS_DELETE ] = query.value( IS_DELETE );
            record[ STATE_ROW ] = StatesRows::NOT_EDITED;

            model.append( record );

        }while(query.next());

        endInsertRows();
    }

    endResetModel();

    return false;
}


bool ClientsModel::saveChanges()
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[ i ][ STATE_ROW ] != StatesRows::NOT_EDITED)
        {
            qDebug() << "!= NOT_EDIT";
            if(model[ i ][ STATE_ROW ] == StatesRows::ADDED)
            {
                qDebug() << "ADDED";
                query.prepare(QString("INSERT INTO %1 (pib, phone_number, email, isDelete) VALUES (:pib, :phone_number, :email, 0)").arg(table));
                query.bindValue(":pib", model[ i ][ PIB ]);
                query.bindValue(":phone_number", model[ i ][ PHONE_NUMBER ]);
                query.bindValue(":email", model[ i ][ EMAIL ]);
                query.exec();
            }
            else if(model[ i ][ STATE_ROW ] == StatesRows::EDITED)
            {
                qDebug() << "EDITED";
                query.prepare(QString("UPDATE %1 SET pib = :pib, phone_number = :phone_number, email = :email WHERE id = :id").arg(table));
                query.bindValue(":pib", model[ i ][ PIB ]);
                query.bindValue(":phone_number", model[ i ][ PHONE_NUMBER ]);
                query.bindValue(":email", model[ i ][ EMAIL ]);
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

void ClientsModel::setTable(QString t, QSqlDatabase *database)
{
    table = t;
    db = database;
}

QVariant ClientsModel::getDataById(int id, Column column)
{
    for(int i = 0; i < model.size(); i++)
    {
        if(model[i][ID].toInt() == id)
            return model[i][column];
    }

    return QVariant();
}

ClientsModel::clientObject ClientsModel::getDataByRow(int row)
{
    return model[row];
}

int ClientsModel::getRowByData(QVariant data, Column column)
{
    for(int i = 0; i < model.size(); i++)
        if(model[i][column] == data)
            return i;
}

bool ClientsModel::setData( const QModelIndex& index, const QVariant& value, int role ) {
    if( !index.isValid() || role != Qt::EditRole || model.count() <= index.row() ) {
        return false;
    }

    model[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}
