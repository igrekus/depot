#ifndef INVENTORYMODEL_H
#define INVENTORYMODEL_H

#include <QAbstractItemModel>
#include <QDebug>

#include <categoryitem.h>
#include <groupitem.h>
#include <productitem.h>
#include <databasemanager.h>
#include <inventoryitem.h>

#define DATA_INVALID -1

#define ROLE_LEVEL_ID  (Qt::UserRole+1)
#define ROLE_NODE_TYPE (Qt::UserRole+2)
#define ROLE_NODE_ID   (Qt::UserRole+3)
#define ROLE_NODE_HAS_CHILDREN (Qt::UserRole+4)

class InventoryModel : public QAbstractItemModel
{
    Q_OBJECT

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
    Q_ENUM(Columns)

    // данные
    struct InventoryNode;
    typedef QList<InventoryNode> InventoryNodeList;
    InventoryNodeList m_nodes;                         // узлы дерева на данном уровне

    DataBaseManager *m_dbman;

    explicit InventoryModel(DataBaseManager *dbman, QObject *parent = nullptr);
    ~InventoryModel();

    // фабрики узлов дерева
    InventoryNode makeCategoryNode(const CategoryItem &item);
    InventoryNode makeGroupNode(const GroupItem &item, InventoryNode *parent);
    InventoryNode makeProductNode(const ProductItem &item, InventoryNode *parent);

    // инициализация модели
    void buildCategoryLevel();
    void buildGroupLevel();
    void buildProductLevel();
    void initModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int findRow(const InventoryNode *invNode) const;

private:
};

#endif // INVENTORYMODEL_H
