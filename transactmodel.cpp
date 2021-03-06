﻿#include "transactmodel.h"

TransactModel::TransactModel(DataBaseManager *dbman, DictModel *dictModel, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_dbman = dbman;
    m_dictModel = dictModel;
}

void TransactModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_data.size());
    m_data.clear();
    endRemoveRows();
}

void TransactModel::initModel()
{
    // TODO: Динамическая подгруза списка после первых 1к, когда прокрутка вниз
    qDebug()<<"building transact list";
    TransactItem::TransactList tmplist = m_dbman->getTransactList();
    beginInsertRows(QModelIndex(), 0, tmplist.isEmpty() ? 0 : tmplist.size()-1);
    m_data = std::move(tmplist.toList());
    endInsertRows();
}

QVariant TransactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < m_headers.size()) {
        return m_headers.at(section);
    }
    return QVariant();
}

int TransactModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_data.size();
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
    case Qt::ToolTipRole:
    case Qt::DisplayRole: {
        switch (index.column()) {
        case ColumnDate: {
            return QVariant(m_data.at(index.row()).itemDate.toString("dd.MM.yyyy"));
            break;
        }
        case ColumnProduct: {
            return QVariant(m_data.at(index.row()).itemName);
            break;
        }
        case ColumnDiff: {
//            return QVariant(std::abs(m_nodeList.at(index.row()).transactItem.itemDiff));
            return QVariant(m_data.at(index.row()).itemDiff);
            break;
        }
        case ColumnProject: {
            return QVariant(m_dictModel->m_projectListModel->getData(m_data.at(index.row()).itemProjectRef));
            break;
        }
        case ColumnStaff: {
            return QVariant(m_dictModel->m_staffListModel->getData(m_data.at(index.row()).itemStaffRef));
            break;
        }
        case ColumnNote: {
            return QVariant(m_data.at(index.row()).itemNote);
            break;
        }
        case ColumnCount:
        default:
            break;
        }
        break;
    }
    case Constants::RoleNodeId: {
        return QVariant(m_data.at(index.row()).itemId);
        break;
    }
    case Qt::TextAlignmentRole: {
        switch (index.column()) {
        case ColumnDate :{
//            return Qt::AlignCenter;
            break;
        }
        case ColumnDiff: {
            return Qt::AlignRight;
            break;
        }
        case ColumnStaff: {
//            return Qt::AlignCenter;
            break;
        }
        case ColumnProject: {
//            return Qt::AlignCenter;
            break;
        }
        }
    }
    case Qt::BackgroundRole: {
        switch (index.column()) {
        case ColumnDiff: {
            if (m_data.at(index.row()).itemDiff > 0) {
                return QVariant(QBrush(QColor(QRgb(Constants::ColorReceiptBackground))));
            } else {
                return QVariant(QBrush(QColor(QRgb(Constants::ColorExpenseBackground))));
            }
            break;
        }
        }
        break;
    }
    case Constants::RoleProjectId: {
        return QVariant(m_data.at(index.row()).itemProjectRef);
        break;
    }
    default:
        break;
    }
    return QVariant();
}

TransactItem TransactModel::getTransactItemByIndex(const QModelIndex &index)
{
    return (m_data.at(index.row()));
}

QModelIndex TransactModel::addTransact(const TransactItem &item)
{
//    qint32 newId = 100;
    qint32 newId = m_dbman->insertTransact(item);

    TransactItem tmpitem(item);
    tmpitem.itemId = newId;

    beginInsertRows(QModelIndex(), 0, 0);
    m_data.prepend(tmpitem);
    endInsertRows();

    return index(0, 0, QModelIndex());
}

void TransactModel::editTransact(const QModelIndex &index, const TransactItem &item)
{
    m_dbman->updateTransact(item);

    m_data.replace(index.row(), item);
    emit dataChanged(index, index);
}

void TransactModel::deleteTransact(const QModelIndex &index)
{
    m_dbman->deleteTransact(m_data.at(index.row()));

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_data.removeAt(index.row());
    endRemoveRows();
}
