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

    StockItem stockItem;                      // данные узла
    StockModel::StockNodeList children;       // массив ссылок на дочерние узлы
    StockNode *parent = nullptr;              // ссылка на родительский узел
    bool mapped = false;                      // проводился ли поиск дочерних узлов?
    bool m_isExpanded = false;                // раскрыт ли узел в view?
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

StockModel::StockNode StockModel::nodeFactoryCategory(const CategoryItem &item)
{
    StockItem::StockItemBuilder b;
    b.setId       (item.itemId);
    b.setName     (item.itemName);
    b.setType     (StockItem::ItemCategory);
    b.setLevel    (StockItem::LevelRoot);
    b.setAmount   (DATA_INVALID);
    b.setSerialn  (QString());
    b.setProject  (QString());
    b.setLocation (QString());
    return StockModel::StockNode(b.build());
}

StockModel::StockNode StockModel::nodeFactoryGroup(const GroupItem &item, StockNode *parent)
{
    StockItem::StockItemBuilder b;
    b.setId       (item.itemId);
    b.setName     (item.itemName);
    b.setType     (StockItem::ItemGroup);
    b.setLevel    (StockItem::Level_1);
    b.setAmount   (DATA_INVALID);
    b.setSerialn  (QString());
    b.setProject  (QString());
    b.setLocation (QString());
    return StockModel::StockNode(b.build(), parent);
}

StockModel::StockNode StockModel::nodeFactoryStock(const StockItem &item, StockNode *parent)
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
    for (const CategoryItem &it : list) {
        if (it.itemId == 1) {
            continue;
        }
        _nodes.append(std::move(nodeFactoryCategory(it)));
    }
}

void StockModel::buildGroupLevel()
{
    qDebug() << "building group level";

    for (StockNode &it : _nodes) {
        GroupItem::GroupList list = m_dbman->getGroupList(it.stockItem.itemId);
        for (const GroupItem &jt : list) {
            it.children.append(std::move(nodeFactoryGroup(jt, &it)));
        }
    }
}

void StockModel::buildStockLevel()
{
    qDebug() << "building product level";

    for (StockNode &it : _nodes) {
        for (StockNode &jt : it.children) {
            StockItem::StockList list = m_dbman->getStockList(it.stockItem.itemId, jt.stockItem.itemId);
            for (const StockItem &kt : list) {
                jt.children.append(std::move(nodeFactoryStock(kt, &jt)));
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
        Q_ASSERT(_nodes.size() > row);
        return createIndex(row, column, const_cast<StockNode*>(&_nodes[row]));
    }

    StockNode *parentNode = static_cast<StockNode *>(parent.internalPointer());
    Q_ASSERT(parentNode!= 0);
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
    Q_ASSERT(currentNode != 0);

    StockNode* parentNode = currentNode->parent;
    if (parentNode != 0) {
        return createIndex(findRow(parentNode), RamificationColumn, parentNode);
    }
    else {
        return QModelIndex();
    }
}

int StockModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return _nodes.size();
    }
    const StockNode* parentNode = static_cast<const StockNode*>(parent.internalPointer());
    Q_ASSERT(parentNode != 0);

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

    const StockNode *stockNode = static_cast<StockNode *>(index.internalPointer());
    const StockItem tmpitem = stockNode->stockItem;

    Q_ASSERT(stockNode != 0);

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
    default:
        break;
    }
    return QVariant();
}

int StockModel::findRow(const StockNode *stockNode) const
{
    Q_ASSERT(stockNode != 0);
    const StockNodeList &parentNodeChildren = stockNode->parent != 0 ? stockNode->parent->children: _nodes;
    StockNodeList::const_iterator position = qFind(parentNodeChildren, *stockNode);
    Q_ASSERT(position != parentNodeChildren.end());
    return std::distance(parentNodeChildren.begin(), position);
}

void StockModel::addCategory(const QString &catName)
{
    beginInsertRows(QModelIndex(), _nodes.size(), _nodes.size() + 1);

    CategoryItem::CategoryItemBuilder b;
    b.setName(catName);

    _nodes.append(std::move(nodeFactoryCategory(b.build())));

    endInsertRows();
}
