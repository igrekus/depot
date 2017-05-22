#include "recursivefilterproxymodel.h"

RecursiveFilterProxyModel::RecursiveFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

RecursiveFilterProxyModel::~RecursiveFilterProxyModel()
{

}

bool RecursiveFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (filterRegExp().isEmpty())
        return true;
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent);
    if(sourceIndex.isValid()) {
        // recursive call to test children
        for(qint32 i=0; i<sourceModel()->rowCount(sourceIndex); ++i) {
            if(filterAcceptsRow(i, sourceIndex)) {
                return true;
            }
        }
        // test self
        for (qint32 i=0; i<sourceModel()->columnCount(); ++i) {
            if (sourceModel()->index(sourceRow, i, sourceParent).data(Qt::DisplayRole).toString().contains(filterRegExp())) {
                return true;
            }
        }
    }
//    return false;
//     default
    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

//QVariant RecursiveFilterProxyModel::data(const QModelIndex &index, int role) const
//{
//    if (!filterRegExp().isEmpty()) {
//        if (role == Qt::BackgroundRole) {
//            QModelIndex sourceIndex = mapToSource(index);
//            if (filterAcceptsRow(sourceIndex.row(), sourceIndex.parent())) {
//                if (sourceIndex.data(Qt::DisplayRole).toString().contains(filterRegExp())) {
//                    return QVariant(QBrush(QColor(QRgb(Constants::ColorSearchBackground))));
//                }
//            }
//        }
//    }
//    return QSortFilterProxyModel::data(index, role);
//}
