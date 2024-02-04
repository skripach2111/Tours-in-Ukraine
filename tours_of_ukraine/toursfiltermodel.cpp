#include "toursfiltermodel.h"

ToursFilterModel::ToursFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void ToursFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void ToursFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool ToursFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, ToursModel::Column::IS_DELETE, source_parent).data(ToursModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::TITLE] && sourceModel()->index(source_row, ToursModel::Column::TITLE, source_parent).data(ToursModel::Role::Read).toString().indexOf(values[FilterParam::TITLE].toString()) == -1 )
        return false;

    return true;
}

bool ToursFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == ToursModel::Column::ID || source_column == ToursModel::Column::IS_DELETE)
        return false;

    return true;
}
