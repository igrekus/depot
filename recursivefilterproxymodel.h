#ifndef RECURSIVEFILTERPROXYMODEL_H
#define RECURSIVEFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QBrush>
#include <constants.h>

class RecursiveFilterProxyModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    explicit RecursiveFilterProxyModel(QObject *parent = nullptr);
    ~RecursiveFilterProxyModel();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // RECURSIVEFILTERPROXYMODEL_H
