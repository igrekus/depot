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
    QString itemProjecTag;
    QString itemMiscTag;
    qint32  itemCode;

    explicit ProductItem():
        AbstractItem(),
        itemFullname(),
        itemSerialn(),
        itemUnit(),
        itemProjecTag(),
        itemMiscTag(),
        itemCode()
    {}

    explicit ProductItem(qint32 id,
                         const QString &name,
                         const QString &fullname,
                         const QString &serialn,
                         const QString &unit,
                         const QString &projTag,
                         const QString &miscTag,
                         qint32 code):
        AbstractItem (id, name),
        itemFullname (fullname),
        itemSerialn  (serialn),
        itemUnit     (unit),
        itemProjecTag(projTag),
        itemMiscTag  (miscTag),
        itemCode     (code)
    {}

     ProductItem(const ProductItem &copy):
        AbstractItem (copy),
        itemFullname (copy.itemFullname),
        itemSerialn  (copy.itemSerialn),
        itemUnit     (copy.itemUnit),
        itemProjecTag(copy.itemProjecTag),
        itemMiscTag  (copy.itemMiscTag),
        itemCode     (copy.itemCode)
    {}


    ProductItem &operator=(const ProductItem &right) {
        if (this != &right) {
            itemId        = right.itemId;
            itemName      = right.itemName;
            itemFullname  = right.itemFullname;
            itemSerialn   = right.itemSerialn;
            itemUnit      = right.itemUnit;
            itemProjecTag = right.itemProjecTag;
            itemMiscTag   = right.itemMiscTag;
            itemCode      = right.itemCode;
        }
        return *this;
    }

    bool operator==(const ProductItem &right) const {
        return (itemId        == right.itemId        &&
                itemName      == right.itemName      &&
                itemFullname  == right.itemFullname  &&
                itemSerialn   == right.itemSerialn   &&
                itemUnit      == right.itemUnit      &&
                itemProjecTag == right.itemProjecTag &&
                itemMiscTag   == right.itemMiscTag   &&
                itemCode      == right.itemCode);
    }

    friend QDebug operator<<(QDebug dbg, const ProductItem &right) {
        dbg.nospace() << "Product("
                      << "id:"       << right.itemId
                      << " name:"    << right.itemName
                      << " fname:"   << right.itemFullname
                      << " serialn:" << right.itemSerialn
                      << " unit:"    << right.itemUnit
                      << " projtag:" << right.itemProjecTag
                      << " micstag:" << right.itemMiscTag
                      << " code:"    << right.itemCode
                      << ")";
        return dbg.maybeSpace();
    }

    using ProductList = QVector<ProductItem>;

    class ProductItemBuilder {
    public:
        qint32  prodId = 0;
        QString prodName = QString();
        QString prodFullname = QString();
        QString prodSerialn = QString();
        QString prodUnit = QString();
        QString prodProjecTag = QString();
        QString prodMiscTag = QString();
        qint32  prodCode = 0;

        ProductItemBuilder& setId(const qint32 id) {
            this->prodId = id;
            return *this;
        }
        ProductItemBuilder& setName(const QString &name) {
            this->prodName = name;
            return *this;
        }
        ProductItemBuilder& setFullname(const QString &fullname) {
            this->prodFullname = fullname;
            return *this;
        }
        ProductItemBuilder& setSerialn(const QString &serialn) {
            this->prodSerialn = serialn;
            return *this;
        }
        ProductItemBuilder& setUnit(const QString &unit) {
            this->prodUnit = unit;
            return *this;
        }
        ProductItemBuilder& setProjetTag(const QString &projTag) {
            this->prodProjecTag = projTag;
            return *this;
        }
        ProductItemBuilder& setMiscTag(const QString &miscTag) {
            this->prodMiscTag = miscTag;
            return *this;
        }
        ProductItemBuilder& setCode(const qint32 code) {
            this->prodCode = code;
            return *this;
        }
        ProductItem buildProductItem() {
            return ProductItem(this->prodId,       this->prodName,
                               this->prodFullname, this->prodSerialn,
                               this->prodUnit,     this->prodProjecTag,
                               this->prodMiscTag,  this->prodCode);
        }
    };
};

#endif // PRODUCTITEM_H
