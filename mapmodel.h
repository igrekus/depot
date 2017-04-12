#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QAbstractListModel>
#include <hashdict.h>

#define ROLE_NODE_ID   (Qt::UserRole+3)

class MapModel : public QAbstractListModel
{
    Q_OBJECT

public:

    HashDict m_mapData;
    QStringList m_strList;

    explicit MapModel(QObject *parent = nullptr);

    void initModel(const HashDict &data);
    void clearModel();

    void addItem(const qint32 pos, const qint32 id, const QString &str);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // MAPMODEL_H
