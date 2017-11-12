#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>

class ProductItem : public AbstractItem
{
public:

    QString itemSerialn;
    QString itemUnit;
    QString itemMiscTag;

    explicit ProductItem():
        AbstractItem(),
        itemSerialn(),
        itemUnit(),
        itemMiscTag()
    {}

    explicit ProductItem(qint32         id,
                         const QString &name,
                         const QString &serialn,
                         const QString &unit,
                         const QString &miscTag):
        AbstractItem   (id, name),
        itemSerialn    (serialn),
        itemUnit       (unit),
        itemMiscTag    (miscTag)
    {}

     ProductItem(const ProductItem &copy):
        AbstractItem   (copy),
        itemSerialn    (copy.itemSerialn),
        itemUnit       (copy.itemUnit),
        itemMiscTag    (copy.itemMiscTag)
    {}


    ProductItem &operator=(const ProductItem &right) {
        if (this != &right) {
            itemId          = right.itemId;
            itemName        = right.itemName;
            itemSerialn     = right.itemSerialn;
            itemUnit        = right.itemUnit;
            itemMiscTag     = right.itemMiscTag;
        }
        return *this;
    }

    bool operator==(const ProductItem &right) const {
        return (itemId          == right.itemId        &&
                itemName        == right.itemName      &&
                itemSerialn     == right.itemSerialn   &&
                itemUnit        == right.itemUnit      &&
                itemMiscTag     == right.itemMiscTag);
    }

    friend QDebug operator<<(QDebug dbg, const ProductItem &right) {
        dbg.nospace() << "Product("
                      << "id:"      << right.itemId
                      << " name:"   << right.itemName
                      << " serial:" << right.itemSerialn
                      << " unit:"   << right.itemUnit
                      << " tag:"    << right.itemMiscTag
                      << ")";
        return dbg.maybeSpace();
    }

    using ProductList = QVector<ProductItem>;

    class ProductItemBuilder {
    public:
        qint32  prodId        = 0;
        QString prodName      = QString();
        QString prodSerialn   = QString();
        QString prodUnit      = QString("л");
        QString prodMiscTag   = QString();

        ProductItemBuilder(){}

        ProductItemBuilder(const ProductItem &item):
            prodId      (item.itemId),
            prodName    (item.itemName),
            prodSerialn (item.itemSerialn),
            prodUnit    (item.itemUnit),
            prodMiscTag (item.itemMiscTag)
        {}

        ProductItemBuilder& setId       (const qint32   id)      { this->prodId       = id;       return *this; }
        ProductItemBuilder& setName     (const QString &name)    { this->prodName     = name;     return *this; }
        ProductItemBuilder& setSerialn  (const QString &serialn) { this->prodSerialn  = serialn;  return *this; }
        ProductItemBuilder& setUnit     (const QString &unit)    { this->prodUnit     = unit;     return *this; }
        ProductItemBuilder& setMiscTag  (const QString &miscTag) { this->prodMiscTag  = miscTag;  return *this; }
        ProductItem build() {
            return ProductItem(this->prodId,       this->prodName,
                               this->prodSerialn,  this->prodUnit,
                               this->prodMiscTag);
        }
    };
};

#endif // PRODUCTITEM_H
