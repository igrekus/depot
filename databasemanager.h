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
#include <constants.h>
#include <reportrequest.h>

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
    ProductItem::ProductList getProductListByGroup(const qint32 catId, const qint32 groupId);
    TransactItem::TransactList getTransactList();

    IdStringList getIdProductList();

    StockItem getStockByProductId(const qint32 prodId);
    QPair<qint32, qint32> getProductParents(const qint32 prodId);

    // словари
    HashDict getMapLocation();
    HashDict getMapProject();
    HashDict getMapMiscTag();
    HashDict getMapCategory();
    HashDict getMapGroup();
    HashDict getMapStaff();

    // связи между словарями
    IdMap getMapGroupToCategory();

    // стата по запросу
    QSqlQuery getStockStats(const ReportRequest &req);
    QSqlQuery getTransactStats(const ReportRequest &req);

    // запись данных в БД
    qint32 insertCategory(const QString &name);
    void updateCategory(const CategoryItem &item);
    void deleteCategory(const CategoryItem &item);

    qint32 insertGroup(const GroupItem &item);
    void updateGroup(const GroupItem &item);
    void deleteGroup(const GroupItem &item);

    qint32 insertProduct(const ProductItem &item);
    void updateProduct(const ProductItem &item);
    void deleteProduct(const ProductItem &item);

    qint32 insertStock(const StockItem &item);
    void updateStock(const StockItem &item);
    void deleteStock(const StockItem &item);

    qint32 insertTransact(const TransactItem &item);
    void updateTransact(const TransactItem &item);
    void deleteTransact(const TransactItem &item);

    // utility
    void convertDB();

signals:

public slots:
};

#endif // DATABASEMANAGER_H
