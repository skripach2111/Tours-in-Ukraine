#ifndef VAUCHERSFILTERMODEL_H
#define VAUCHERSFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

#include "vauchersmodel.h"

class VauchersFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit VauchersFilterModel(QObject *parent = nullptr);

    enum FilterParam {
        ID = 0
    };

    void setFilterParam(FilterParam par, QVariant val);
    void setEnabledFilterParam(FilterParam par, bool flag);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;


private:
    QHash <FilterParam, bool> params;
    QHash <FilterParam, QVariant> values;
};

#endif // VAUCHERSFILTERMODEL_H
