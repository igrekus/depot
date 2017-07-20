#ifndef DECORATORPROXYMODEL_H
#define DECORATORPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include <constants.h>

class DecoratorProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DecoratorProxyModel(QObject *parent = nullptr);
    ~DecoratorProxyModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // DECORATORPROXYMODEL_H
