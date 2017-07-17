#include "inventorymodel.h"

struct InventoryModel::InventoryNode
{
    InventoryNode():
        inventoryItem(),
        children(),
        parent(nullptr),
        mapped(false),
        m_isExpanded(false)
    {}

    InventoryNode(const InventoryItem &invItem, InventoryNode *par = nullptr):
        inventoryItem(invItem),
        children(),
        parent(par),
        mapped(false),
        m_isExpanded(false)
    {}

    bool operator==(const InventoryNode &a) const
    {
        bool r = this->inventoryItem == a.inventoryItem;
        Q_ASSERT(!r || this->parent == a.parent);
        Q_ASSERT(!r || this->mapped == a.mapped);
        Q_ASSERT(!r || this->children == a.children);
        return r;
    }

    friend QDebug operator<<(QDebug dbg, const InventoryNode &right) {
        dbg.nospace() << "InventoryNode("
                      << "data:"    << right.inventoryItem
                      << " child:"  << right.children
                      << " parent:" << right.parent
                      << " row:"    << right.selfRow
                      << ")";
        return dbg.maybeSpace();
    }
    InventoryItem inventoryItem;                   // данные узла
    InventoryModel::InventoryNodeList children;    // массив ссылок на дочерние узлы
    InventoryNode *parent = nullptr;               // ссылка на родительский узел
    qint32 selfRow = 0;
    bool mapped = false;                           // проводился ли поиск дочерних узлов?
    bool m_isExpanded = false;                     // раскрыт ли узел в view?

    InventoryModel::InventoryNodeList siblings() const {return parent->children;}
};

InventoryModel::InventoryModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

InventoryModel::~InventoryModel()
{

}

InventoryModel::InventoryNode InventoryModel::makeClassNode(const ClassItem &item)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId   (item.itemId)
                                         .setName (item.itemName)
                                         .setType (Constants::ItemClass)
                                         .setLevel(Constants::LevelRoot)
                                         .build(),
                                         nullptr);
}

InventoryModel::InventoryNode InventoryModel::makeCategoryNode(const CategoryItem &item, InventoryNode *parent)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId   (item.itemId)
                                         .setName (item.itemName)
                                         .setType (Constants::ItemCategory)
                                         .setLevel(Constants::Level_1)
                                         .build(),
                                         parent);
}

InventoryModel::InventoryNode InventoryModel::makeGroupNode(const GroupItem &item, InventoryNode *parent)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId   (item.itemId)
                                         .setName (item.itemName)
                                         .setType (Constants::ItemGroup)
                                         .setLevel(Constants::Level_2)
                                         .build(),
                                         parent);
}

InventoryModel::InventoryNode InventoryModel::makeProductNode(const ProductItem &item, InventoryModel::InventoryNode *parent)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId(item.itemId)
                                         .setName(item.itemName)
                                         .setType (Constants::ItemItem)
                                         .setLevel(Constants::Level_3)
                                         .setFullname(item.itemFullname)
                                         .setSerialn(item.itemSerialn)
                                         .setUnit(item.itemUnit)
                                         .setMiscTag(item.itemMiscTag)
                                         .build(),
                                         parent);
}

void InventoryModel::fillClassNode(const QModelIndex &index, InventoryNode &node)
{
    CategoryItem::CategoryList list = m_dbman->getCategoryList(node.inventoryItem.itemId);
    qint32 count = list.size()-1;
    if (count < 0)
        count = 0;
    beginInsertRows(index, 0, count);
    for (const CategoryItem &it : list) {
        node.children.append(std::move(makeCategoryNode(it, &node)));
    }
    endInsertRows();
}

void InventoryModel::fillCategoryNode(const QModelIndex &index, InventoryNode &node)
{
    GroupItem::GroupList list = m_dbman->getGroupList(node.inventoryItem.itemId);
    qint32 count = list.size()-1;
    if (count < 0)
        count = 0;
    beginInsertRows(index, 0, count);
    for (const GroupItem &it : list) {
        node.children.append(std::move(makeGroupNode(it, &node)));
    }
    endInsertRows();
}

void InventoryModel::fillGroupNode(const QModelIndex &index, InventoryNode &node)
{
    ProductItem::ProductList list = m_dbman->getProductListByGroup(node.inventoryItem.itemId);
    qint32 count = list.size()-1;
    if (count < 0)
        count = 0;
    beginInsertRows(index, 0, count);
    for (const ProductItem &it : list) {
        node.children.append(std::move(makeProductNode(it, &node)));
    }
    endInsertRows();
}

