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

    StockModel::StockNodeList *siblings(){return &parent->children;}
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
    return StockModel::StockNode(StockItem::StockItemBuilder()
                                 .setId   (item.itemId)
                                 .setName (item.itemName)
                                 .setType (Constants::ItemCategory)
                                 .setLevel(Constants::LevelRoot)
                                 .build());
}

StockModel::StockNode StockModel::makeGroupNode(const GroupItem &item, StockNode *parent)
{
    return StockModel::StockNode(StockItem::StockItemBuilder()
                                 .setId   (item.itemId)
                                 .setName (item.itemName)
                                 .setType (Constants::ItemGroup)
                                 .setLevel(Constants::Level_1)
                                 .build(),
                                 parent);
}

StockModel::StockNode StockModel::makeStockNode(const StockItem &item, StockModel::StockNode *parent)
{
    return StockModel::StockNode(item, parent);
}

void StockModel::buildCategoryLevel()
{
    qDebug() << "stock: building category level";
    CategoryItem::CategoryList list = m_dbman->getCategoryList();
    beginInsertRows(QModelIndex(), 0, list.size()-1);
    for (const CategoryItem &it : list) {
//        if (it.itemId == 1) {
//            continue;
//        }
        m_nodes.append(std::move(makeCategoryNode(it)));
    }
    endInsertRows();
}

void StockModel::buildGroupLevel()
{
    qDebug() << "stock: building group level";
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
    qDebug() << "stock: building product level";
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

void StockModel::initModel()
{
    // TODO: формировать уровни одновременно - надо ли?
    buildCategoryLevel();
    buildGroupLevel();
    buildStockLevel();
}

void StockModel::clearNodeList(StockModel::StockNodeList &list)
{
    Q_ASSERT(!list.isEmpty());
    for (StockNode &it : list) {
        if (!it.children.isEmpty()) {
            clearNodeList(it.children);
        }
    }
    // если есть родитель у списка (т.е. не в корне):
    if (list.at(0).parent != nullptr) {
        // получаем указатель на ноду-родителя списка
        StockNode *parentStockNode = list.at(0).parent;
        // создаём QModelIndex ноды-родителя для сигнала
        QModelIndex pindex = createIndex(findRow(parentStockNode), 0, &parentStockNode);
        beginRemoveRows(pindex, 0, 0+list.count()-1);
        list.clear();
        endRemoveRows();
    } else {
        // чистим корень, пустой индекс
        beginRemoveRows(QModelIndex(), 0, 0+list.count()-1);
        list.clear();
        endRemoveRows();
    }
}

void StockModel::clear()
{
    beginResetModel();
    if (!m_nodes.isEmpty()) {
        clearNodeList(m_nodes);
    }
    endResetModel();
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
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemId;
            } else {
                return tmpitem.itemName;
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
        case AmountColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
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
            return m_dictModel->m_projectListModel->getData(tmpitem.itemProject);
            break;
        }
        case LocationColumn: {
            return m_dictModel->m_locationListModel->getData(tmpitem.itemLocation);
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
    case Constants::RoleNodeType: {
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
    case Constants::RoleNodeId: {
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
    case Constants::RoleNodeHasChildren: {
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

//    beginInsertRows(QModelIndex(), row, row + 1);
    beginInsertRows(QModelIndex(), row, row);
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

    pnode->children.reserve(1);
//    beginInsertRows(pindex, pnode->children.size(), pnode->children.size() + 1);
    beginInsertRows(pindex, pnode->children.size(), pnode->children.size());
    pnode->children.insert(row, std::move(makeGroupNode(GroupItem::GroupItemBuilder()
                                                        .setId  (newId)
                                                        .setName(grpName)
                                                        .build(), pnode)));
    endInsertRows();

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
    StockNode *delNode = static_cast<StockNode *>(index.internalPointer());
    qint32 row = findRow(delNode);

    m_dbman->deleteGroup(GroupItem::GroupItemBuilder()
                         .setId  (delNode->stockItem.itemId)
                         .setName(delNode->stockItem.itemName)
                         .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
//    delNode->parent->children.removeAt(row);
    delNode->siblings()->removeAt(row);
    endRemoveRows();
}

QModelIndex StockModel::addStock(const StockItem &item)
{
    QPair<qint32, qint32> grpcat = m_dbman->getProductParents(item.itemProductRef);

    auto catIter = std::find_if(m_nodes.begin(), m_nodes.end(),
                                [&grpcat](const StockNode &it) -> bool {
        return (it.stockItem.itemId == grpcat.second);
    });
//    qDebug() << "cat:" << catIter->stockItem.itemId << catIter->stockItem.itemName;

    auto grpIter = std::find_if(catIter->children.begin(), catIter->children.end(),
                                [&grpcat](const StockNode &it) -> bool {
        return (it.stockItem.itemId == grpcat.first);
    });
    qint32 grprow = std::distance(catIter->children.begin(), grpIter);
//    qDebug() << "grp:" << grpIter->stockItem.itemId << grpIter->stockItem.itemName;

    auto targetIter = std::find_if(grpIter->children.begin(), grpIter->children.end(),
                                   [&grpIter](const StockNode &it) -> bool {
            return it.stockItem.itemName > grpIter->stockItem.itemName;});
    qint32 targetRow = std::distance(grpIter->children.begin(), targetIter);

    qint32 newId = m_dbman->insertStock(item);

    StockNode *pnode = &catIter->children[grprow];

    QModelIndex pindex = createIndex(targetRow, RamificationColumn, pnode);

//    beginInsertRows(pindex, targetRow, targetRow + 1);
    beginInsertRows(pindex, targetRow, targetRow);
    pnode->children.insert(targetRow, std::move(makeStockNode(StockItem::StockItemBuilder(item)
                                                              .setId  (newId)
                                                              .build(),
                                                              pnode)));
    endInsertRows();

    return index (targetRow, RamificationColumn, pindex);
}

void StockModel::editStock(const QModelIndex &index, const StockItem &item)
{
    StockItem &editItem = static_cast<StockNode *>(index.internalPointer())->stockItem;

    editItem = item;

    m_dbman->updateStock(item);

    emit dataChanged(index, index);
}

void StockModel::deleteStock(const QModelIndex &index)
{
    StockNode *delNode = static_cast<StockNode *>(index.internalPointer());
    qint32 row = findRow(delNode);

    m_dbman->deleteStock(StockItem::StockItemBuilder()
                         .setId  (delNode->stockItem.itemId)
                         .build());

    beginRemoveRows(index.parent(), index.row(), index.row());
    delNode->siblings()->removeAt(row);
    endRemoveRows();
}

StockItem StockModel::getStockItemByIndex(const QModelIndex &index)
{
    return (static_cast<StockNode *>(index.internalPointer())->stockItem);
}

void StockModel::debugInfo(const QModelIndex &index)
{
    StockNode *node = static_cast<StockNode *>(index.internalPointer());
    qint32 row = findRow(node);

    qDebug() << "node: row:" << row << "parent:" << node->parent
             << "child:" << node->children.size()
             << "data:" << node->stockItem;
}
