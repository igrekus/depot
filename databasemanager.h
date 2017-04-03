#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCodec>

#include <categoryitem.h>
#include <groupitem.h>
#include <productitem.h>
#include <stockitem.h>
#include <hashdict.h>

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseManager(QObject *parent = 0);

    void connectToDatabase();

    QSqlQuery execSimpleQuery(const QString &qry);
    QSqlQuery execParametrizedQuery(const QString &qry, const QVariantList &params);

    // списки итемов из БД
    CategoryItem::CategoryList getCategoryList();
    GroupItem::GroupList getGroupList(const qint32 catId);
    StockItem::StockList getStockList(const qint32 catId, const qint32 groupId);

    // словари
    HashDict getMapLocation();
    HashDict getMapProjectTag();
    HashDict getMapMiscTag();
    HashDict getMapCategory();
signals:

public slots:
};

#endif // DATABASEMANAGER_H