void InventoryModel::buildClassLevel()
{
    qDebug() << "inventory: building class level (0)";
    ClassItem::ClassList list = m_dbman->getClassList();
    qint32 count = list.size()-1;
    if (count < 0)
        count = 0;
    beginInsertRows(QModelIndex(), 0, count);
    for (const ClassItem &it : list) {
        m_nodes.append(std::move(makeClassNode(it)));
    }
    endInsertRows();
}

void InventoryModel::buildCategoryLevel()
{
    qDebug() << "inventory: building category level (1)";
    qint32 i=0;
    for (InventoryNode &classNode : m_nodes) {
        fillClassNode(createIndex(i, 0, &classNode), classNode);
        ++i;
    }
}

void InventoryModel::buildGroupLevel()
{
    qDebug() << "inventory: building group level (2)";
    for (InventoryNode &classNode : m_nodes) {
        qint32 i=0;
        for (InventoryNode &categoryNode : classNode.children) {
            fillCategoryNode(createIndex(i, 0, &categoryNode), categoryNode);
            ++i;
        }
    }
}

void InventoryModel::buildProductLevel()
{
    qDebug() << "inventory: building product level (3)";
    for (InventoryNode &classNode : m_nodes) {
        for (InventoryNode &categoryNode : classNode.children) {
            qint32 i=0;
            for (InventoryNode &groupNode : categoryNode.children) {
                fillGroupNode(createIndex(i, 0, &groupNode), groupNode);
                ++i;
            }
        }
    }
}

void InventoryModel::initModel()
{
    buildClassLevel();
    buildCategoryLevel();
    buildGroupLevel();
    buildProductLevel();
}

QVariant InventoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < m_headers.size()) {
        return m_headers[section];
    }
    return QVariant();
}

QModelIndex InventoryModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        Q_ASSERT(m_nodes.size() > row);
        return createIndex(row, column, const_cast<InventoryNode*>(&m_nodes.at(row)));
    }

    InventoryNode *parentNode = static_cast<InventoryNode *>(parent.internalPointer());
    Q_ASSERT(parentNode!=nullptr);
//    Q_ASSERT(parentNode->mapped);
    Q_ASSERT(parentNode->children.size() > row);
    return createIndex(row, column, &parentNode->children[row]);
}

QModelIndex InventoryModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    InventoryNode *currentNode = static_cast<InventoryNode *>(index.internalPointer());
    Q_ASSERT(currentNode != nullptr);

    InventoryNode* parentNode = currentNode->parent;
    if (parentNode != nullptr) {
        return createIndex(findRow(parentNode), RamificationColumn, parentNode);
    }
    else {
        return QModelIndex();
    }
}

int InventoryModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_nodes.size();
    }
    const InventoryNode* parentNode = static_cast<const InventoryNode*>(parent.internalPointer());
    Q_ASSERT(parentNode != nullptr);

    return parentNode->children.size();
}

int InventoryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant InventoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const InventoryNode *tmpnode = static_cast<InventoryNode *>(index.internalPointer());
    Q_ASSERT(tmpnode != nullptr);

    const InventoryItem tmpitem = tmpnode->inventoryItem;

    switch (role) {
    case Qt::DisplayRole: {
        switch (index.column()) {
        case CategoryColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemId;
            }
            return tmpitem.itemName;
            break;
        }
        case CodeColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemId;
            }
            break;
        }
        case NameColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemName;
            }
            break;
        }
        case UnitColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemUnit;
            }
            break;
        }
        case SerialnColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemSerialn;
            }
            break;
        }
        case FullnameColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemFullname;
            }
            break;
        }
        case ColumnCount:
        default:
            break;
        }
        break;
    }
    case Constants::RoleLevelId: {
        return tmpitem.itemLevel;
        break;
    }
    case Constants::RoleNodeType: {
        return tmpitem.itemType;
        break;
    }
    case Constants::RoleNodeId: {
        return tmpitem.itemId;
        break;
    }
    case Constants::RoleNodeHasChildren: {
        return !tmpnode->children.isEmpty();
        break;
    }
    case Constants::RoleSearchString: {
//        return QVariant(QString(tmpnode->inventoryItem.itemId+";"+
//                                tmpnode->inventoryItem.itemName+";"+
//                                tmpnode->inventoryItem.itemUnit+";"+
//                                tmpnode->inventoryItem.itemSerialn+";"+
//                                tmpnode->inventoryItem.itemFullname+";"));
    break;
    }
    default:
        break;
    }
    return QVariant();
}

