#ifndef ABSTRACTTREEITEM_H
#define ABSTRACTTREEITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>
#include <constants.h>

class AbstractTreeItem : public AbstractItem
{
public:

    Constants::TreeItemType  itemType  = Constants::ItemCategory;
    Constants::TreeItemLevel itemLevel = Constants::LevelRoot;

    AbstractTreeItem():
        AbstractItem(),
        itemType (),
        itemLevel()
    {}

    AbstractTreeItem(qint32 id, const QString &name, Constants::TreeItemType type, Constants::TreeItemLevel level):
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
