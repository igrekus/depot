#ifndef INVENTORYTREEMODEL_H
#define INVENTORYTREEMODEL_H

#include <QAbstractItemModel>
#include <constants.h>
#include <inventoryitem.h>

class InventoryTreeNode
{
public:
    enum Columns {
        RamificationColumn = 0,
        CategoryColumn= RamificationColumn,
        CodeColumn,
        NameColumn,
        UnitColumn,
        SerialnColumn,
        FullnameColumn,
        ColumnCount
    };

    explicit InventoryTreeNode(const InventoryItem &data, InventoryTreeNode *parentNode = nullptr) {
        m_parentNode = parentNode;
        m_nodeData = data;
    }

    ~InventoryTreeNode() {
        qDeleteAll(m_childNodes);
    }

    void appendChild(InventoryTreeNode *child) {
        m_childNodes.append(child);
    }

    InventoryTreeNode *child(qint32 row) {
        return m_childNodes.value(row);
    }

    qint32 childCount() const {
        return m_childNodes.count();
    }

    qint32 columnCount() const {
        return ColumnCount;
    }

    qint32 row() const {
        if (m_parentNode)
            return m_parentNode->m_childNodes.indexOf(const_cast<InventoryTreeNode*>(this));
    }

    InventoryTreeNode *parentNode()
    {
        return m_parentNode;
    }

    InventoryItem data() const {
        return m_nodeData;
    }

    QVector<InventoryTreeNode*> m_childNodes;
    InventoryTreeNode *m_parentNode;
    InventoryItem m_nodeData;
};

class InventoryTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit InventoryTreeModel(QObject *parent = nullptr);
    ~InventoryTreeModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void initModel();

    InventoryTreeNode *rootNode;
private:
};

#endif // INVENTORYTREEMODEL_H
