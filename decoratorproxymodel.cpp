#include "decoratorproxymodel.h"

DecoratorProxyModel::DecoratorProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

DecoratorProxyModel::~DecoratorProxyModel()
{

}

int DecoratorProxyModel::rowCount(const QModelIndex &parent) const
{
    return sourceModel()->rowCount(parent) + 1;
//    return QSortFilterProxyModel::rowCount(parent) + 1;
}

QVariant DecoratorProxyModel::data(const QModelIndex &index, int role) const
{
    qint32 row = index.row();
    qint32 col = index.column();

    qDebug() << "row: " << row << "col:" << col;

    if (role != Qt::DisplayRole) {
        if (row < QSortFilterProxyModel::rowCount(QModelIndex())) {
            return QSortFilterProxyModel::data(index, role);
        }
        return QVariant();
    }

//    if (row == QSortFilterProxyModel::rowCount(QModelIndex())) {
        return QVariant(QString("Total:"));
//    }
//    return QSortFilterProxyModel::data(index, role);
}

Qt::ItemFlags DecoratorProxyModel::flags(const QModelIndex &index) const
{
//    if (index.row() == QSortFilterProxyModel::rowCount(QModelIndex())) {
//        qDebug() << "flag call for virtual row";
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
//    }
//    return QSortFilterProxyModel::flags(index);
}
