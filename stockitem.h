#ifndef STOCKITEM_H
#define STOCKITEM_H

#include <QObject>
#include <QDebug>

#include <abstracttreeitem.h>

class StockItem : public AbstractTreeItem
{
public:

    // TODO: move&swap idiom constructors;
    // данные
    qint32  itemAmount;
    QString itemSerialn;
    qint32  itemProject;
    qint32  itemLocation;
    qint32  itemProductRef;

    StockItem():
        AbstractTreeItem(),
        itemAmount(),
        itemSerialn(),
        itemProject(),
        itemLocation(),
        itemProductRef()
    {}
    StockItem(qint32 id, const QString &name, Constants::TreeItemType type, Constants::TreeItemLevel level, qint32 amount, const QString &serialn, const qint32 proj, const qint32 loc, const qint32 prod):
        AbstractTreeItem(id,
                         name,
                         type,
                         level),
        itemAmount      (amount),
        itemSerialn     (serialn),
        itemProject     (proj),
        itemLocation    (loc),
        itemProductRef  (prod)
    {}
    StockItem(const StockItem &copy):
        AbstractTreeItem(copy),
        itemAmount      (copy.itemAmount),
        itemSerialn     (copy.itemSerialn),
        itemProject     (copy.itemProject),
        itemLocation    (copy.itemLocation),
        itemProductRef  (copy.itemProductRef)
    {}
    ~StockItem(){}

    StockItem &operator=(const StockItem &right) {
        if (this != &right) {
            itemId         = right.itemId;
            itemName       = right.itemName;
            itemType       = right.itemType;
            itemLevel      = right.itemLevel;
            itemAmount     = right.itemAmount;
            itemSerialn    = right.itemSerialn;
            itemProject    = right.itemProject;
            itemLocation   = right.itemLocation;
            itemProductRef = right.itemProductRef;
        }
        return *this;
    }

    bool operator==(const StockItem &right) const {
        return (itemType       == right.itemType    &&
                itemLevel      == right.itemLevel   &&
                itemId         == right.itemId      &&
                itemName       == right.itemName    &&
                itemAmount     == right.itemAmount  &&
                itemSerialn    == right.itemSerialn &&
                itemProject    == right.itemProject &&
                itemLocation   == right.itemLocation&&
                itemProductRef == right.itemProductRef);
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
                      << " prod:" << right.itemProductRef
                      << ")";
        return dbg.maybeSpace();
    }

    using StockList = QVector<StockItem>;

    class StockItemBuilder {
    public:
        qint32        stockId         = 0;
        QString       stockName       = QString();
        Constants::TreeItemType  stockType       = Constants::ItemItem;
        Constants::TreeItemLevel stockLevel      = Constants::Level_2;
        qint32        stockAmount     = Constants::DataInvalid;
        QString       stockSerialn    = QString();
        qint32        stockProject    = 0;
        qint32        stockLocation   = 0;
        qint32        stockProductRef = 0;

        StockItemBuilder& setId       (const qint32    id)        { this->stockId         = id;        return *this; }
        StockItemBuilder& setName     (const QString  &name)      { this->stockName       = name;      return *this; }
        StockItemBuilder& setType     (const Constants::TreeItemType  itemtype) { this->stockType  = itemtype;  return *this; }
        StockItemBuilder& setLevel    (const Constants::TreeItemLevel itemlevel){ this->stockLevel = itemlevel; return *this; }
        StockItemBuilder& setAmount   (const qint32    amout)     { this->stockAmount     = amout;     return *this; }
        StockItemBuilder& setSerialn  (const QString  &serialn)   { this->stockSerialn    = serialn;   return *this; }
        StockItemBuilder& setProject  (const qint32   &project)   { this->stockProject    = project;   return *this; }
        StockItemBuilder& setLocation (const qint32   &loc)       { this->stockLocation   = loc;       return *this; }
        StockItemBuilder& setProduct  (const qint32   &prod)      { this->stockProductRef = prod;      return *this; }
        StockItem build() {
            return StockItem(this->stockId,      this->stockName,
                             this->stockType,    this->stockLevel,
                             this->stockAmount,  this->stockSerialn,
                             this->stockProject, this->stockLocation,
                             this->stockProductRef);
        }
    };
};

#endif // STOCKITEM_H
