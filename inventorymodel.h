#ifndef INVENTORYMODEL_H
#define INVENTORYMODEL_H

#include <QAbstractItemModel>
#include <QDebug>

#include <categoryitem.h>
#include <groupitem.h>
#include <productitem.h>
#include <databasemanager.h>
#include <inventoryitem.h>
#include <dictmodel.h>

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
    InventoryNodeList m_nodes;                         // root nodes

    DataBaseManager *m_dbman;

    DictModel *m_dictModel;

    explicit InventoryModel(QObject *parent = nullptr);
    ~InventoryModel();

    InventoryModel &setDatabaseManager(DataBaseManager *dbman) {m_dbman    = dbman; return *this;}
    InventoryModel &setDictModel      (DictModel       *dict) {m_dictModel = dict;  return *this;}

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

    QModelIndex addCategory(const QString &catName);
    QModelIndex editCategory(const QModelIndex &index, const QString &newName);
    void deleteCategory(const QModelIndex &index);

    QModelIndex addGroup(const QModelIndex &pindex, const QString &grpName);
    QModelIndex editGroup(const QModelIndex &index, const QString &newName);
    void deleteGroup(const QModelIndex &index);

    QModelIndex addInventory(const QModelIndex &pindex, const ProductItem &item);
    QModelIndex editInventory(const QModelIndex &index, const ProductItem &item);
    void deleteInventory(const QModelIndex &index);

    ProductItem getProductItemByIndex(const QModelIndex &index);
    InventoryItem getInventoryItemByIndex(const QModelIndex &index);

private:
};

#endif // INVENTORYMODEL_H
