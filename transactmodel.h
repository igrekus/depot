#ifndef TRANSACTMODEL_H
#define TRANSACTMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include <QBrush>

#include <dictmodel.h>
#include <databasemanager.h>
#include <transactitem.h>

#define ROLE_NODE_ID   (Qt::UserRole+3)

#define COLOR_RECEIPT 0xBAFAAD
#define COLOR_EXPENSE 0xFFD3C9

class TransactModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Columns {
        ColumnDate = 0,
        ColumnProduct,
        ColumnDiff,
        ColumnStaff,
        ColumnProject,
        ColumnNote,
        ColumnCount
    };

    struct TransactNode {
        TransactItem transactItem;

        TransactNode():
            transactItem()
        {}

        TransactNode(const TransactItem &transItem):
            transactItem(transItem)
        {}

        bool operator==(const TransactNode &a) const {
            return this->transactItem == a.transactItem;
        }
    };
    // данные
    using TransactNodeList = QList<TransactNode>;
    TransactNodeList m_nodeList;

    // менеджеры
    DataBaseManager *m_dbman;
    DictModel *m_dictModel;

    explicit TransactModel(DataBaseManager *dbman, DictModel *dictModel,QObject *parent = nullptr);
    ~TransactModel(){}

    void initModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // TRANSACTMODEL_H
