#ifndef PROJECTRECURSIVEFILTERPROXYMODEL_H
#define PROJECTRECURSIVEFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include <constants.h>
#include <stockmodel.h>

class ProjectRecursiveFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ProjectRecursiveFilterProxyModel(QObject *parent = nullptr);
    ~ProjectRecursiveFilterProxyModel();

    ProjectRecursiveFilterProxyModel &setFilterProjectId(const qint32 id) {m_filterProjectId = id; return *this;}
    ProjectRecursiveFilterProxyModel &setFilterLocationId(const qint32 id) {m_filterLocationId = id; return *this;}

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

    qint32 m_filterProjectId;
    qint32 m_filterLocationId;
};

#endif // PROJECTRECURSIVEFILTERPROXYMODEL_H
