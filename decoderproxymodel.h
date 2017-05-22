#ifndef DECODERPROXYMODEL_H
#define DECODERPROXYMODEL_H

#include <QObject>
#include <QIdentityProxyModel>
#include <QDate>

class DecoderProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    DecoderProxyModel(QObject *parent = nullptr);
    ~DecoderProxyModel();

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const override;
};

#endif // DECODERPROXYMODEL_H
