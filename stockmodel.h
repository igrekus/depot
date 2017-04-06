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
        ProjectColumn,
        LocationColumn,
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
    StockNode makeGroupNode(const GroupItem &item, StockNode *parent = nullptr);
    StockNode makeStockNode(const StockItem &item, StockNode *parent = nullptr);

    void initModel();
    void buildCategoryLevel();
    void buildGroupLevel();
    void buildStockLevel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int findRow(const StockNode *stockNode) const;

    QModelIndex addCategory(const QString &catName);
    void editCategory(const QModelIndex &index, const QString &newName);
    void deleteCategory(const QModelIndex &index);

private:
};

#endif // STOCKMODEL_H
