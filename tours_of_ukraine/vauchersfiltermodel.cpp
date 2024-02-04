#include "vauchersfiltermodel.h"

VauchersFilterModel::VauchersFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

void VauchersFilterModel::setFilterParam(FilterParam par, QVariant val)
{
    values[par] = val;
    invalidateFilter();
}

void VauchersFilterModel::setEnabledFilterParam(FilterParam par, bool flag)
{
    params[par] = flag;
    invalidateFilter();
}

bool VauchersFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(sourceModel()->index(source_row, VauchersModel::Column::IS_DELETE, source_parent).data(VauchersModel::Role::Read).toBool())
        return false;

    if(params[FilterParam::ID] && sourceModel()->index(source_row, VauchersModel::Column::ID, source_parent).data(VauchersModel::Role::Read).toString().indexOf(values[FilterParam::ID].toString()) == -1 )
        return false;

    return true;
}

bool VauchersFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    if(source_column == VauchersModel::Column::IS_DELETE)
        return false;

    return true;
}
