#include "inventorytreemodel.h"

InventoryTreeModel::InventoryTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

InventoryTreeModel::~InventoryTreeModel()
{
}

QVariant InventoryTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList headers = {"Категория", "Код", "Наименование", "Ед. измерения", "Серийный №", "Полное наименование"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers.at(section);
    }
    return QVariant();
}

QModelIndex InventoryTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    InventoryTreeNode *parentNode;

    if (!parent.isValid()) {
        parentNode = rootNode;
    } else {
        parentNode = static_cast<InventoryTreeNode *>(parent.internalPointer());
    }

    InventoryTreeNode *childNode = parentNode->child(row);
    if (childNode) {
        return createIndex(row, column, childNode);
    } else {
        return QModelIndex();
    }
}

QModelIndex InventoryTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    InventoryTreeNode *childNode = static_cast<InventoryTreeNode*>(index.internalPointer());
    InventoryTreeNode *parentNode = childNode->parentNode();

    if (parentNode == rootNode) {
        return QModelIndex();
    }

    return createIndex(parentNode->row(), 0, parentNode);
}

int InventoryTreeModel::rowCount(const QModelIndex &parent) const
{
    InventoryTreeNode *parentNode;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid()) {
        parentNode = rootNode;
    } else {
        parentNode = static_cast<InventoryTreeNode*>(parent.internalPointer());
    }
    return parentNode->childCount();
}

int InventoryTreeModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;
    return InventoryTreeNode::ColumnCount;
}

QVariant InventoryTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    InventoryTreeNode *node = static_cast<InventoryTreeNode*>(index.internalPointer());

    return node->m_nodeData.itemName;

    return QVariant();
}

void InventoryTreeModel::initModel()
{
//    for (int i=0; i<10; ++i) {
//        rootNode->appendChild(new InventoryTreeNode(InventoryItem(), rootNode));
//    }
}
