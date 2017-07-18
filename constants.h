#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QDebug>

using IdMap  = QMultiHash<qint32, qint32>;
using IdPair = QPair<qint32, QString>;
using IdStringList = QVector<IdPair>;

struct ProductRelation {
    qint32 parentClass    = 0;
    qint32 parentCategory = 0;
    qint32 parentGroup    = 0;

    ProductRelation():
        parentClass(),
        parentCategory(),
        parentGroup()
    {}

    ProductRelation(const qint32 cls,
                      const qint32 cat,
                      const qint32 grp):
        parentClass(cls),
        parentCategory(cat),
        parentGroup(grp)
    {}

    ProductRelation(const ProductRelation &copy) = default;
    ProductRelation &operator=(const ProductRelation &right) = default;
    bool operator==(const ProductRelation &right) const {
        return (parentClass    == right.parentClass &&
                parentCategory == right.parentCategory &&
                parentGroup    == right.parentGroup);
    }

    friend QDebug operator<<(QDebug dbg, const ProductRelation &right) {
        dbg.nospace() << "ProductRelation("
                      << "class:"  << right.parentClass
                      << " cat:"   << right.parentCategory
                      << " group:" << right.parentGroup
                      << ")" ;
        return dbg.maybeSpace();
    }

    class ProductRelationBuilder {
    public:
        qint32 cls = 0;
        qint32 cat = 0;
        qint32 grp = 0;

        ProductRelationBuilder& setClass   (const qint32 cls) { this->cls = cls; return *this; }
        ProductRelationBuilder& setCategory(const qint32 cat) { this->cat = cat; return *this; }
        ProductRelationBuilder& setGroup   (const qint32 grp) { this->grp = grp; return *this; }
        ProductRelation build() {
            return ProductRelation(this->cls,
                                     this->cat,
                                     this->grp);
        }
    };
};

namespace Constants {

enum Role {
    RoleLevelId         = Qt::UserRole+1,
    RoleNodeType        = Qt::UserRole+2,
    RoleNodeId          = Qt::UserRole+3,
    RoleNodeHasChildren = Qt::UserRole+4,
    RoleSearchString    = Qt::UserRole+5,
    RoleProjectId       = Qt::UserRole+6,
    RoleBackground      = Qt::UserRole+7
};

enum TreeItemType {
    ItemClass = 0,
    ItemCategory,
    ItemGroup,
    ItemItem
};

enum TreeItemLevel {
    LevelRoot = 0,
    Level_1,
    Level_2,
    Level_3
};

enum DataValues {
    DataInvalid = -1
};

enum Color {
    ColorReceipt = 0xff006100,
    ColorExpense = 0xff9c0006,
    ColorReceiptBackground = 0xffBAFAAD,
    ColorExpenseBackground = 0xffFFD3C9,
    ColorSearchHighligh  = 0xffFFFF71,

    ColorLevel_1 = 0xffF0F6FF,
    ColorLevel_2 = 0xffE6F0FF,
    ColorLevel_3 = 0xffD3E5FF
};

}

#endif // CONSTANTS_H
