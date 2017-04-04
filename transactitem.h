#ifndef TRANSACTITEM_H
#define TRANSACTITEM_H

#include <QObject>
#include <QDate>
#include <QDebug>

#include <abstractitem.h>

class TransactItem : public AbstractItem
{
public:

    QDate   itemDate;
    qint32  itemDiff;
    QString itemNote;
    QString itemStaff;
    qint32  itemBillRef;

    explicit TransactItem():
        AbstractItem(),
        itemDate(),
        itemDiff(),
        itemNote(),
        itemStaff(),
        itemBillRef()
    {}

    explicit TransactItem(const qint32   id,
                          const QString &name,
                          const QDate   &date,
                          const qint32   diff,
                          const QString &note,
                          const QString &staff,
                          const qint32   bill):
        AbstractItem(id, name),
        itemDate   (date),
        itemDiff   (diff),
        itemNote   (note),
        itemStaff  (staff),
        itemBillRef(bill)
    {}

    TransactItem(const TransactItem &copy):
        AbstractItem(copy),
        itemDate   (copy.itemDate),
        itemDiff   (copy.itemDiff),
        itemNote   (copy.itemNote),
        itemStaff  (copy.itemStaff),
        itemBillRef(copy.itemBillRef)
    {}

    TransactItem &operator=(const TransactItem &right) {
        if (this != &right) {
            itemId      = right.itemId;
            itemName    = right.itemName;
            itemDate    = right.itemDate;
            itemDiff    = right.itemDiff;
            itemNote    = right.itemNote;
            itemStaff   = right.itemStaff;
            itemBillRef = right.itemBillRef;
        }
        return *this;
    }

    bool operator==(const TransactItem &right) const {
        return (itemId      == right.itemId    &&
                itemName    == right.itemName  &&
                itemDate    == right.itemDate  &&
                itemDiff    == right.itemDiff  &&
                itemNote    == right.itemNote  &&
                itemStaff   == right.itemStaff &&
                itemBillRef == right.itemBillRef);
    }

    friend QDebug operator<<(QDebug dbg, const TransactItem &right) {
        dbg.nospace() << "Transact("
                      << "id:"     << right.itemId
                      << " name:"  << right.itemName
                      << " date:"  << right.itemDate
                      << " diff:"  << right.itemDiff
                      << " note:"  << right.itemNote
                      << " staff:" << right.itemStaff
                      << " bill:"  << right.itemBillRef
                      << ")";
        return dbg.maybeSpace();
    }
    using TransactList = QVector<TransactItem>;

    class TransactItemBuilder {
    public:
        qint32  transId      = 0;
        QString transName    = QString();
        QDate   transDate    = QDate();
        qint32  transDiff    = 0;
        QString transNote    = QString();
        QString transStaff   = QString();
        qint32  transBillRef = 0;

        TransactItemBuilder& setId     (const qint32   id)    { this->transId      = id;    return *this; }
        TransactItemBuilder& setName   (const QString &name)  { this->transName    = name;  return *this; }
        TransactItemBuilder& setDate   (const QDate   &date)  { this->transDate    = date;  return *this; }
        TransactItemBuilder& setDiff   (const qint32   diff)  { this->transDiff    = diff;  return *this; }
        TransactItemBuilder& setNote   (const QString &note)  { this->transNote    = note;  return *this; }
        TransactItemBuilder& setStaff  (const QString &staff) { this->transStaff   = staff; return *this; }
        TransactItemBuilder& setBillRef(const qint32  &bill)  { this->transBillRef = bill;  return *this; }
        TransactItem build() {
            return TransactItem(this->transId,     this->transName,
                                this->transDate,   this->transDiff,
                                this->transNote,   this->transStaff,
                                this->transBillRef);
        }
    };
};

#endif // TRANSACTITEM_H
