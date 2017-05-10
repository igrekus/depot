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

InventoryModel::InventoryNode InventoryModel::makeCategoryNode(const CategoryItem &item)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId   (item.itemId)
                                         .setName (item.itemName)
                                         .setType (Constants::ItemCategory)
                                         .setLevel(Constants::LevelRoot)
                                         .build(),
                                         nullptr);
}

InventoryModel::InventoryNode InventoryModel::makeGroupNode(const GroupItem &item, InventoryNode *parent)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId   (item.itemId)
                                         .setName (item.itemName)
                                         .setType (Constants::ItemGroup)
                                         .setLevel(Constants::Level_1)
                                         .build(),
                                         parent);
}

InventoryModel::InventoryNode InventoryModel::makeProductNode(const ProductItem &item, InventoryModel::InventoryNode *parent)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId(item.itemId)
                                         .setName(item.itemName)
                                         .setType (Constants::ItemItem)
                                         .setLevel(Constants::Level_2)
                                         .setFullname(item.itemFullname)
                                         .setSerialn(item.itemSerialn)
                                         .setUnit(item.itemUnit)
                                         .setMiscTag(item.itemMiscTag)
                                         .build(),
                                         parent);
}

void InventoryModel::buildCategoryLevel()
{
    qDebug() << "inventory: building category level";
    CategoryItem::CategoryList list = m_dbman->getCategoryList();
//    beginInsertRows(QModelIndex(), 0, list.size()-1);
    for (const CategoryItem &it : list) {
//        if (it.itemId == 1) {
//            continue;
//        }
        m_nodes.append(std::move(makeCategoryNode(it)));
    }
//    endInsertRows();
}

void InventoryModel::buildGroupLevel()
{
    qDebug() << "inventory: building group level";
// TODO: begininsertorws;
    for (InventoryNode &it : m_nodes) {
        GroupItem::GroupList list = m_dbman->getGroupList(it.inventoryItem.itemId);
        for (const GroupItem &jt : list) {
            it.children.append(std::move(makeGroupNode(jt, &it)));
        }
    }
}

void InventoryModel::buildProductLevel()
{
    qDebug() << "inventory: building product level";
// TODO: endinsertrows
    for (InventoryNode &it : m_nodes) {
        for (InventoryNode &jt : it.children) {
            ProductItem::ProductList list = m_dbman->getProductList(it.inventoryItem.itemId, jt.inventoryItem.itemId);
            for (const ProductItem &kt : list) {
                jt.children.append(std::move(makeProductNode(kt, &jt)));
            }
        }
    }
}

void InventoryModel::initModel()
{
    buildCategoryLevel();
    buildGroupLevel();
    buildProductLevel();
}

