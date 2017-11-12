#ifndef TRANSACTMODEL_H
#define TRANSACTMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include <QBrush>

#include <dictmodel.h>
#include <databasemanager.h>
#include <transactitem.h>

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
        ColumnExpire,
        ColumnCount
    };

    const QStringList m_headers = {"Дата транзакции",
                                   "Наименование",
                                   "+/-",
                                   "Сотрудник",
                                   "Тема",
                                   "Примечание",
                                   "Срок годности"};

    // данные
    using TransactItemList = QList<TransactItem>;
    TransactItemList m_data;

    // менеджеры
    DataBaseManager *m_dbman;
    DictModel *m_dictModel;

    explicit TransactModel(DataBaseManager *dbman, DictModel *dictModel,QObject *parent = nullptr);
    ~TransactModel(){}

    void clear();

    void initModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    TransactItem getTransactItemByIndex(const QModelIndex &index);

    QModelIndex addTransact(const TransactItem &item);
    void editTransact(const QModelIndex &index, const TransactItem &item);
    void deleteTransact(const QModelIndex &index);

private:
};

#endif // TRANSACTMODEL_H
