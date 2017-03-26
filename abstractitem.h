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

    explicit AbstractItem(qint32 id, const QString &name):
        itemId(id),
        itemName(name)
    {}

    explicit AbstractItem(const AbstractItem &copy):
        itemId(copy.itemId),
        itemName(copy.itemName)
    {}

//    AbstractItem &operator=(const AbstractItem &right) {
//        if (this != &right) {
//            itemId   = right.itemId;
//            itemName = right.itemName;
//        }
//        return *this;
//    }

//    bool operator==(const AbstractItem &right) const {
//        return (itemId   == right.itemId &&
//                itemName == right.itemName);
//    }

//    friend QDebug operator<<(QDebug dbg, const AbstractItem &right) {
//        dbg.nospace() << "Category("
//                      << "id:"    << right.itemId
//                      << " name:"  << right.itemName
//                      << ")";
//        return dbg.maybeSpace();
//    }
};

#endif // ABSTRACTITEM_H
