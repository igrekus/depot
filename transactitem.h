#ifndef TRANSACTITEM_H
#define TRANSACTITEM_H

#include <QObject>
#include <QDate>
#include <QDebug>

#include <abstractitem.h>
#include <stockitem.h>

class TransactItem : public AbstractItem
{
public:

    QDate   itemDate;
    qint32  itemDiff;
    QString itemNote;
    qint32  itemStockRef;
    qint32  itemStaffRef;
    qint32  itemProjectRef;
    qint32  itemBillRef;

    explicit TransactItem():
        AbstractItem(),
        itemDate(),
        itemDiff(),
        itemNote(),
        itemStockRef(),
        itemStaffRef(),
        itemProjectRef(),
        itemBillRef()
    {}

    explicit TransactItem(const qint32   id,
                          const QString &name,
                          const QDate   &date,
                          const qint32   diff,
                          const QString &note,
                          const qint32   stock,
                          const qint32   staff,
                          const qint32   proj,
                          const qint32   bill):
        AbstractItem(id, name),
        itemDate      (date),
        itemDiff      (diff),
        itemNote      (note),
        itemStockRef  (stock),
        itemStaffRef  (staff),
        itemProjectRef(proj),
        itemBillRef   (bill)
    {}

    TransactItem(const TransactItem &copy):
        AbstractItem(copy),
        itemDate      (copy.itemDate),
        itemDiff      (copy.itemDiff),
        itemNote      (copy.itemNote),
        itemStockRef  (copy.itemStockRef),
        itemStaffRef  (copy.itemStaffRef),
        itemProjectRef(copy.itemProjectRef),
        itemBillRef   (copy.itemBillRef)
    {}

    TransactItem &operator=(const TransactItem &right) {
        if (this != &right) {
            itemId         = right.itemId;
            itemName       = right.itemName;
            itemDate       = right.itemDate;
            itemDiff       = right.itemDiff;
            itemNote       = right.itemNote;
            itemStockRef   = right.itemStockRef;
            itemStaffRef   = right.itemStaffRef;
            itemProjectRef = right.itemProjectRef;
            itemBillRef    = right.itemBillRef;
        }
        return *this;
    }

    bool operator==(const TransactItem &right) const {
        return (itemId         == right.itemId     &&
                itemName       == right.itemName   &&
                itemDate       == right.itemDate   &&
                itemDiff       == right.itemDiff   &&
                itemNote       == right.itemNote   &&
                itemStockRef   == right.itemStockRef  &&
                itemStaffRef   == right.itemStaffRef  &&
                itemProjectRef == right.itemProjectRef&&
                itemBillRef    == right.itemBillRef);
    }

    friend QDebug operator<<(QDebug dbg, const TransactItem &right) {
        dbg.nospace() << "Transact("
                      << "id:"     << right.itemId
                      << " name:"  << right.itemName
                      << " date:"  << right.itemDate
                      << " diff:"  << right.itemDiff
                      << " note:"  << right.itemNote
                      << " stock:" << right.itemStockRef
                      << " staff:" << right.itemStaffRef
                      << " proj:"  << right.itemProjectRef
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
        qint32  transStockRef= 0;
        qint32  transStaffRef= 0;
        qint32  transProject = 0;
        qint32  transBillRef = 0;

        TransactItemBuilder& setId     (const qint32   id)    { this->transId      = id;    return *this; }
        TransactItemBuilder& setName   (const QString &name)  { this->transName    = name;  return *this; }
        TransactItemBuilder& setDate   (const QDate   &date)  { this->transDate    = date;  return *this; }
        TransactItemBuilder& setDiff   (const qint32   diff)  { this->transDiff    = diff;  return *this; }
        TransactItemBuilder& setNote   (const QString &note)  { this->transNote    = note;  return *this; }
        TransactItemBuilder& setStock  (const qint32  &stock) { this->transStockRef= stock; return *this; }
        TransactItemBuilder& setStaff  (const qint32  &staff) { this->transStaffRef= staff; return *this; }
        TransactItemBuilder& setProject(const qint32  &proj ) { this->transProject = proj;  return *this; }
        TransactItemBuilder& setBill(const qint32  &bill)     { this->transBillRef = bill;  return *this; }
        TransactItem build() {
            return TransactItem(this->transId,
                                this->transName,
                                this->transDate,
                                this->transDiff,
                                this->transNote,
                                this->transStockRef,
                                this->transStaffRef,
                                this->transProject,
                                this->transBillRef);
        }
        TransactItem fromStockItem(const StockItem &stock) {
            this->transName = stock.itemName;
            this->transDate = QDate::currentDate();
            this->transStockRef = stock.itemId;
            this->transProject = stock.itemProject;
            return TransactItem(this->transId,
                                this->transName,
                                this->transDate,
                                this->transDiff,
                                this->transNote,
                                this->transStockRef,
                                this->transStaffRef,
                                this->transProject,
                                this->transBillRef);
        }
    };
};

#endif // TRANSACTITEM_H
