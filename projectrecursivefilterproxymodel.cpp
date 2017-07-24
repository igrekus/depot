#include "projectrecursivefilterproxymodel.h"

ProjectRecursiveFilterProxyModel::ProjectRecursiveFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

ProjectRecursiveFilterProxyModel::~ProjectRecursiveFilterProxyModel()
{

}

bool ProjectRecursiveFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    // TODO: factor out check into functions
//    if (filterRegExp().isEmpty() && (m_filterProjectId == 0))
//        return true;

    QModelIndex sourceIndex = sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent);
    if(sourceIndex.isValid()) {
        // recursive call to test children
        qint32 rows = sourceModel()->rowCount(sourceIndex);
        if (rows > 0) {
            for(qint32 i=0; i<rows; ++i) {
                if(filterAcceptsRow(i, sourceIndex)) {
                    return true;
                }
            }
        }

        // test self
        qint32 projId = sourceModel()->index(sourceRow, StockModel::ProjectColumn, sourceParent).data(Constants::RoleProjectId).toInt();
        if (m_filterProjectId == 0 || m_filterProjectId == projId) {
            for (qint32 i=1; i<sourceModel()->columnCount(); ++i) {
                if (sourceModel()->index(sourceRow, i, sourceParent).data(Qt::DisplayRole).toString().contains(filterRegExp())) {
                    return true;
                }
            }
        }
    }
    return false;
}
