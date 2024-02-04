#include "clientsfiltermodel.h"

ClientsFilterModel::ClientsFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void ClientsFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void ClientsFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool ClientsFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, ClientsModel::Column::IS_DELETE, source_parent).data(ClientsModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::PIB] && sourceModel()->index(source_row, ClientsModel::Column::PIB, source_parent).data(ClientsModel::Role::Read).toString().indexOf(values[FilterParam::PIB].toString()) == -1 )
        return false;

    if(params[FilterParam::PHONE_NUMBER] && sourceModel()->index(source_row, ClientsModel::Column::PHONE_NUMBER, source_parent).data(ClientsModel::Role::Read).toString().indexOf(values[FilterParam::PHONE_NUMBER].toString()) == -1 )
        return false;

    if(params[FilterParam::EMAIL] && sourceModel()->index(source_row, ClientsModel::Column::EMAIL, source_parent).data(ClientsModel::Role::Read).toString().indexOf(values[FilterParam::EMAIL].toString()) == -1 )
        return false;

    return true;
}

bool ClientsFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == ClientsModel::Column::ID || source_column == ClientsModel::Column::IS_DELETE)
        return false;

    return true;
}
