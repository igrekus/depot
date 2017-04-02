#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QObject>
#include <QDebug>

class AbstractItem
{
public:
    qint32  itemId;
    QString itemName;

    explicit AbstractItem():
        itemId(),
        itemName()
    {}

    explicit AbstractItem(const qint32 id, const QString &name):
        itemId(id),
        itemName(name)
    {}

    explicit AbstractItem(const AbstractItem &copy):
        itemId(copy.itemId),
        itemName(copy.itemName)
    {}

    virtual ~AbstractItem() {}
};

#endif // ABSTRACTITEM_H
