#ifndef CATEGORYITEM_H
#define CATEGORYITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>

class CategoryItem : public AbstractItem
{
public:

    CategoryItem():
        AbstractItem()
    {}

    explicit CategoryItem(const qint32 id, const QString &name):
        AbstractItem(id, name)
    {}

    CategoryItem(const CategoryItem &copy):
        AbstractItem(copy)
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
    using CategoryList = QList<CategoryItem>;

    class CategoryItemBuilder {
    public:
        qint32  catId = 0;
        QString catName = QString();

        CategoryItemBuilder& setId  (const qint32   id)   { this->catId   = id;   return *this; }
        CategoryItemBuilder& setName(const QString &name) { this->catName = name; return *this; }
//        Builder& setProductP(){          // pre-defined category, if needed
//			this->i = 42;
//			this->f = -1.0f/12.0f;
//			this->c = '@';
//			return *this;
//		}
        CategoryItem build() {
            return CategoryItem(this->catId, this->catName);
        }
    };
};

#endif // CATEGORYITEM_H
