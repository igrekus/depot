#ifndef STOCKMODEL_H
#define STOCKMODEL_H

#include <QAbstractItemModel>

#include <databasemanager.h>

#define DATA_INVALID -1

#define ROLE_LEVEL_ID  (Qt::UserRole+1)
#define ROLE_NODE_TYPE (Qt::UserRole+2)

class StockItem;

class StockModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    enum TreeLevel {
        TreeRoot = 0,
        TreeLevel_1,
        TreeLevel_2
    };
    Q_ENUM(TreeLevel)

    enum Columns {
        RamificationColumn = 0,
        NumberColumn = RamificationColumn,
        NameColumn,
        AmountColumn,
        SerialnColumn,
        LocationColumn,
        ColumnCount
    };
    Q_ENUM(Columns)

    // данные
    struct StockNode;
    typedef QVector<StockNode> StockNodeList;
    StockNodeList _nodes;                         // узлы дерева на данном уровне

    // менеджеры
    DataBaseManager *m_dbman;

    // методы
    explicit StockModel(DataBaseManager *dbman, QObject *parent = nullptr);
    ~StockModel();

    // фабрики узлов дерева
    StockNode nodeFactoryCategory(const CategoryItem &item);
    StockNode nodeFactoryGroup(const GroupItem &item, StockNode *parent = nullptr);
    StockNode nodeFactoryStock(const StockItem &item, StockNode *parent = nullptr);

    void initModel();
    void buildCategoryLevel();
    void buildGroupLevel();
    void buildStockLevel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    int findRow(const StockNode *stockNode) const;
};

#endif // STOCKMODEL_H
