#include "decoderproxymodel.h"

DecoderProxyModel::DecoderProxyModel(QObject *parent) : QIdentityProxyModel(parent)
{

}

DecoderProxyModel::~DecoderProxyModel()
{

}

QVariant DecoderProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (role != Qt::DisplayRole)
        return QIdentityProxyModel::data(proxyIndex, role);

    QVariant d = QIdentityProxyModel::data(proxyIndex, role);
    switch (d.type()) {
    case QVariant::Date:
        return d.toDate().toString("dd.MM.yyyy");
        break;
    case QVariant::String:
        return d.toString().toLocal8Bit();
        break;
    default:
        return d;
        break;
    }
    return QVariant();
}
