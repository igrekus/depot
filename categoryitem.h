#ifndef CATEGORYITEM_H
#define CATEGORYITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>

class CategoryItem : public AbstractItem
{
public:

    explicit CategoryItem():
        AbstractItem()
    {}

    explicit CategoryItem(qint32 id, const QString &name):
        AbstractItem(id, name)
    {}

    explicit CategoryItem(const CategoryItem &copy):
        AbstractItem(copy.itemId, copy.itemName)
    {}

    CategoryItem &operator=(const CategoryItem &right) {
        if (this != &right) {
            itemId   = right.itemId;
            itemName = right.itemName;
        }
        return *this;
    }

    bool operator==(const CategoryItem &right) const {
        return (itemId   == right.itemId &&
                itemName == right.itemName);
    }

    friend QDebug operator<<(QDebug dbg, const CategoryItem &right) {
        dbg.nospace() << "Category("
                      << "id:"    << right.itemId
                      << " name:" << right.itemName
                      << ")";
        return dbg.maybeSpace();
    }
    using CategoryList = QVector<CategoryItem>;
};

#endif // CATEGORYITEM_H