QVariant InventoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList headers = {"Категория", "Код", "Наименование", "Ед. измерения", "Серийный №", "Полное наименование"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers[section];
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
    const InventoryItem tmpitem = tmpnode->inventoryItem;

    Q_ASSERT(tmpnode != nullptr);

    switch (role) {
    case Qt::DisplayRole: {
        switch (index.column()) {
        case CategoryColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
//                return tmpitem.itemId;
                return QVariant();
            } else {
                return tmpitem.itemName;
            }
            break;
        }
        case CodeColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemId;
//                return m_dictModel->m_productListModel->getName(tmpitem.itemId);
            } else {
                return QVariant();
            }
            break;
        }
        case NameColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemName;
            } else {
                return QVariant();
            }
            break;
        }
        case UnitColumn: {
            return tmpitem.itemUnit;
            break;
        }
        case SerialnColumn: {
            return tmpitem.itemSerialn;
            break;
        }
        case FullnameColumn: {
            return tmpitem.itemFullname;
            break;
        }
        case ColumnCount:
        default:
            break;
        }
        break;
    }
    case Constants::RoleLevelId: {
        switch (index.column()) {
        case CategoryColumn:
        case CodeColumn:
        case NameColumn:
        case UnitColumn:
        case SerialnColumn:
        case FullnameColumn: {
            return tmpitem.itemLevel;
            break;
        }
        case ColumnCount:
        default:
            break;
        }
        break;
    }
    case Constants::RoleNodeType: {
        switch (index.column()) {
        case CategoryColumn:
        case CodeColumn:
        case NameColumn:
        case UnitColumn:
        case SerialnColumn:
        case FullnameColumn: {
            return tmpitem.itemType;
            break;
        }
        case ColumnCount:
        default:
            break;
        }
        break;
    }
    case Constants::RoleNodeId: {
        switch (index.column()) {
        case CategoryColumn:
        case CodeColumn:
        case NameColumn:
        case UnitColumn:
        case SerialnColumn:
        case FullnameColumn: {
            return tmpitem.itemId;
            break;
        }
        case ColumnCount:
        default:
            break;
        }
        break;
    }
    case Constants::RoleNodeHasChildren: {
        switch (index.column()) {
        case CategoryColumn:
        case CodeColumn:
        case NameColumn:
        case UnitColumn:
        case SerialnColumn:
        case FullnameColumn: {
            return !tmpnode->children.isEmpty();
            break;
        }
        case ColumnCount:
        default:
            break;
        }
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

QModelIndex InventoryModel::addCategory(const QString &catName)
{
    qint32 newId = m_dbman->insertCategory(catName);
    m_dictModel->m_categoryListModel->addItem(newId, catName);

    auto row_iterator = std::find_if(m_nodes.begin(), m_nodes.end(), [&catName](const InventoryNode &it){
        return it.inventoryItem.itemName > catName;
    });
    qint32 row = std::distance(m_nodes.begin(), row_iterator);
    beginInsertRows(QModelIndex(), row, row);
    InventoryNode tmpnode = makeCategoryNode(CategoryItem::CategoryItemBuilder()
                                             .setId(newId)
                                             .setName(catName)
                                             .build());
    m_nodes.insert(row, tmpnode);
    endInsertRows();

    return index(row, 0, QModelIndex());
}

QModelIndex InventoryModel::editCategory(const QModelIndex &index, const QString &newName)
{
    // TODO: !!!rewrite to be like editInventory!!!
    InventoryNode *editNode = static_cast<InventoryNode *>(index.internalPointer());
    InventoryItem &editItem = editNode->inventoryItem;
    editItem.itemName = newName;

    m_dbman->updateCategory(CategoryItem::CategoryItemBuilder()
                            .setId  (editItem.itemId)
                            .setName(editItem.itemName)
                            .build());

    emit dataChanged(index, index);

    //TODO: сортировка при переименовании
//    auto dest_row_iterator = std::find_if(m_nodes.begin(), m_nodes.end(), [&newName](const InventoryNode &it){
//        return it.inventoryItem.itemName > newName;
//    });
//    qint32 dest_row = std::distance(m_nodes.begin(), dest_row_iterator);
//    qint32 row = index.row();

//    beginMoveRows(index.parent(), row, row, index.parent(), dest_row);
//    m_nodes.move(row, dest_row);
//    endMoveRows();

//    return this->index(dest_row, 0, index.parent());
    return this->index(index.row(), 0, index.parent());
}

void InventoryModel::deleteCategory(const QModelIndex &index)
{
    InventoryNode *delNode = static_cast<InventoryNode *>(index.internalPointer());

    m_dbman->deleteCategory(CategoryItem::CategoryItemBuilder()
                            .setId  (delNode->inventoryItem.itemId)
                            .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
    m_nodes.removeAt(index.row());
    endRemoveRows();
}

QModelIndex InventoryModel::addGroup(const QModelIndex &pindex, const QString &grpName)
{
    // TODO: соединить метод с методом добавки категории?
    InventoryNode *pnode = static_cast<InventoryNode *>(pindex.internalPointer());

    auto row_iterator = std::find_if(pnode->children.begin(), pnode->children.end(),
                                     [&grpName](const InventoryNode &it){return it.inventoryItem.itemName > grpName;});

    qint32 row = std::distance(pnode->children.begin(), row_iterator);

    GroupItem::GroupItemBuilder b;
    b.setName(grpName);
    b.setCategory(pnode->inventoryItem.itemId);

    qint32 newId = m_dbman->insertGroup(b.build());

    b.setId(newId);

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

QModelIndex InventoryModel::addInventory(const QModelIndex &pindex, const ProductItem &item)
{
    // TODO: соединить метод с методом добавки категории?
    InventoryNode *pnode = static_cast<InventoryNode *>(pindex.internalPointer());

    QString tmpstr = item.itemName;

    auto row_iterator = std::find_if(pnode->children.begin(), pnode->children.end(),
                                     [&tmpstr](const InventoryNode &it){return it.inventoryItem.itemName > tmpstr;});

    qint32 row = std::distance(pnode->children.begin(), row_iterator);

    qint32 newId = m_dbman->insertProduct(item);

//    beginInsertRows(pindex, row, row + 1);
    beginInsertRows(pindex, row, row);
    pnode->children.insert(row, std::move(makeProductNode(ProductItem::ProductItemBuilder(item)
                                                          .setId(newId)
                                                          .build(), pnode)));
    endInsertRows();

    return index(row, 0, pindex);
}

void InventoryModel::editInventory(const QModelIndex &index, const ProductItem &item)
{
    // TODO: сортировака при добавлении продукта
    InventoryNode *editNode = static_cast<InventoryNode *>(index.internalPointer());
    *editNode = std::move(makeProductNode(item, editNode->parent));

    m_dbman->updateProduct(item);

    emit dataChanged(index, index);
}

void InventoryModel::deleteInventory(const QModelIndex &index)
{
    InventoryNode *delNode = static_cast<InventoryNode *>(index.internalPointer());
    InventoryNode *parentNode = delNode->parent;

    m_dbman->deleteProduct(ProductItem::ProductItemBuilder()
                           .setId  (delNode->inventoryItem.itemId)
                           .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
//    delNode->siblings().removeAt(index.row());
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
            .setGroup   (tmpnode->parent->inventoryItem.itemId)          // TODO: FIX this shit
            .setCategory(tmpnode->parent->parent->inventoryItem.itemId)
            .build());
}

InventoryItem InventoryModel::getInventoryItemByIndex(const QModelIndex &index)
{
    InventoryNode *tmpnode = static_cast<InventoryNode *>(index.internalPointer());
    return (tmpnode->inventoryItem);
}
