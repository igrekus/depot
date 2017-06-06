#ifndef TRANSACTRECURSIVEFILTERPROXYMODEL_H
#define TRANSACTRECURSIVEFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include <constants.h>
#include <stockmodel.h>
#include <transactmodel.h>

class TransactRecursiveFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TransactRecursiveFilterProxyModel(QObject *parent = nullptr);
    ~TransactRecursiveFilterProxyModel();

    TransactRecursiveFilterProxyModel &setFilterProjectId(const qint32 id) {m_filterProjectId = id; return *this;}

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    qint32 m_filterProjectId;
};

#endif // TRANSACTRECURSIVEFILTERPROXYMODEL_H
