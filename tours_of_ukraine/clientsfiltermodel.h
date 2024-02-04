#ifndef CLIENTSFILTERMODEL_H
#define CLIENTSFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

#include "clientsmodel.h"

class ClientsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ClientsFilterModel(QObject *parent = nullptr);

    enum FilterParam {
        PIB = 0,
        PHONE_NUMBER,
        EMAIL
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

#endif // CLIENTSFILTERMODEL_H
