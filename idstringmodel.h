#ifndef IDSTRINGMODEL_H
#define IDSTRINGMODEL_H

#include <QAbstractListModel>
#include <constants.h>

class IdStringModel : public QAbstractListModel
{
    Q_OBJECT

public:

    IdStringList m_data;

    explicit IdStringModel(QObject *parent = nullptr);

    IdStringModel &setData(const IdStringList &data) {m_data = data; return *this;}

    void clear();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    qint32 getId(const QString &name) const;
    QString getName(const qint32 id) const;

private:
};

#endif // IDSTRINGMODEL_H
