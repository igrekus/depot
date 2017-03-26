#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>

class GroupItem : public AbstractItem
{
public:

    explicit GroupItem():
        AbstractItem()
    {}

    explicit GroupItem(qint32 id, const QString &name):
        AbstractItem(id, name)
    {}

    explicit GroupItem(const GroupItem &copy):
        AbstractItem(copy.itemId, copy.itemName)
    {}

    GroupItem &operator=(const GroupItem &right) {
        if (this != &right) {
            itemId   = right.itemId;
            itemName = right.itemName;
        }
        return *this;
    }

    bool operator==(const GroupItem &right) const {
        return (itemId   == right.itemId &&
                itemName == right.itemName);
    }

    friend QDebug operator<<(QDebug dbg, const GroupItem &right) {
        dbg.nospace() << "Group("
                      << "id:"    << right.itemId
                      << " name:" << right.itemName
                      << ")";
        return dbg.maybeSpace();
    }

    using GroupList = QVector<GroupItem>;
};

#endif // GROUPITEM_H
