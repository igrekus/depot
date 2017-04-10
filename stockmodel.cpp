#include "stockmodel.h"

struct StockModel::StockNode
{
    StockNode():
        stockItem(),
        children(),
        parent(nullptr),
        mapped(false),
        m_isExpanded(false)
    {}

    StockNode(const StockItem &stockItem, StockNode *parent = nullptr):
        stockItem(stockItem),
        children(),
        parent(parent),
        mapped(false),
        m_isExpanded(false)
    {}

    bool operator==(const StockNode &a) const
    {
        bool r = this->stockItem == a.stockItem;
        Q_ASSERT(!r || this->parent == a.parent);
        Q_ASSERT(!r || this->mapped == a.mapped);
        Q_ASSERT(!r || this->children == a.children);
        return r;
    }

    friend QDebug operator<<(QDebug dbg, const StockNode &right) {
        dbg.nospace() << "StockNode("
                      << "data:"    << right.stockItem
                      << " child:"  << right.children
                      << " parent:" << right.parent
                      << ")";
        return dbg.maybeSpace();
    }
    StockItem stockItem;                      // данные узла
    StockModel::StockNodeList children;       // массив ссылок на дочерние узлы
    StockNode *parent = nullptr;              // ссылка на родительский узел
    bool mapped = false;                      // проводился ли поиск дочерних узлов?
    bool m_isExpanded = false;                // раскрыт ли узел в view?

    StockModel::StockNodeList siblings(){return children;}
};

StockModel::StockModel(DataBaseManager *dbman, DictModel *dictModel, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_dbman = dbman;
    m_dictModel = dictModel;
}

StockModel::~StockModel()
{

}

StockModel::StockNode StockModel::makeCategoryNode(const CategoryItem &item)
{
    StockItem::StockItemBuilder b;
    b.setId       (item.itemId);
    b.setName     (item.itemName);
    b.setType     (StockItem::ItemCategory);
    b.setLevel    (StockItem::LevelRoot);
//    b.setAmount   (DATA_INVALID);
//    b.setSerialn  (QString());
//    b.setProject  (QString());
//    b.setLocation (QString());
    return StockModel::StockNode(b.build());
}

StockModel::StockNode StockModel::makeGroupNode(const GroupItem &item, StockNode *parent)
{
    StockItem::StockItemBuilder b;
    b.setId       (item.itemId);
    b.setName     (item.itemName);
    b.setType     (StockItem::ItemGroup);
    b.setLevel    (StockItem::Level_1);
//    b.setAmount   (DATA_INVALID);
//    b.setSerialn  (QString());
//    b.setProject  (QString());
//    b.setLocation (QString());
    return StockModel::StockNode(b.build(), parent);
}

StockModel::StockNode StockModel::makeStockNode(const StockItem &item, StockModel::StockNode *parent)
{
    return StockModel::StockNode(item, parent);
}

void StockModel::initModel()
{
    // TODO: формировать уровни одновременно - надо ли?
    buildCategoryLevel();
    buildGroupLevel();
    buildStockLevel();
}

void StockModel::buildCategoryLevel()
{
    qDebug() << "building category level";

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

void StockModel::buildGroupLevel()
{
    qDebug() << "building group level";
// TODO: begininsertorws;
    for (StockNode &it : m_nodes) {
        GroupItem::GroupList list = m_dbman->getGroupList(it.stockItem.itemId);
        for (const GroupItem &jt : list) {
            it.children.append(std::move(makeGroupNode(jt, &it)));
        }
    }
}

void StockModel::buildStockLevel()
{
    qDebug() << "building product level";
// TODO: endinsertrows
    for (StockNode &it : m_nodes) {
        for (StockNode &jt : it.children) {
            StockItem::StockList list = m_dbman->getStockList(it.stockItem.itemId, jt.stockItem.itemId);
            for (const StockItem &kt : list) {
                jt.children.append(std::move(makeStockNode(kt, &jt)));
            }
        }
    }
}

QVariant StockModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList headers = {"№", "Наименование", "Остаток", "№ партии", "Тема", "Место хранения"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers[section];
    }
    return QVariant();
}

QModelIndex StockModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    if (!parent.isValid()) {
        Q_ASSERT(m_nodes.size() > row);
        return createIndex(row, column, const_cast<StockNode*>(&m_nodes.at(row)));
    }

    StockNode *parentNode = static_cast<StockNode *>(parent.internalPointer());
    Q_ASSERT(parentNode!=nullptr);
//    Q_ASSERT(parentNode->mapped);
    Q_ASSERT(parentNode->children.size() > row);
    return createIndex(row, column, &parentNode->children[row]);
}

QModelIndex StockModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    StockNode *currentNode = static_cast<StockNode *>(index.internalPointer());
    Q_ASSERT(currentNode != nullptr);

    StockNode* parentNode = currentNode->parent;
    if (parentNode != nullptr) {
        return createIndex(findRow(parentNode), RamificationColumn, parentNode);
    }
    else {
        return QModelIndex();
    }
}

int StockModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_nodes.size();
    }
    const StockNode* parentNode = static_cast<const StockNode*>(parent.internalPointer());
    Q_ASSERT(parentNode != nullptr);

    return parentNode->children.size();
}

int StockModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant StockModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const StockNode *tmpnode = static_cast<StockNode *>(index.internalPointer());
    const StockItem tmpitem = tmpnode->stockItem;

