#ifndef STOCKITEM_H
#define STOCKITEM_H

#include <QObject>
#include <QDebug>

class StockItem
{
public:

    // TODO: move&swap idiom constructors;
    enum ItemType {
        ItemCategory = 0,
        ItemGroup,
        ItemProduct
    };

    enum ItemLevel {
        LevelRoot = 0,
        Level_1,
        Level_2
    };

    // данные
    qint32  itemType;
    qint32  itemLevel;
    qint32  itemId;
    QString itemName;
    qint32  itemAmount;
    QString itemSerialn;
    QString itemLocation;

    StockItem():
        itemType(),
        itemLevel(),
        itemId(),
        itemName(),
        itemAmount(),
        itemSerialn(),
        itemLocation()
    {}
    StockItem(ItemType type, ItemLevel level, qint32 id, const QString &name, qint32 amount, const QString &serialn, const QString &location):
        itemType    (type),
        itemLevel   (level),
        itemId      (id),
        itemName    (name),
        itemAmount  (amount),
        itemSerialn (serialn),
        itemLocation(location)
    {}
    StockItem(const StockItem &copy):
        itemType    (copy.itemType),
        itemLevel   (copy.itemLevel),
        itemId      (copy.itemId),
        itemName    (copy.itemName),
        itemAmount  (copy.itemAmount),
        itemSerialn (copy.itemSerialn),
        itemLocation(copy.itemLocation)
    {}
    ~StockItem(){}

    StockItem &operator=(const StockItem &right) {
        if (this != &right) {
            itemType     = right.itemType;
            itemLevel    = right.itemLevel;
            itemId       = right.itemId;
            itemName     = right.itemName;
            itemAmount   = right.itemAmount;
            itemSerialn  = right.itemSerialn;
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
                      << " loc:"  << right.itemLocation
                      << ")";
        return dbg.maybeSpace();
    }

    using StockList = QVector<StockItem>;
};

#endif // STOCKITEM_H
