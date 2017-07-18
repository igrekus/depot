#include "mapmodel.h"

MapModel::MapModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void MapModel::initModel(const HashDict &data)
{
    qint32 cnt = data.id.values().size() - 1;
    if (cnt <= 0)
        cnt = 0;
    beginInsertRows(QModelIndex(), 0, data.id.values().isEmpty() ? 0 : data.id.values().size()-1);
    m_mapData = data;
    m_strList = data.id.values();
    std::sort(m_strList.begin(), m_strList.end());
    endInsertRows();
}

void MapModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_strList.isEmpty() ? 0 : m_strList.size()-1);
    m_mapData.id.clear();
    m_mapData.di.clear();
    m_strList.clear();
    endRemoveRows();
}

bool MapModel::isEmpty()
{
    return m_strList.isEmpty();
}

void MapModel::addItem(const qint32 id, const QString &str)
{
    auto rowIter = std::find_if(m_strList.begin(), m_strList.end(), [&str](const QString &it){
        return it > str;
    });
    qint32 row = std::distance(m_strList.begin(), rowIter);
    m_mapData.id.insert(id, str);
    m_mapData.di.insert(str, id);

    beginInsertRows(QModelIndex(), row, row);
    m_strList.insert(row, str);
    endInsertRows();
}

void MapModel::addItemAtPosition(const qint32 pos, const qint32 id, const QString &str)
{
    beginInsertRows(QModelIndex(), pos, pos);
    m_strList.insert(pos, str);
    m_mapData.id.insert(id, str);
    m_mapData.di.insert(str, id);
    endInsertRows();
}

void MapModel::editItem(const qint32 id, const QString &name)
{
    qint32 row = m_strList.indexOf(m_mapData.id.value(id));

    m_mapData.di.remove(m_mapData.id.value(id));
    m_mapData.id.replace(id, name);
    m_mapData.di.insert(name, id);

    m_strList.replace(row, name);
    dataChanged(index(row, 0, QModelIndex()), index(row, 0, QModelIndex()));
}

void MapModel::removeItem(const qint32 id)
{
    qint32 row = m_strList.indexOf(m_mapData.id.value(id));

    m_mapData.di.remove(m_mapData.id.value(id));
    m_mapData.id.remove(id);

    beginRemoveRows(QModelIndex(), row, row);
    m_strList.removeAt(row);
    endRemoveRows();
}

QString MapModel::getData(const qint32 id)
{
    return m_mapData.id.value(id);
}

qint32 MapModel::getId(const QString &data)
{
    return m_mapData.di.value(data);
}

QVariant MapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList headers = {"Название"};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers[section];
    }
    return QVariant();
}

int MapModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return m_mapData.id.size();
}

QVariant MapModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole: {
        return m_strList.at(index.row());
        break;
    }
    case Constants::RoleNodeId: {
        return m_mapData.di.value(m_strList.at(index.row()));
    }
    }

    return QVariant();
}