//    Q_ASSERT(stockNode != nullptr);

    switch (role) {
    case Qt::DisplayRole: {
        switch (index.column()) {
        case NumberColumn: {
            if (tmpitem.itemType == StockItem::ItemStock) {
                return tmpitem.itemId;
            } else {
                return tmpitem.itemName;
            }
            break;
        }
        case NameColumn: {
            if (tmpitem.itemType == StockItem::ItemStock) {
                return tmpitem.itemName;
            } else {
                return QVariant();
            }
            break;
        }
        case AmountColumn: {
            if (tmpitem.itemType == StockItem::ItemStock) {
                return tmpitem.itemAmount;
            } else {
                return QVariant();
            }
            break;
        }
        case SerialnColumn: {
            return tmpitem.itemSerialn;
            break;
        }
        case ProjectColumn: {
            return tmpitem.itemProject;
            break;
        }
        case LocationColumn: {
            return tmpitem.itemLocation;
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
        case NumberColumn:
        case NameColumn:
        case AmountColumn:
        case SerialnColumn:
        case ProjectColumn:
        case LocationColumn: {
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
        case NumberColumn:
        case NameColumn:
        case AmountColumn:
        case SerialnColumn:
        case ProjectColumn:
        case LocationColumn: {
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
        case NumberColumn:
        case NameColumn:
        case AmountColumn:
        case SerialnColumn:
        case ProjectColumn:
        case LocationColumn: {
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
        case NumberColumn:
        case NameColumn:
        case AmountColumn:
        case SerialnColumn:
        case ProjectColumn:
        case LocationColumn: {
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

int StockModel::findRow(const StockNode *stockNode) const
{
    Q_ASSERT(stockNode != nullptr);
    const StockNodeList &searchList = stockNode->parent != nullptr ? stockNode->parent->children : m_nodes;
    return searchList.indexOf(*stockNode);
//    StockNodeList::const_iterator position = std::find(searchList.begin(), searchList.end(), *stockNode);
//    Q_ASSERT(position != searchList.end());
//    return std::distance(searchList.begin(), position);
}

QModelIndex StockModel::addCategory(const QString &catName)
{
    auto row_iterator = std::find_if(m_nodes.begin(), m_nodes.end(), [&catName](const StockNode &it){
        return it.stockItem.itemName > catName;
    });

    qint32 row = std::distance(std::begin(m_nodes), row_iterator);

    qint32 newId = m_dbman->insertCategory(catName);

    beginInsertRows(QModelIndex(), row, row + 1);
    m_nodes.insert(row, std::move(makeCategoryNode(CategoryItem::CategoryItemBuilder()
                                                   .setId(newId)
                                                   .setName(catName)
                                                   .build())));
    endInsertRows();

    return index(row, 0, QModelIndex());
}

void StockModel::editCategory(const QModelIndex &index, const QString &newName)
{
    StockItem &editItem = static_cast<StockNode *>(index.internalPointer())->stockItem;
    editItem.itemName = newName;
    m_dbman->updateCategory(CategoryItem::CategoryItemBuilder()
                            .setId  (editItem.itemId)
                            .setName(editItem.itemName)
                            .build());
    emit dataChanged(index, index);
}

void StockModel::deleteCategory(const QModelIndex &index)
{
    StockNode *delNode = static_cast<StockNode *>(index.internalPointer());
    qint32 row = findRow(delNode);

    m_dbman->deleteCategory(CategoryItem::CategoryItemBuilder()
                            .setId  (delNode->stockItem.itemId)
                            .setName(delNode->stockItem.itemName)
                            .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
    m_nodes.removeAt(row);
    endRemoveRows();
}

QModelIndex StockModel::addGroup(const QModelIndex &pindex, const QString &grpName)
{
    // TODO: соединить метод с методом добавки категории?
    StockNode *pnode = static_cast<StockNode *>(pindex.internalPointer());

    auto row_iterator = std::find_if(pnode->children.begin(), pnode->children.end(),
                                     [&grpName](const StockNode &it){return it.stockItem.itemName > grpName;});

    qint32 row = std::distance(pnode->children.begin(), row_iterator);

    qint32 newId = m_dbman->insertGroup(grpName);

    qDebug() << "ins data:" << pnode->stockItem.itemName << grpName << row << pnode->children.size();

    pnode->children.reserve(1);
    beginInsertRows(pindex, pnode->children.size(), pnode->children.size() + 1);
    pnode->children.insert(row, std::move(makeGroupNode(GroupItem::GroupItemBuilder()
                                                        .setId  (newId)
                                                        .setName(grpName)
                                                        .build(), pnode)));
    endInsertRows();

    qDebug() << pnode->children.size();
    return index(row, 0, pindex);
}

void StockModel::editGroup(const QModelIndex &index, const QString &newName)
{
    StockItem &editItem = static_cast<StockNode *>(index.internalPointer())->stockItem;
    editItem.itemName = newName;

    m_dbman->updateGroup(GroupItem::GroupItemBuilder()
                         .setId  (editItem.itemId)
                         .setName(editItem.itemName)
                         .build());

    emit dataChanged(index, index);
}

void StockModel::deleteGroup(const QModelIndex &index)
{
    // TODO: fix
    StockNode *delNode = static_cast<StockNode *>(index.internalPointer());
    qint32 row = findRow(delNode);

    m_dbman->deleteGroup(GroupItem::GroupItemBuilder()
                         .setId  (delNode->stockItem.itemId)
                         .setName(delNode->stockItem.itemName)
                         .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
    delNode->siblings().removeAt(row);
    endRemoveRows();
}

void StockModel::debugInfo(const QModelIndex &index)
{
    StockNode *node = static_cast<StockNode *>(index.internalPointer());
    qint32 row = findRow(node);

    qDebug() << "node: row:" << row << "parent:" << node->parent
             << "child:" << node->children.size()
             << "data:" << node->stockItem;
}
