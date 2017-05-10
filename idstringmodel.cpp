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

bool IdStringModel::isEmpty()
{
    return m_data.isEmpty();
}

void IdStringModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_data.count());
    m_data.clear();
    endRemoveRows();
}

int IdStringModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.size();
}

QVariant IdStringModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole: {
        return m_data.at(index.row()).second;
    }
    case Constants::RoleNodeId: {
        return m_data.at(index.row()).first;
    }
    }

    return QVariant();
}

qint32 IdStringModel::getId(const QString &name) const
{
    auto it = std::find_if(m_data.begin(), m_data.end(), [&name](const IdPair &it) -> bool {
        return (it.second == name);
    });
    if (it == m_data.end()) {
        return 0;
    }
    return m_data.at(std::distance(m_data.begin(), it)).first;
}

QString IdStringModel::getName(const qint32 id) const
{
    auto it = std::find_if(m_data.begin(), m_data.end(), [&id](const IdPair &it) -> bool {
        return (it.first == id);
    });
    if (it == m_data.end()) {
        return QString();
    }
    return m_data.at(std::distance(m_data.begin(), it)).second;
}
