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

    InventoryNode(const InventoryItem &invItem, InventoryNode *parent = nullptr):
        inventoryItem(invItem),
        children(),
        parent(parent),
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
                      << ")";
        return dbg.maybeSpace();
    }
    InventoryItem inventoryItem;                   // данные узла
    InventoryModel::InventoryNodeList children;    // массив ссылок на дочерние узлы
    InventoryNode *parent = nullptr;               // ссылка на родительский узел
    bool mapped = false;                           // проводился ли поиск дочерних узлов?
    bool m_isExpanded = false;                     // раскрыт ли узел в view?

    InventoryModel::InventoryNodeList *siblings(){return &parent->children;}
};

InventoryModel::InventoryModel(DataBaseManager *dbman, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_dbman = dbman;
}

InventoryModel::~InventoryModel()
{

}

InventoryModel::InventoryNode InventoryModel::makeCategoryNode(const CategoryItem &item)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId   (item.itemId)
                                         .setName (item.itemName)
                                         .setType (StockItem::ItemCategory)
                                         .setLevel(StockItem::LevelRoot)
                                         .build());
}

InventoryModel::InventoryNode InventoryModel::makeGroupNode(const GroupItem &item, InventoryNode *parent)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId   (item.itemId)
                                         .setName (item.itemName)
                                         .setType (StockItem::ItemGroup)
                                         .setLevel(StockItem::Level_1)
                                         .build(),
                                         parent);
}

InventoryModel::InventoryNode InventoryModel::makeProductNode(const ProductItem &item, InventoryModel::InventoryNode *parent)
{
    return InventoryModel::InventoryNode(InventoryItem::InventoryItemBuilder()
                                         .setId(item.itemId)
                                         .setName(item.itemName)
                                         .setType (StockItem::ItemItem)
                                         .setLevel(StockItem::Level_2)
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
    beginInsertRows(QModelIndex(), 0, list.size()-1);
    for (const CategoryItem &it : list) {
        if (it.itemId == 1) {
            continue;
        }
        m_nodes.append(std::move(makeCategoryNode(it)));
    }
    endInsertRows();
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
            if (tmpitem.itemType == InventoryItem::ItemItem) {
                return tmpitem.itemId;
            } else {
                return tmpitem.itemName;
            }
            break;
        }
        case CodeColumn: {
            if (tmpitem.itemType == InventoryItem::ItemItem) {
                return tmpitem.itemId;
            } else {
                return QVariant();
            }
            break;
        }
        case NameColumn: {
            if (tmpitem.itemType == InventoryItem::ItemItem) {
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
    case ROLE_LEVEL_ID: {
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
    case ROLE_NODE_TYPE: {
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
    case ROLE_NODE_ID: {
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
    case ROLE_NODE_HAS_CHILDREN: {
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
    const InventoryNodeList &searchList = invNode->parent != nullptr ? invNode->parent->children : m_nodes;
    return searchList.indexOf(*invNode);
//    StockNodeList::const_iterator position = std::find(searchList.begin(), searchList.end(), *stockNode);
//    Q_ASSERT(position != searchList.end());
//    return std::distance(searchList.begin(), position);
}
