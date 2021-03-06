#include "decoratorproxymodel.h"

DecoratorProxyModel::DecoratorProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

DecoratorProxyModel::~DecoratorProxyModel()
{

}

int DecoratorProxyModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "proxy rowCount() call, sourceModel():"  << sourceModel()->rowCount(parent) << "base class rowCount():" << QSortFilterProxyModel::rowCount(parent);
    qDebug() << "modified rowCount() response:" << sourceModel()->rowCount(parent) + 1;
    return sourceModel()->rowCount(parent) + 1;
//    return QSortFilterProxyModel::rowCount(parent) + 1;
    return QSortFilterProxyModel::rowCount(parent);
}

QVariant DecoratorProxyModel::data(const QModelIndex &index, int role) const
{
//    qint32 row = index.row();
//    qint32 col = index.column();

//    qDebug() << "row: " << row << "col:" << col;

//    if (role != Qt::DisplayRole) {
//        if (row < QSortFilterProxyModel::rowCount(QModelIndex())) {
//            return QSortFilterProxyModel::data(index, role);
//        }
//        return QVariant();
//    }

////    if (row == QSortFilterProxyModel::rowCount(QModelIndex())) {
//        return QVariant(QString("Total:"));
////    }
    return QSortFilterProxyModel::data(index, role);
}

Qt::ItemFlags DecoratorProxyModel::flags(const QModelIndex &index) const
{
    qint32 row = index.row();
    qint32 col = index.column();
    qDebug() << "flags():" << row << col;
//    if (index.row() == QSortFilterProxyModel::rowCount(QModelIndex())) {
//        qDebug() << "flag call for virtual row";
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
//    }
    return QSortFilterProxyModel::flags(index);
}
