#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <QObject>
#include <QDebug>

#include <abstracttreeitem.h>

class InventoryItem : public AbstractTreeItem
{
public:

    QString   itemSerialn;
    QString   itemUnit;
    QString   itemMiscTag;

    InventoryItem():
        AbstractTreeItem(),
        itemSerialn(),
        itemUnit(),
        itemMiscTag()
    {}
    InventoryItem(qint32 id,
                  const QString &name,
                  Constants::TreeItemType type,
                  Constants::TreeItemLevel level,
                  const QString &serialn,
                  const QString &unit,
                  const QString misctag):
        AbstractTreeItem(id,
                         name,
                         type,
                         level),
        itemSerialn(serialn),
        itemUnit(unit),
        itemMiscTag(misctag)
    {}
    InventoryItem(const InventoryItem &copy):
        AbstractTreeItem(copy),
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
                itemSerialn  == right.itemSerialn &&
                itemUnit     == right.itemUnit    &&
                itemMiscTag  == right.itemMiscTag);
    }
    friend QDebug operator<<(QDebug dbg, const InventoryItem &right) {
        dbg.nospace() << "InventoryItem("
                      << "type:"  << right.itemType
                      << " lvl:"  << right.itemLevel
                      << " id:"   << right.itemId
                      << " name:" << right.itemName
                      << " sn:"   << right.itemSerialn
                      << " unit:" << right.itemUnit
                      << " mtag:" << right.itemMiscTag
                      << ")";
        return dbg.maybeSpace();
    }

    using InventoryList = QVector<InventoryItem>;

    class InventoryItemBuilder {
    public:
        qint32        invId       = 0;
        QString       invName     = QString();
        Constants::TreeItemType  invType     = Constants::ItemItem;
        Constants::TreeItemLevel invLevel    = Constants::Level_2;
        QString       invSerialn  = QString();
        QString       invUnit     = QString();
        QString       invMiscTag  = QString();

        InventoryItemBuilder& setId      (const qint32    id)           { this->invId       = id;        return *this; }
        InventoryItemBuilder& setName    (const QString  &name)         { this->invName     = name;      return *this; }
        InventoryItemBuilder& setType    (const Constants::TreeItemType  itemtype) { this->invType     = itemtype;  return *this; }
        InventoryItemBuilder& setLevel   (const Constants::TreeItemLevel itemlevel){ this->invLevel    = itemlevel; return *this; }
        InventoryItemBuilder& setSerialn (const QString  &serialn)      { this->invSerialn  = serialn;   return *this; }
        InventoryItemBuilder& setUnit    (const QString  &unit)         { this->invUnit     = unit;      return *this; }
        InventoryItemBuilder& setMiscTag (const QString  &misctag)      { this->invMiscTag  = misctag;   return *this; }
        InventoryItem build() {
            return InventoryItem(this->invId,       this->invName,
                                 this->invType,     this->invLevel,
                                 this->invSerialn,  this->invUnit,
                                 this->invMiscTag);
        }
    };
};

#endif // INVENTORYITEM_H
