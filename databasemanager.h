#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCodec>
#include <QDate>
#include <QMessageBox>

#include <categoryitem.h>
#include <groupitem.h>
#include <productitem.h>
#include <stockitem.h>
#include <hashdict.h>
#include <transactitem.h>
#include <linkeddict.h>
#include <constants.h>
#include <reportrequest.h>
#include <classitem.h>

class DataBaseManager : public QObject
{
    Q_OBJECT
public:

    struct ProductParents {
        qint32 parentClass    = 0;
        qint32 parentCategory = 0;
        qint32 parentGroup    = 0;

        ProductParents():
            parentClass(),
            parentCategory(),
            parentGroup()
        {}
    };

    explicit DataBaseManager(QObject *parent = 0);

    void connectToDatabase();

    QSqlQuery execSimpleQuery(const QString &qry);
    QSqlQuery execParametrizedQuery(const QString &qry, const QVariantList &params);

    // списки итемов
    ClassItem::ClassList getClassList();
    CategoryItem::CategoryList getCategoryList(const qint32 classId);
    GroupItem::GroupList getGroupList(const qint32 catId);
    StockItem::StockList getStockList(const qint32 catId, const qint32 groupId);
    ProductItem::ProductList getProductListByGroup(const qint32 groupId);
    TransactItem::TransactList getTransactList();

    IdStringList getIdProductList();

    StockItem getStockByProductId(const qint32 prodId);
    ProductParents getProductParents(const qint32 prodId);

    // словари
    HashDict fillHashDict(QSqlQuery &&q);
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

    qint32 insertDictRecord(const qint32 &tableId, const QString &name);
    void updateDictRecord(const qint32 &tableId, const qint32 recId, const QString &name);
    void deleteDictRecord(const qint32 &tableId, const qint32 recId);

    // проверки на наличие
    bool checkLocationFk(const qint32 locId);
    bool checkStaffFk(const qint32 staffId);
    bool checkProjectFk(const qint32 projId);

    // utility
    TransactItem makeTransactItemFromStockItem(const StockItem &stock);

signals:

public slots:
};

#endif // DATABASEMANAGER_H
