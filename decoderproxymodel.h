#ifndef DECODERPROXYMODEL_H
#define DECODERPROXYMODEL_H

#include <QObject>
#include <QIdentityProxyModel>
#include <QDate>
#include <QDebug>

class DecoderProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    qint32 m_rowCount = 0;

    DecoderProxyModel(QObject *parent = nullptr);
    ~DecoderProxyModel();

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const override;
};

#endif // DECODERPROXYMODEL_H
