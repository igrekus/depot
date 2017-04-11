#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <QObject>
#include <QDebug>

#include <abstracttreeitem.h>

#define DATA_INVALID -1

class InventoryItem : public AbstractTreeItem
{
public:

    QString   itemFullname;
    QString   itemSerialn;
    QString   itemUnit;
    QString   itemMiscTag;

    InventoryItem():
        AbstractTreeItem(),
        itemFullname(),
        itemSerialn(),
        itemUnit(),
        itemMiscTag()
    {}
    InventoryItem(qint32 id, const QString &name, TreeItemType type, TreeItemLevel level, const QString &fullname, const QString &serialn, const QString &unit, const QString misctag):
        AbstractTreeItem(id,
                         name,
                         type,
                         level),
        itemFullname(fullname),
        itemSerialn(serialn),
        itemUnit(unit),
        itemMiscTag(misctag)
    {}
    InventoryItem(const InventoryItem &copy):
        AbstractTreeItem(copy),
        itemFullname    (copy.itemFullname),
        itemSerialn     (copy.itemSerialn),
        itemUnit        (copy.itemUnit),
        itemMiscTag     (copy.itemMiscTag)
    {}
    ~InventoryItem(){}

    InventoryItem &operator=(const InventoryItem &right) {
        if (this != &right) {
            itemId       = right.itemId;
            itemName     = right.itemName;
            itemType     = right.itemType;
            itemLevel    = right.itemLevel;
            itemFullname = right.itemFullname;
            itemSerialn  = right.itemSerialn;
            itemUnit     = right.itemUnit;
            itemMiscTag  = right.itemMiscTag;
        }
        return *this;
    }
    bool operator==(const InventoryItem &right) const {
        return (itemId       == right.itemId      &&
                itemName     == right.itemName    &&
                itemType     == right.itemType    &&
                itemLevel    == right.itemLevel   &&
                itemFullname == right.itemFullname&&
                itemSerialn  == right.itemSerialn &&
                itemUnit     == right.itemUnit    &&
                itemMiscTag  == right.itemMiscTag);
    }
    friend QDebug operator<<(QDebug dbg, const InventoryItem &right) {
        dbg.nospace() << "StockItem("
                      << "type:"  << right.itemType
                      << " lvl:"  << right.itemLevel
                      << " id:"   << right.itemId
                      << " name:" << right.itemName
                      << " fname:"<< right.itemFullname
                      << " sn:"   << right.itemSerialn
                      << " unit:" << right.itemUnit
                      << " mtag:" << right.itemMiscTag
                      << ")";
        return dbg.maybeSpace();
    }

    using InventoryList = QList<InventoryItem>;

    class InventoryItemBuilder {
    public:
        qint32        invId       = 0;
        QString       invName     = QString();
        TreeItemType  invType     = ItemItem;
        TreeItemLevel invLevel    = Level_2;
        QString       invFullname = QString();
        QString       invSerialn  = QString();
        QString       invUnit     = QString();
        QString       invMiscTag  = QString();

        InventoryItemBuilder& setId      (const qint32    id)           { this->invId       = id;        return *this; }
        InventoryItemBuilder& setName    (const QString  &name)         { this->invName     = name;      return *this; }
        InventoryItemBuilder& setType    (const TreeItemType  itemtype) { this->invType     = itemtype;  return *this; }
        InventoryItemBuilder& setLevel   (const TreeItemLevel itemlevel){ this->invLevel    = itemlevel; return *this; }
        InventoryItemBuilder& setFullname(const QString  &fullname)     { this->invFullname = fullname;  return *this; }
        InventoryItemBuilder& setSerialn (const QString  &serialn)      { this->invSerialn  = serialn;   return *this; }
        InventoryItemBuilder& setUnit    (const QString  &unit)         { this->invUnit     = unit;      return *this; }
        InventoryItemBuilder& setMiscTag (const QString  &misctag)      { this->invMiscTag  = misctag;   return *this; }
        InventoryItem build() {
            return InventoryItem(this->invId,       this->invName,
                                 this->invType,     this->invLevel,
                                 this->invFullname, this->invSerialn,
                                 this->invUnit,     this->invMiscTag);
        }
    };
};

#endif // INVENTORYITEM_H