int InventoryModel::findRow(const InventoryNode *invNode) const
{
    Q_ASSERT(invNode != nullptr);
    const InventoryNodeList searchList = invNode->parent != nullptr ? invNode->siblings() : m_nodes;
//    return searchList.indexOf(*invNode);
    InventoryNodeList::const_iterator position = std::find(searchList.begin(), searchList.end(), *invNode);
//    Q_ASSERT(position != searchList.end());
    return std::distance(searchList.begin(), position);
}

QModelIndex InventoryModel::addCategory(const QModelIndex &pindex, const QString &catName)
{
    // TODO: obey custom sort order
    InventoryNode *pnode = static_cast<InventoryNode *>(pindex.internalPointer());
    qint32 classId = pnode->inventoryItem.itemId;
    qint32 newId = m_dbman->insertCategory(classId, catName);

    auto row_iterator = std::find_if(pnode->children.begin(), pnode->children.end(),
                                     [&catName](const InventoryNode &it){
                                        return QString::localeAwareCompare(it.inventoryItem.itemName, catName) > 0;
                                     });

    qint32 row = std::distance(pnode->children.begin(), row_iterator);
    beginInsertRows(pindex, row, row);
    pnode->children.insert(row, makeCategoryNode(CategoryItem::CategoryItemBuilder()
                                                 .setId(newId)
                                                 .setName(catName)
                                                 .build(),
                                                 pnode));
    endInsertRows();
    return index(row, 0, pindex);
}

void InventoryModel::editCategory(const QModelIndex &index, const QString &newName)
{
    InventoryNode *editNode = static_cast<InventoryNode *>(index.internalPointer());
    InventoryItem &editItem = editNode->inventoryItem;

    editItem.itemName = newName;

    // TODO: convert InventoryItem to CategoryItem via a method
    m_dbman->updateCategory(CategoryItem::CategoryItemBuilder()
                            .setId  (editItem.itemId)
                            .setName(editItem.itemName)
                            .build());

    emit dataChanged(index, index);
}

