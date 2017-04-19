#include "idstringmodel.h"

IdStringModel::IdStringModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant IdStringModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant(QString());
}

void IdStringModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_data.first.count());
    m_data.first.clear();
    m_data.second.clear();
    endRemoveRows();
}

int IdStringModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.first.size();
}

QVariant IdStringModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole: {
        return m_data.second.at(index.row());
    }
    case Constants::RoleNodeId: {
        return m_data.first.at(index.row());
    }
    }

    return QVariant();
}
