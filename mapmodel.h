#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QAbstractListModel>
#include <hashdict.h>
#include <constants.h>

class MapModel : public QAbstractListModel
{
    Q_OBJECT

public:

    HashDict m_mapData;
    QStringList m_strList;

    explicit MapModel(QObject *parent = nullptr);

    void initModel(const HashDict &data);
    void clear();

    bool isEmpty();

    void addItem(const qint32 pos, const qint32 id, const QString &str);

    QString getData(const qint32 id);
    qint32 getId(const QString &data);
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // MAPMODEL_H
