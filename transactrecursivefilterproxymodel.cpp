#include "transactrecursivefilterproxymodel.h"

TransactRecursiveFilterProxyModel::TransactRecursiveFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

TransactRecursiveFilterProxyModel::~TransactRecursiveFilterProxyModel()
{

}

bool TransactRecursiveFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent);
    if(sourceIndex.isValid()) {
        // test self
        qint32 projId = sourceModel()->index(sourceRow, TransactModel::ColumnProject, sourceParent).data(Constants::RoleProjectId).toInt();
        if (m_filterProjectId == 0 || m_filterProjectId == projId) {
            for (qint32 i=0; i<sourceModel()->columnCount(); ++i) {
                if (sourceModel()->index(sourceRow, i, sourceParent).data(Qt::DisplayRole).toString().contains(filterRegExp())) {
                    return true;
                }
            }
        }
    }
    return false;
}
