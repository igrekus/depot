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

StockModel::StockNode StockModel:: makeClassNode(const ClassItem &item)
{
    return StockModel::StockNode(StockItem::StockItemBuilder()
                                 .setId   (item.itemId)
                                 .setName (item.itemName)
                                 .setType (Constants::ItemClass)
                                 .setLevel(Constants::LevelRoot)
                                 .build());
}

StockModel::StockNode StockModel::makeCategoryNode(const CategoryItem &item, StockNode *parent)
{
    return StockModel::StockNode(StockItem::StockItemBuilder()
                                 .setId   (item.itemId)
                                 .setName (item.itemName)
                                 .setType (Constants::ItemCategory)
                                 .setLevel(Constants::Level_1)
                                 .build(),
                                 parent);
}

StockModel::StockNode StockModel::makeGroupNode(const GroupItem &item, StockNode *parent)
{
    return StockModel::StockNode(StockItem::StockItemBuilder()
                                 .setId   (item.itemId)
                                 .setName (item.itemName)
                                 .setType (Constants::ItemGroup)
                                 .setLevel(Constants::Level_2)
                                 .build(),
                                 parent);
}

StockModel::StockNode StockModel::makeStockNode(const StockItem &item, StockModel::StockNode *parent)
{
    return StockModel::StockNode(item, parent);
}

void StockModel::fillClassNode(const QModelIndex &index, StockNode &node)
{
    CategoryItem::CategoryList list = m_dbman->getCategoryList(node.stockItem.itemId);
    beginInsertRows(index, 0, list.size()-1);
    for (const CategoryItem &it : list) {
        node.children.append(std::move(makeCategoryNode(it, &node)));
    }
    endInsertRows();
}

void StockModel::fillCategoryNode(const QModelIndex &index, StockNode &node)
{
    GroupItem::GroupList list = m_dbman->getGroupList(node.stockItem.itemId);
    beginInsertRows(index, 0, list.size()-1);
    for (const GroupItem &it : list) {
        node.children.append(std::move(makeGroupNode(it, &node)));
    }
    endInsertRows();
}

void StockModel::fillGroupNode(const QModelIndex &index, StockNode &node)
{
    // TODO: finish
    StockItem::StockList list = m_dbman->getStockList(node.stockItem.itemId);
    beginInsertRows(index, 0, list.size()-1);
    for (const StockItem &it : list) {
        node.children.append(std::move(makeStockNode(it, &node)));
    }
    endInsertRows();
}

void StockModel::buildClassLevel()
{
    qDebug() << "stock: building class level (0)";
    ClassItem::ClassList list = m_dbman->getClassList();
    beginInsertRows(QModelIndex(), 0, list.size()-1);
    for (const ClassItem &it : list) {
        m_nodes.append(std::move(makeClassNode(it)));
    }
    endInsertRows();
}

void StockModel::buildCategoryLevel()
{
    qDebug() << "stock: building category level (1)";
    qint32 i=0;
    for (StockNode &classNode : m_nodes) {
        fillClassNode(createIndex(i, 0, &classNode), classNode);
        ++i;
    }
}

void StockModel::buildGroupLevel()
{
    qDebug() << "stock: building group level (2)";
    for (StockNode &classNode : m_nodes) {
        qint32 i=0;
        for (StockNode &categoryNode : classNode.children) {
            fillCategoryNode(createIndex(i, 0, &categoryNode), categoryNode);
            ++i;
        }
    }
}

void StockModel::buildStockLevel()
{
    qDebug() << "stock: building product level (2)";
    for (StockNode &classNode : m_nodes) {
        for (StockNode &categoryNode : classNode.children) {
            qint32 i=0;
            for (StockNode &groupNode : categoryNode.children) {
                fillGroupNode(createIndex(i, 0, &groupNode), groupNode);
                ++i;
            }
        }
    }
}

void StockModel::initModel()
{
    // TODO: формировать уровни одновременно - как?
    buildClassLevel();
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
    const QStringList headers = {"Каталог", "Наименование", "Единица", "Остаток", "№ партии", "Тема", "Место хранения", "Полное наименование"};
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
                return tmpitem.itemProduct;
            }
            return tmpitem.itemName;
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
        case AmountColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemAmount;
            }
            break;
        }
        case SerialnColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return tmpitem.itemSerialn;
            }
            break;
        }
        case ProjectColumn: {            
            if (tmpitem.itemType == Constants::ItemItem) {
                if (tmpitem.itemProject == 0) {
                    return QVariant(QString("INVALID PROJECT"));
                }
                return m_dictModel->m_projectListModel->getData(tmpitem.itemProject);
            }
            break;
        }
        case LocationColumn: {
            if (tmpitem.itemType == Constants::ItemItem) {
                return m_dictModel->m_locationListModel->getData(tmpitem.itemLocation);
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
    }
    case Constants::RoleNodeType: {
        return tmpitem.itemType;
        break;
    }
    case Constants::RoleNodeId: {
        return tmpitem.itemId;
    }
    case Constants::RoleNodeHasChildren: {
        return !tmpnode->children.isEmpty();
        break;
    }
    case Constants::RoleProjectId: {
        return tmpitem.itemProject;
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

QModelIndex StockModel::addStock(const StockItem &item)
{
    // REFACTOR
    ProductRelation p = m_dbman->getProductParents(item.itemProduct);

    auto catIter = std::find_if(m_nodes.begin(), m_nodes.end(),
                                [&p](const StockNode &it) -> bool {
        return (it.stockItem.itemId == p.parentCategory);
    });

    auto grpIter = std::find_if(catIter->children.begin(), catIter->children.end(),
                                [&p](const StockNode &it) -> bool {
        return (it.stockItem.itemId == p.parentGroup);
    });
    qint32 grprow = std::distance(catIter->children.begin(), grpIter);

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

    qDebug() << "stock debug:\nrow:" << row << "parent:" << node->parent
             << "child:" << node->children.size()
             << "data:" << node->stockItem;
}
