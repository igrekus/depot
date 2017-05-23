#ifndef STOCKMODEL_H
#define STOCKMODEL_H

#include <QAbstractItemModel>

#include <databasemanager.h>
#include <dictmodel.h>

#define DATA_INVALID -1

#define ROLE_LEVEL_ID  (Qt::UserRole+1)
#define ROLE_NODE_TYPE (Qt::UserRole+2)
#define ROLE_NODE_ID   (Qt::UserRole+3)
#define ROLE_NODE_HAS_CHILDREN (Qt::UserRole+4)

class StockItem;

class StockModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    enum Columns {
        RamificationColumn = 0,
        NumberColumn = RamificationColumn,
        NameColumn,
        UnitColumn,
        AmountColumn,
        SerialnColumn,
        ProjectColumn,
        LocationColumn,
        FullnameColumn,
        ColumnCount
    };
    Q_ENUM(Columns)

    // данные
    struct StockNode;
    typedef QList<StockNode> StockNodeList;
    StockNodeList m_nodes;                         // узлы дерева на данном уровне

    // менеджеры
    DataBaseManager *m_dbman;
    DictModel *m_dictModel;

    // методы
    explicit StockModel(DataBaseManager *dbman, DictModel *dictModel, QObject *parent = nullptr);
    ~StockModel();

    // фабрики узлов дерева
    StockNode makeCategoryNode(const CategoryItem &item);
    StockNode makeGroupNode(const GroupItem &item, StockNode *parent);
    StockNode makeStockNode(const StockItem &item, StockNode *parent);

    // инициализация модели
    void buildCategoryLevel();
    void buildGroupLevel();
    void buildStockLevel();
    void initModel();

    void clearNodeList(StockModel::StockNodeList &list);
    void clear();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int findRow(const StockNode *stockNode) const;

    QModelIndex addStock(const StockItem &item);
    void editStock(const QModelIndex &index, const StockItem &item);
    void deleteStock(const QModelIndex &index);

    StockItem getStockItemByIndex(const QModelIndex &index);

    // TODO: debug helper
    void debugInfo(const QModelIndex &index);

private:
};

#endif // STOCKMODEL_H
