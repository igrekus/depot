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

    GroupItem(const GroupItem &copy):
        AbstractItem(copy)
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

    using GroupList = QList<GroupItem>;

    class GroupItemBuilder {
    public:
        qint32  groupId = 0;
        QString groupName = QString();

        GroupItemBuilder& setId  (const qint32   id)   { this->groupId   = id;   return *this; }
        GroupItemBuilder& setName(const QString &name) { this->groupName = name; return *this; }
//        Builder& setProductP(){          // pre-defined group, if needed
//			this->i = 42;
//			this->f = -1.0f/12.0f;
//			this->c = '@';
//			return *this;
//		}
        GroupItem build() {
            return GroupItem(this->groupId, this->groupName);
        }
    };
};

#endif // GROUPITEM_H