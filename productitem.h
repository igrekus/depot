#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>

class ProductItem : public AbstractItem
{
public:

    QString itemFullname;
    QString itemSerialn;
    QString itemUnit;
    QString itemMiscTag;
    qint32  itemGroupRef;
    qint32  itemCategoryRef;

    explicit ProductItem():
        AbstractItem(),
        itemFullname(),
        itemSerialn(),
        itemUnit(),
        itemMiscTag(),
        itemGroupRef(),
        itemCategoryRef()
    {}

    explicit ProductItem(qint32         id,
                         const QString &name,
                         const QString &fullname,
                         const QString &serialn,
                         const QString &unit,
                         const QString &miscTag,
                         qint32         group,
                         qint32         category):
        AbstractItem   (id, name),
        itemFullname   (fullname),
        itemSerialn    (serialn),
        itemUnit       (unit),
        itemMiscTag    (miscTag),
        itemGroupRef   (group),
        itemCategoryRef(category)
    {}

     ProductItem(const ProductItem &copy):
        AbstractItem   (copy),
        itemFullname   (copy.itemFullname),
        itemSerialn    (copy.itemSerialn),
        itemUnit       (copy.itemUnit),
        itemMiscTag    (copy.itemMiscTag),
        itemGroupRef   (copy.itemGroupRef),
        itemCategoryRef(copy.itemCategoryRef)
    {}


    ProductItem &operator=(const ProductItem &right) {
        if (this != &right) {
            itemId          = right.itemId;
            itemName        = right.itemName;
            itemFullname    = right.itemFullname;
            itemSerialn     = right.itemSerialn;
            itemUnit        = right.itemUnit;
            itemMiscTag     = right.itemMiscTag;
            itemGroupRef    = right.itemGroupRef;
            itemCategoryRef = right.itemCategoryRef;
        }
        return *this;
    }

    bool operator==(const ProductItem &right) const {
        return (itemId          == right.itemId        &&
                itemName        == right.itemName      &&
                itemFullname    == right.itemFullname  &&
                itemSerialn     == right.itemSerialn   &&
                itemUnit        == right.itemUnit      &&
                itemMiscTag     == right.itemMiscTag   &&
                itemGroupRef    == right.itemGroupRef  &&
                itemCategoryRef == right.itemCategoryRef);
    }

    friend QDebug operator<<(QDebug dbg, const ProductItem &right) {
        dbg.nospace() << "Product("
                      << "id:"      << right.itemId
                      << " name:"   << right.itemName
                      << " fname:"  << right.itemFullname
                      << " serial:" << right.itemSerialn
                      << " unit:"   << right.itemUnit
                      << " tag:"    << right.itemMiscTag
                      << " grp:"    << right.itemGroupRef
                      << " cat:"    << right.itemCategoryRef
                      << ")";
        return dbg.maybeSpace();
    }

    using ProductList = QVector<ProductItem>;

    class ProductItemBuilder {
    public:
        qint32  prodId        = 0;
        QString prodName      = QString();
        QString prodFullname  = QString();
        QString prodSerialn   = QString();
        QString prodUnit      = QString();
        QString prodMiscTag   = QString();
        qint32  prodGroupRef  = 0;
        qint32  prodCatRef    = 0;

        ProductItemBuilder(){}

        ProductItemBuilder(const ProductItem &item):
            prodId      (item.itemId),
            prodName    (item.itemName),
            prodFullname(item.itemFullname),
            prodSerialn (item.itemSerialn),
            prodUnit    (item.itemUnit),
            prodMiscTag (item.itemMiscTag),
            prodGroupRef(item.itemGroupRef),
            prodCatRef  (item.itemCategoryRef)
        {}

        ProductItemBuilder& setId       (const qint32   id)      { this->prodId       = id;       return *this; }
        ProductItemBuilder& setName     (const QString &name)    { this->prodName     = name;     return *this; }
        ProductItemBuilder& setFullname (const QString &fullname){ this->prodFullname = fullname; return *this; }
        ProductItemBuilder& setSerialn  (const QString &serialn) { this->prodSerialn  = serialn;  return *this; }
        ProductItemBuilder& setUnit     (const QString &unit)    { this->prodUnit     = unit;     return *this; }
        ProductItemBuilder& setMiscTag  (const QString &miscTag) { this->prodMiscTag  = miscTag;  return *this; }
        ProductItemBuilder& setGroup    (const qint32   group)   { this->prodGroupRef = group;    return *this; }
        ProductItemBuilder& setCategory (const qint32   cat)     { this->prodCatRef   = cat;      return *this; }
        ProductItem build() {
            return ProductItem(this->prodId,       this->prodName,
                               this->prodFullname, this->prodSerialn,
                               this->prodUnit,     this->prodMiscTag,
                               this->prodGroupRef, this->prodCatRef);
        }
    };
};

#endif // PRODUCTITEM_H
