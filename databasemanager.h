#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCodec>
#include <QDate>

#include <categoryitem.h>
#include <groupitem.h>
#include <productitem.h>
#include <stockitem.h>
#include <hashdict.h>
#include <transactitem.h>
#include <linkeddict.h>

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseManager(QObject *parent = 0);

    void connectToDatabase();

    QSqlQuery execSimpleQuery(const QString &qry);
    QSqlQuery execParametrizedQuery(const QString &qry, const QVariantList &params);

    // списки итемов
    CategoryItem::CategoryList getCategoryList();
    GroupItem::GroupList getGroupList(const qint32 catId);
    StockItem::StockList getStockList(const qint32 catId, const qint32 groupId);
    ProductItem::ProductList getProductList(const qint32 catId, const qint32 groupId);
    TransactItem::TransactList getTransactList();

    // словари
    HashDict getMapLocation();
    HashDict getMapProject();
    HashDict getMapMiscTag();
    HashDict getMapCategory();

    // связи между словарями
    LinkedDict getLinkGroupToCategory();

    // запись данных в БД
    qint32 insertCategory(const QString &name);
    void updateCategory(const CategoryItem &item);
    void deleteCategory(const CategoryItem &item);

    qint32 insertGroup(const QString &name);
    void updateGroup(const GroupItem &item);
    void deleteGroup(const GroupItem &item);

    qint32 insertProduct(const ProductItem &item);
    void updateProduct(const ProductItem &item);
    void deleteProduct(const ProductItem &item);

    // utility
    void convertDB();

signals:

public slots:
};

#endif // DATABASEMANAGER_H
