#include "mapmodel.h"

MapModel::MapModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void MapModel::initModel(const HashDict &data)
{
    m_mapData = data;
    m_strList = data.id.values();
    std::sort(m_strList.begin(), m_strList.end());
}

void MapModel::clearModel()
{
    m_mapData.id.clear();
    m_mapData.di.clear();
    m_strList.clear();
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
        return QVariant(m_strList.at(index.row()));
        break;
    }
    case ROLE_NODE_ID: {
        return QVariant(m_mapData.di.value(m_strList.at(index.row())));
    }
    }

    return QVariant();
}
