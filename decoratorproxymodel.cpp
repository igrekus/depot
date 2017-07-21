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
    qint32 row = index.row();
    qint32 col = index.column();
    qDebug() << "flags():" << row << col;
//    if (index.row() == QSortFilterProxyModel::rowCount(QModelIndex())) {
//        qDebug() << "flag call for virtual row";
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
//    }
//    return QSortFilterProxyModel::flags(index);
}


[QUOTE=d_stranz;299849]...does your override of rowCount() ever get called?[/QUOTE]

Yes, it gets called properly. I modified my override to confirm:

[CODE]
int DecoratorProxyModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "proxy rowCount() call, sourceModel():"  << sourceModel()->rowCount(parent) << "base class rowCount():" << QSortFilterProxyModel::rowCount(parent);
    qDebug() << "modified rowCount() response:" << sourceModel()->rowCount(parent) + 1;
    return sourceModel()->rowCount(parent) + 1;
//    return QSortFilterProxyModel::rowCount(parent) + 1;
}

Both implementations, using the source model [b]rowCount()[/b] and using the base class [b]rowCount()[/b] report the same number of rows.

[/CODE]

Debug output:
[CODE]
> Source model initModel(), rows: 7
proxy rowCount() call, sourceModel()->rowC: 7 base class rowCount(): 7
modified rowCount() response: 8
proxy rowCount() call, sourceModel(): 7 base class rowCount(): 7
modified rowCount() response: 8
proxy rowCount() call, sourceModel(): 7 base class rowCount(): 7
modified rowCount() response: 8
proxy rowCount() call, sourceModel(): 7 base class rowCount(): 7
modified rowCount() response: 8
proxy rowCount() call, sourceModel(): 7 base class rowCount(): 7
modified rowCount() response: 8
proxy rowCount() call, sourceModel(): 7 base class rowCount(): 7
modified rowCount() response: 8
proxy rowCount() call, sourceModel(): 7 base class rowCount(): 7
modified rowCount() response: 8
[/CODE]

[QUOTE=d_stranz;299849]Have you also overridden the [QTCLASS]QSortFilterProxyModel::filterAcceptsRow()[/QTCLASS] protected method?[/QUOTE]

No, I didn't touch the [QTCLASS]QSortFilterProxyModel::filterAcceptsRow()[/QTCLASS] method. In fact, I've read Qt docs and found out that it is better to use [QTCLASS]QIdentityProxyModel[/QTCLASS] if I just want to modify the output. So, I tried subclassing identity proxy overriding only [b]rowCount()[/b] and [b]data()[/b] methods with the same code I did for sortfilter proxy, but it didn't help, the view doesn't call the [b]data()[/b] method for that additional virtual [b]rowCount()+1[/b] row.

I also tried fiddling with the [QTCLASS]QSortFilterProxyModel::flags()[/QTCLASS], but seems like it doesn't get called for the virtual row either.
[CODE]
Qt::ItemFlags DecoratorProxyModel::flags(const QModelIndex &index) const
{
    qint32 row = index.row();
    qint32 col = index.column();
    qDebug() << "flags():" << row << col;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
[/CODE]
