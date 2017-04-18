#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>

using IdMap = QMultiHash<qint32, qint32>;

namespace Constants {

enum Role {
    RoleLevelId         = Qt::UserRole+1,
    RoleNodeType        = Qt::UserRole+2,
    RoleNodeId          = Qt::UserRole+3,
    RoleNodeHasChildren = Qt::UserRole+4
};

enum TreeItemType {
    ItemCategory = 0,
    ItemGroup,
    ItemItem
};

enum TreeItemLevel {
    LevelRoot = 0,
    Level_1,
    Level_2
};

enum DataValues {
    DataInvalid = -1
};

}

#endif // CONSTANTS_H