void InventoryModel::deleteCategory(const QModelIndex &index)
{
    InventoryNode *delNode = static_cast<InventoryNode *>(index.internalPointer());

    m_dbman->deleteCategory(CategoryItem::CategoryItemBuilder()
                            .setId(delNode->inventoryItem.itemId)
                            .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
    delNode->parent->children.removeAt(index.row());
    endRemoveRows();
}

QModelIndex InventoryModel::addGroup(const QModelIndex &pindex, const QString &grpName)
{
    InventoryNode *pnode = static_cast<InventoryNode *>(pindex.internalPointer());

    qint32 categoryId = pnode->inventoryItem.itemId;

    qint32 newId = m_dbman->insertGroup(categoryId, grpName);

    auto row_iterator = std::find_if(pnode->children.begin(),
                                     pnode->children.end(),
                                     [&grpName](const InventoryNode &it) {
                                         return QString::localeAwareCompare(it.inventoryItem.itemName, grpName) > 0;
                                     });

    qint32 row = std::distance(pnode->children.begin(), row_iterator);

    GroupItem::GroupItemBuilder b;
    b.setId(newId);
    b.setName(grpName);

    beginInsertRows(pindex, row, row);
    pnode->children.insert(row, std::move(makeGroupNode(b.build(), pnode)));
    endInsertRows();

    return index(row, 0, pindex);
}

void InventoryModel::editGroup(const QModelIndex &index, const QString &newName)
{
    // TODO: сортировака при добавлении группы
    InventoryItem &editItem = static_cast<InventoryNode *>(index.internalPointer())->inventoryItem;
    editItem.itemName = newName;

    m_dbman->updateGroup(GroupItem::GroupItemBuilder()
                         .setId  (editItem.itemId)
                         .setName(editItem.itemName)
                         .build());

    emit dataChanged(index, index);
}

void InventoryModel::deleteGroup(const QModelIndex &index)
{
    InventoryNode *delNode = static_cast<InventoryNode *>(index.internalPointer());
    InventoryNode *parentNode = delNode->parent;

    m_dbman->deleteGroup(GroupItem::GroupItemBuilder()
                         .setId(delNode->inventoryItem.itemId)
                         .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
    parentNode->children.removeAt(index.row());
    endRemoveRows();
}

QModelIndex InventoryModel::addInventory(const QModelIndex &pindex, const ProductRelation &relation, const ProductItem &item)
{
    Q_UNUSED(pindex)
    InventoryNode *destNode = findDestinationNode(relation);
//    InventoryNode *pnode = static_cast<InventoryNode *>(pindex.internalPointer());

    QString tmpstr = item.itemName;

    auto row_iterator = std::find_if(destNode->children.begin(),
                                     destNode->children.end(),
                                     [&tmpstr](const InventoryNode &it) {
                                         return QString::localeAwareCompare(it.inventoryItem.itemName, tmpstr) > 0;
                                     });

    qint32 row = std::distance(destNode->children.begin(), row_iterator);

    qint32 newId = m_dbman->insertProduct(relation, item);

    QModelIndex destIndex = createIndex(findRow(destNode), 0, destNode);
    beginInsertRows(destIndex, row, row);
    destNode->children.insert(row, makeProductNode(ProductItem::ProductItemBuilder(item)
                                                   .setId(newId)
                                                   .build(),
                                                   destNode));
    endInsertRows();

    return index(row, 0, destIndex);
}

QModelIndex InventoryModel::editInventory(const QModelIndex &index, const ProductRelation &relation, const ProductItem &item)
{
    m_dbman->updateProduct(relation, item);

    InventoryNode *sourceNode = static_cast<InventoryNode *>(index.parent().internalPointer());
    InventoryNode *destNode = findDestinationNode(relation);

    beginRemoveRows(index.parent(), index.row(), index.row());
    sourceNode->children.removeAt(index.row());
    endRemoveRows();

    QString tmpstr = item.itemName;
    auto row_iterator = std::find_if(destNode->children.begin(),
                                     destNode->children.end(),
                                     [&tmpstr](const InventoryNode &it) {
                                         return QString::localeAwareCompare(it.inventoryItem.itemName, tmpstr) > 0;
                                     });
    qint32 row = std::distance(destNode->children.begin(), row_iterator);

    QModelIndex destIndex = createIndex(findRow(destNode), 0, destNode);
    beginInsertRows(destIndex, row, row);
    destNode->children.insert(row, makeProductNode(item, destNode));
    endInsertRows();

    return this->index(row, 0, destIndex);
}

void InventoryModel::deleteInventory(const QModelIndex &index)
{
    InventoryNode *delNode = static_cast<InventoryNode *>(index.internalPointer());
    InventoryNode *parentNode = delNode->parent;

    m_dbman->deleteProduct(ProductItem::ProductItemBuilder()
                           .setId(delNode->inventoryItem.itemId)
                           .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
    parentNode->children.removeAt(index.row());
    endRemoveRows();
}

ProductItem InventoryModel::getProductItemByIndex(const QModelIndex &index)
{
    InventoryNode *tmpnode = static_cast<InventoryNode *>(index.internalPointer());
    return (ProductItem::ProductItemBuilder()
            .setId      (tmpnode->inventoryItem.itemId)
            .setName    (tmpnode->inventoryItem.itemName)
            .setFullname(tmpnode->inventoryItem.itemFullname)
            .setSerialn (tmpnode->inventoryItem.itemSerialn)
            .setUnit    (tmpnode->inventoryItem.itemUnit)
            .setMiscTag (tmpnode->inventoryItem.itemMiscTag)
            .build());
}

InventoryItem InventoryModel::getInventoryItemByIndex(const QModelIndex &index)
{
    InventoryNode *tmpnode = static_cast<InventoryNode *>(index.internalPointer());
    return (tmpnode->inventoryItem);
}

InventoryModel::InventoryNode *InventoryModel::findDestinationNode(const ProductRelation &relation)
{
    decltype(m_nodes.begin()) categoryIter;
    for (auto &it : m_nodes) {
        categoryIter = std::find_if(it.children.begin(),
                                    it.children.end(),
                                    [&relation](const InventoryNode &node) -> bool {
                                        return node.inventoryItem.itemId == relation.parentCategory;
                                    });
        if (categoryIter != it.children.end()) {
            break;
        }
    }

    auto groupIter = std::find_if(categoryIter->children.begin(),
                                  categoryIter->children.end(),
                                  [&relation](const InventoryNode &node) -> bool {
                                      return node.inventoryItem.itemId == relation.parentGroup;
                                  });

    return &(*groupIter);
}
