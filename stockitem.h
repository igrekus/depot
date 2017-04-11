#ifndef STOCKITEM_H
#define STOCKITEM_H

#include <QObject>
#include <QDebug>

#include <abstracttreeitem.h>

#define DATA_INVALID -1

class StockItem : public AbstractTreeItem
{
public:

    // TODO: move&swap idiom constructors;
    // данные
    qint32    itemAmount;
    QString   itemSerialn;
    QString   itemProject;
    QString   itemLocation;

    StockItem():
        AbstractTreeItem(),
        itemAmount(),
        itemSerialn(),
        itemProject(),
        itemLocation()
    {}
    StockItem(qint32 id, const QString &name, TreeItemType type, TreeItemLevel level, qint32 amount, const QString &serialn, const QString &proj, const QString &loc):
        AbstractTreeItem(id,
                         name,
                         type,
                         level),
        itemAmount      (amount),
        itemSerialn     (serialn),
        itemProject     (proj),
        itemLocation    (loc)
    {}
    StockItem(const StockItem &copy):
        AbstractTreeItem(copy),
        itemAmount      (copy.itemAmount),
        itemSerialn     (copy.itemSerialn),
        itemProject     (copy.itemProject),
        itemLocation    (copy.itemLocation)
    {}
    ~StockItem(){}

    StockItem &operator=(const StockItem &right) {
        if (this != &right) {
            itemId       = right.itemId;
            itemName     = right.itemName;
            itemType     = right.itemType;
            itemLevel    = right.itemLevel;
            itemAmount   = right.itemAmount;
            itemSerialn  = right.itemSerialn;
            itemProject  = right.itemProject;
            itemLocation = right.itemLocation;
        }
        return *this;
    }

    bool operator==(const StockItem &right) const {
        return (itemType     == right.itemType    &&
                itemLevel    == right.itemLevel   &&
                itemId       == right.itemId      &&
                itemName     == right.itemName    &&
                itemAmount   == right.itemAmount  &&
                itemSerialn  == right.itemSerialn &&
                itemProject  == right.itemProject &&
                itemLocation == right.itemLocation);
    }

    friend QDebug operator<<(QDebug dbg, const StockItem &right) {
        dbg.nospace() << "StockItem("
                      << "type:"  << right.itemType
                      << " lvl:"  << right.itemLevel
                      << " id:"   << right.itemId
                      << " name:" << right.itemName
                      << " amt:"  << right.itemAmount
                      << " sn:"   << right.itemSerialn
                      << " proj:" << right.itemProject
                      << " loc:"  << right.itemLocation
                      << ")";
        return dbg.maybeSpace();
    }

    using StockList = QList<StockItem>;

    class StockItemBuilder {
    public:
        qint32        stockId         = 0;
        QString       stockName       = QString();
        TreeItemType  stockType       = ItemItem;
        TreeItemLevel stockLevel      = Level_2;
        qint32        stockAmount     = DATA_INVALID;
        QString       stockSerialn    = QString();
        QString       stockProject    = QString();
        QString       stockLocation   = QString();

        StockItemBuilder& setId       (const qint32    id)        { this->stockId       = id;        return *this; }
        StockItemBuilder& setName     (const QString  &name)      { this->stockName     = name;      return *this; }
        StockItemBuilder& setType     (const TreeItemType  itemtype) { this->stockType  = itemtype;  return *this; }
        StockItemBuilder& setLevel    (const TreeItemLevel itemlevel){ this->stockLevel = itemlevel; return *this; }
        StockItemBuilder& setAmount   (const qint32    amout)     { this->stockAmount   = amout;     return *this; }
        StockItemBuilder& setSerialn  (const QString  &serialn)   { this->stockSerialn  = serialn;   return *this; }
        StockItemBuilder& setProject  (const QString  &project)   { this->stockProject  = project;   return *this; }
        StockItemBuilder& setLocation (const QString  &loc)       { this->stockLocation = loc;       return *this; }
        StockItem build() {
            return StockItem(this->stockId,      this->stockName,
                             this->stockType,    this->stockLevel,
                             this->stockAmount,  this->stockSerialn,
                             this->stockProject, this->stockLocation);
        }
    };
};

#endif // STOCKITEM_H
