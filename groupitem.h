#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>

class GroupItem : public AbstractItem
{
public:

    qint32 itemCategoryRef;

    explicit GroupItem():
        AbstractItem()
    {}

    explicit GroupItem(const qint32 id, const QString &name, const qint32 catRef):
        AbstractItem(id, name),
        itemCategoryRef(catRef)
    {}

    GroupItem(const GroupItem &copy):
        AbstractItem(copy),
        itemCategoryRef(copy.itemCategoryRef)
    {}

    GroupItem &operator=(const GroupItem &right) {
        if (this != &right) {
            itemId          = right.itemId;
            itemName        = right.itemName;
            itemCategoryRef = right.itemCategoryRef;
        }
        return *this;
    }

    bool operator==(const GroupItem &right) const {
        return (itemId          == right.itemId   &&
                itemName        == right.itemName &&
                itemCategoryRef == right.itemCategoryRef);
    }

    friend QDebug operator<<(QDebug dbg, const GroupItem &right) {
        dbg.nospace() << "Group("
                      << "id:"    << right.itemId
                      << " name:" << right.itemName
                      << " cat:"  << right.itemCategoryRef
                      << ")";
        return dbg.maybeSpace();
    }

    using GroupList = QVector<GroupItem>;

    class GroupItemBuilder {
    public:
        qint32  groupId = 0;
        QString groupName = QString();
        qint32  groupCatRef = 0;

        GroupItemBuilder& setId      (const qint32   id)   { this->groupId     = id;   return *this; }
        GroupItemBuilder& setName    (const QString &name) { this->groupName   = name; return *this; }
        GroupItemBuilder& setCategory(const qint32  &cat)  { this->groupCatRef = cat; return *this; }
//        Builder& setProductP(){          // pre-defined group, if needed
//			this->i = 42;
//			this->f = -1.0f/12.0f;
//			this->c = '@';
//			return *this;
//		}
        GroupItem build() {
            return GroupItem(this->groupId, this->groupName, this->groupCatRef);
        }
    };
};

#endif // GROUPITEM_H
