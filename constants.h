#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QDebug>

using IdMap  = QMultiHash<qint32, qint32>;
using IdPair = QPair<qint32, QString>;
using IdStringList = QVector<IdPair>;

struct ProductParentData {
    qint32 parentClass    = 0;
    qint32 parentCategory = 0;
    qint32 parentGroup    = 0;

    ProductParentData():
        parentClass(),
        parentCategory(),
        parentGroup()
    {}

    ProductParentData(const qint32 cls,
                      const qint32 cat,
                      const qint32 grp):
        parentClass(cls),
        parentCategory(cat),
        parentGroup(grp)
    {}

    ProductParentData(const ProductParentData &copy) = default;
    ProductParentData &operator=(const ProductParentData &right) = default;

    friend QDebug operator<<(QDebug dbg, const ProductParentData &right) {
        dbg.nospace() << "ProductParentIds("
                      << "class:"  << right.parentClass
                      << "  cat:"  << right.parentCategory
                      << " group:" << right.parentGroup
                      << ")" ;
        return dbg.maybeSpace();
    }

    class ProductParentDataBuilder {
    public:
        qint32 cls = 0;
        qint32 cat = 0;
        qint32 grp = 0;

        ProductParentDataBuilder& setClass   (const qint32 cls) { this->cls = cls; return *this; }
        ProductParentDataBuilder& setCategory(const qint32 cat) { this->cat = cat; return *this; }
        ProductParentDataBuilder& setGroup   (const qint32 grp) { this->grp = grp; return *this; }
        ProductParentData build() {
            return ProductParentData(this->cls,
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
    RoleProjectId       = Qt::UserRole+6
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
    ColorSearchHighligh  = 0xffFFFF71
};

}

#endif // CONSTANTS_H
