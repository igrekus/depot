#include "transactmodel.h"

TransactModel::TransactModel(DataBaseManager *dbman, DictModel *dictModel, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_dbman = dbman;
    m_dictModel = dictModel;
}

void TransactModel::initModel()
{
    // TODO: Динамическая подгруза списка после первых 1к, когда прокрутка вниз
    qDebug()<<"building transact list";
    TransactItem::TransactList tmplist = m_dbman->getTransactList();
    beginInsertRows(QModelIndex(), 0, tmplist.size()-1);
    for (const TransactItem it : tmplist) {
        m_nodeList.append(TransactNode(it));
    }
    endInsertRows();
}

QVariant TransactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList headers = {"Дата", "Наименование", "Изменение", "Сотрудник", "Примечание"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers.at(section);
    }
    return QVariant();
}

int TransactModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_nodeList.size();
}

int TransactModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant TransactModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
    case Qt::DisplayRole: {
        switch (index.column()) {
        case ColumnDate: {
            return QVariant(m_nodeList.at(index.row()).transactItem.itemDate.toString(Qt::ISODate));
            break;
        }
        case ColumnProduct: {
            return QVariant(m_nodeList.at(index.row()).transactItem.itemName);
            break;
        }
        case ColumnDiff: {
            return QVariant(m_nodeList.at(index.row()).transactItem.itemDiff);
            break;
        }
        case ColumnStaff: {
            return QVariant(m_nodeList.at(index.row()).transactItem.itemStaff);
            break;
        }
        case ColumnNote: {
            return QVariant(m_nodeList.at(index.row()).transactItem.itemNote);
            break;
        }
        case ColumnCount:
        default:
            break;
        }
        break;
    }
    case ROLE_NODE_ID: {
        break;
    }
    default:
        break;
    }
    return QVariant();
}
