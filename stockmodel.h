#ifndef STOCKMODEL_H
#define STOCKMODEL_H

#include <QAbstractItemModel>

#include <databasemanager.h>
#include <dictmodel.h>

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
        ExpireColumn,
        ColumnCount
    };
    Q_ENUM(Columns)

    const QStringList m_headers = {"Каталог",
                                   "Наименование",
                                   "Единица",
                                   "Остаток",
                                   "№ партии",
                                   "Тема",
                                   "Место хранения",
                                   "Срок годности"};

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
    StockNode makeClassNode(const ClassItem &item);
    StockNode makeCategoryNode(const CategoryItem &item, StockNode *parent);
    StockNode makeGroupNode(const GroupItem &item, StockNode *parent);
    StockNode makeStockNode(const StockItem &item, StockNode *parent);

    // заполнение отдельных узлов
    // TODO: refactor these into one function (with functors)
    void fillClassNode(const QModelIndex &index, StockNode &node);
    void fillCategoryNode(const QModelIndex &index, StockNode &node);
    void fillGroupNode(const QModelIndex &index, StockNode &node);

    // инициализация модели
    void buildClassLevel();
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
    void modifyStockByTransact(const TransactItem &item);

    StockItem getStockItemByIndex(const QModelIndex &index);
    StockModel::StockNode *findStockNodeByTransactItem(const TransactItem &item);
    QModelIndex findStockIndexByTransactItem(const TransactItem &item);

    // TODO: debug helper
    void debugInfo(const QModelIndex &index);

private:
};

#endif // STOCKMODEL_H
