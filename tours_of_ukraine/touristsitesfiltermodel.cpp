#include "touristsitesfiltermodel.h"

TouristSitesFilterModel::TouristSitesFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void TouristSitesFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void TouristSitesFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool TouristSitesFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, TouristSitesModel::Column::IS_DELETE, source_parent).data(TouristSitesModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::TITLE] && sourceModel()->index(source_row, TouristSitesModel::Column::TITLE, source_parent).data(TouristSitesModel::Role::Read).toString().indexOf(values[FilterParam::TITLE].toString()) == -1 )
        return false;

    return true;
}

bool TouristSitesFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == TouristSitesModel::Column::ID || source_column == TouristSitesModel::Column::IS_DELETE)
        return false;

    return true;
}
