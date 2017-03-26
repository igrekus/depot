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

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseManager(QObject *parent = 0);

    void connectToDatabase();

    QSqlQuery execSimpleQuery(const QString &qry);

    // получаем списки итемов из БД
    CategoryItem::CategoryList getCategoryList();
    GroupItem::GroupList getGroupList(qint32 catId);
    StockItem::StockList getStockList(qint32 catId, qint32 groupId);

signals:

public slots:
};

#endif // DATABASEMANAGER_H
