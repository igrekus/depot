#ifndef ABSTRACTTREEITEM_H
#define ABSTRACTTREEITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>

class AbstractTreeItem : public AbstractItem
{
public:
    enum TreeItemType {
        ItemCategory = 0,
        ItemGroup,
        ItemItem
    };

    enum TreeItemLevel {
        LevelRoot = 0,
        Level_1,
        Level_2
    };

    TreeItemType  itemType  = ItemCategory;
    TreeItemLevel itemLevel = LevelRoot;

    AbstractTreeItem():
        AbstractItem(),
        itemType (),
        itemLevel()
    {}

    AbstractTreeItem(qint32 id, const QString &name, TreeItemType type, TreeItemLevel level):
        AbstractItem(id,
                     name),
        itemType (type),
        itemLevel(level)
    {}

    AbstractTreeItem(const AbstractTreeItem &copy):
        AbstractItem(copy),
        itemType (copy.itemType),
        itemLevel(copy.itemLevel)
    {}

    virtual ~AbstractTreeItem(){}
};

#endif // ABSTRACTTREEITEM_H
