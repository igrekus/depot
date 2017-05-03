#include "databasemanager.h"

#define AT_WORK

DataBaseManager::DataBaseManager(QObject *parent) : QObject(parent)
{

}

void DataBaseManager::connectToDatabase()
{
    // устанавливаем соединение с бд
    // реализуем "неправильно", т.к. в нашем случае все параметры подключения заданы заранее
    // глобальный инстанс объекта создавать не нужно, работаем через статические функции QSqlDatabase

    qDebug() << "connecting to db...";

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("depot");

    db.open();
//    if (!db.open()) {
//        throw db.lastError();
//    }

    qDebug() << "ok";
}

QSqlQuery DataBaseManager::execSimpleQuery(const QString &qry)
{
    // TODO: обработка ошибок запросов к БД через throw
    QSqlQuery query(QSqlDatabase::database());
    query.exec(qry);
    qDebug().noquote() << ">" << query.lastQuery() << "|" << query.lastError()
                       << "| rows: " << query.numRowsAffected();
    Q_ASSERT(query.isActive());
    return query;
}

QSqlQuery DataBaseManager::execParametrizedQuery(const QString &qry, const QVariantList &params)
{
    // TODO: если надо будет записывать много записей, сделать через execBatch (см хелп)
    // FIXME: не работают бинды
    QSqlQuery query(QSqlDatabase::database());
    query.prepare(qry);
    for (qint32 i=0; i<params.count(); ++i) {
        query.bindValue(i, params.at(i));
    }
    query.exec();
    qDebug().noquote() << ">" << query.lastQuery() << "|" << query.lastError()
                       << "| rows: " << query.numRowsAffected();
    Q_ASSERT(query.isActive());
    return query;
}

CategoryItem::CategoryList DataBaseManager::getCategoryList()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    CategoryItem::CategoryList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getCategoryList()");

    while (q.next()) {
#ifndef AT_WORK
        tmplist.append(CategoryItem::CategoryItemBuilder()
                       .setId  (q.value(0).toInt())
                       .setName(q.value(1).toString())
                       .build());
#endif

#ifdef AT_WORK
        tmplist.append(CategoryItem::CategoryItemBuilder()
                       .setId  (                  q.value(0).toInt())
                       .setName(decode->toUnicode(q.value(1).toString().toLocal8Bit()))
                       .build());
#endif
    }
    return tmplist;
}


GroupItem::GroupList DataBaseManager::getGroupList(const qint32 catId)
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif
    GroupItem::GroupList tmplist;
//    QVariantList params;

//    params.append(QVariant(catId));

//    QSqlQuery q = execParametrizedQuery("CALL getGroupListByCategory(?)", params);
    QSqlQuery q = execSimpleQuery("CALL getGroupListByCategory("+QString::number(catId)+");");

    while (q.next()) {
#ifndef AT_WORK
        tmplist.append(GroupItem::GroupItemBuilder()
                       .setId  (q.value(0).toInt())
                       .setName(q.value(1).toString())
                       .build());
#endif

#ifdef AT_WORK
        tmplist.append(GroupItem::GroupItemBuilder()
                       .setId  (                  q.value(0).toInt())
                       .setName(decode->toUnicode(q.value(1).toString().toLocal8Bit()))
                       .build());
#endif
    }
    return tmplist;
}

StockItem::StockList DataBaseManager::getStockList(const qint32 catId, const qint32 groupId)
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    StockItem::StockList tmplist;

//    QSqlQuery q = execSimpleQuery("CALL getStockByCategoryAndGroup("+QString::number(catId)+", "+QString::number(groupId)+")");
    Q_UNUSED(catId)
    QSqlQuery q = execSimpleQuery("CALL getStockByGroup("+QString::number(groupId)+")");
    while (q.next()) {

#ifndef AT_WORK
        tmplist.append(StockItem::StockItemBuilder()
                       .setId         (q.value(0).toInt())
                       .setName       (q.value(1).toString())
                       .setType       (StockItem::ItemStock)
                       .setLevel      (StockItem::Level_2)
                       .setAmount     (q.value(2).toInt())
                       .setSerialn    (q.value(3).toString())
                       .setProject    (q.value(4).toString())
                       .setLocation   (q.value(5).toString())
                       .build());
#endif

#ifdef AT_WORK
        tmplist.append(StockItem::StockItemBuilder()
                       .setId         (                  q.value(0).toInt())
                       .setName       (decode->toUnicode(q.value(1).toString().toLocal8Bit()))
                       .setType       (                  Constants::ItemItem)
                       .setLevel      (                  Constants::Level_2)
                       .setAmount     (                  q.value(2).toInt())
                       .setSerialn    (decode->toUnicode(q.value(3).toString().toLocal8Bit()))
                       .setProject    (                  q.value(4).toString().toInt())
                       .setLocation   (                  q.value(5).toString().toInt())
                       .setProduct    (                  q.value(6).toString().toInt())
                       .build());
#endif
    }
    return tmplist;
}

ProductItem::ProductList DataBaseManager::getProductList(const qint32 catId, const qint32 groupId)
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    ProductItem::ProductList tmplist;

    Q_UNUSED(catId)
    QSqlQuery q = execSimpleQuery("CALL getProductByGroup("+QString::number(groupId)+")");
    while (q.next()) {

#ifndef AT_WORK
#endif

#ifdef AT_WORK
        tmplist.append(ProductItem::ProductItemBuilder()
                       .setId      (                  q.value(0).toInt())
                       .setName    (decode->toUnicode(q.value(1).toString().toLocal8Bit()))
                       .setFullname(decode->toUnicode(q.value(2).toString().toLocal8Bit()))
                       .setSerialn (decode->toUnicode(q.value(3).toString().toLocal8Bit()))
                       .setUnit    (decode->toUnicode(q.value(4).toString().toLocal8Bit()))
                       .setMiscTag (decode->toUnicode(q.value(5).toString().toLocal8Bit()))
                       .setGroup   (                  q.value(6).toInt())
                       .setCategory(                  q.value(7).toInt())
                       .build());
#endif
    }
    return tmplist;
}

TransactItem::TransactList DataBaseManager::getTransactList()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    TransactItem::TransactList tmplist;

    // TODO: неправильный запрос, запрашивать сток
//    QSqlQuery q = execSimpleQuery("CALL getTransactList1k()");
    QSqlQuery q = execSimpleQuery("CALL getTransactListFull()");

    while (q.next()) {
#ifndef AT_WORK
        tmplist.append(TransactItem::TransactItemBuilder()
                       .setId     (q.value(0).toInt())
                       .setDate   (q.value(1).toDate())
                       .setDiff   (q.value(2).toInt())
                       .setNote   (q.value(3).toString())
                       .setStaff  (q.value(4).toString())
                       .setName   (q.value(5).toString())
                       .setBillRef(q.value(6).toInt())
                       .build());
#endif

#ifdef AT_WORK
        tmplist.append(TransactItem::TransactItemBuilder()
                       .setId     (                  q.value(0).toInt())
                       .setDate   (                  q.value(1).toDate())
                       .setDiff   (                  q.value(2).toInt())
                       .setNote   (decode->toUnicode(q.value(3).toString().toLocal8Bit()))
                       .setStaff  (                  q.value(4).toInt())
                       .setName   (decode->toUnicode(q.value(5).toString().toLocal8Bit()))
                       .setProject(                  q.value(6).toInt())
                       .setStock  (                  q.value(7).toInt())
                       .setBill   (                  q.value(8).toInt())
                       .build());
#endif
    }
    return tmplist;
}

IdStringList DataBaseManager::getIdProductList()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    IdStringList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getIdProductList()");
//    QSqlQuery q = execSimpleQuery("CALL getIdProductList100()");
    while (q.next()) {
        QPair<qint32, QString> tmppair;
        tmppair.first  =                   q.value(0).toInt();
        tmppair.second = decode->toUnicode(q.value(1).toString().toLocal8Bit());
        tmplist.append(tmppair);
//        if (tmppair.second.contains("атаре"))
//            qDebug() << tmppair;
    }
    return tmplist;
}

StockItem DataBaseManager::getStockByProductId(const qint32 prodId)
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    QSqlQuery q = execSimpleQuery("CALL getStockByProductId("+QString::number(prodId)+")");

    if (!q.next()) {
        return StockItem::StockItemBuilder().build();
    }
#ifdef AT_WORK
    return StockItem::StockItemBuilder()
           .setId         (                  q.value(0).toInt())
           .setName       (decode->toUnicode(q.value(1).toString().toLocal8Bit()))
           .setType       (                  Constants::ItemItem)
           .setLevel      (                  Constants::Level_2)
           .setAmount     (                  q.value(2).toInt())
           .setSerialn    (decode->toUnicode(q.value(3).toString().toLocal8Bit()))
           .setProject    (                  q.value(4).toString().toInt())
           .setLocation   (                  q.value(5).toString().toInt())
           .setProduct    (                  q.value(6).toString().toInt())
           .build();
#endif
}

QPair<qint32, qint32> DataBaseManager::getProductParents(const qint32 prodId)
{
    QSqlQuery q = execSimpleQuery("CALL getProductParents("+QString::number(prodId)+")");

    QPair<qint32, qint32> tmppair;
    if (!q.next()) {
        tmppair.first=0;
        tmppair.second=0;
        return tmppair;
    }
    tmppair.first =q.value(0).toInt();
    tmppair.second=q.value(1).toInt();
    return tmppair;
}

HashDict DataBaseManager::getMapLocation()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    HashDict tmphash;
    QSqlQuery q = execSimpleQuery("CALL getLocationList()");
    while (q.next()) {
#ifndef AT_WORK
        tmphash.id.insert(q.value(0).toInt(),
                          q.value(1).toString());
        tmphash.di.insert(q.value(1).toString(),
                          q.value(0).toInt());
#endif

#ifdef AT_WORK
        tmphash.id.insert(                  q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmphash.di.insert(decode->toUnicode(q.value(1).toString().toLocal8Bit()),
                                            q.value(0).toInt());
#endif
    }
    return tmphash;
}

HashDict DataBaseManager::getMapProject()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    HashDict tmphash;
    QSqlQuery q = execSimpleQuery("CALL getProjectList()");
    while (q.next()) {
#ifndef AT_WORK
        tmphash.id.insert(q.value(0).toInt(),
                          q.value(1).toString());
        tmphash.di.insert(q.value(1).toString(),
                          q.value(0).toInt());
#endif

#ifdef AT_WORK
        tmphash.id.insert(                  q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmphash.di.insert(decode->toUnicode(q.value(1).toString().toLocal8Bit()),
                                            q.value(0).toInt());
#endif
    }
    return tmphash;
}

HashDict DataBaseManager::getMapMiscTag()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    HashDict tmphash;
    QSqlQuery q = execSimpleQuery("CALL getMiscTagList()");
    while (q.next()) {
#ifndef AT_WORK
        tmphash.id.insert(q.value(0).toInt(),
                          q.value(1).toString());
        tmphash.di.insert(q.value(1).toString(),
                          q.value(0).toInt());
#endif

#ifdef AT_WORK
        tmphash.id.insert(                  q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmphash.di.insert(decode->toUnicode(q.value(1).toString().toLocal8Bit()),
                                            q.value(0).toInt());
#endif
    }
    return tmphash;
}

HashDict DataBaseManager::getMapCategory()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    HashDict tmphash;
    QSqlQuery q = execSimpleQuery("CALL getCategoryList()");
    while (q.next()) {
#ifndef AT_WORK
        tmphash.id.insert(q.value(0).toInt(),
                          q.value(1).toString());
        tmphash.di.insert(q.value(1).toString(),
                          q.value(0).toInt());
#endif

#ifdef AT_WORK
        tmphash.id.insert(                  q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmphash.di.insert(decode->toUnicode(q.value(1).toString().toLocal8Bit()),
                                            q.value(0).toInt());
#endif
    }
    return tmphash;
}

HashDict DataBaseManager::getMapGroup()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    HashDict tmphash;
    QSqlQuery q = execSimpleQuery("CALL getGroupList()");
    while (q.next()) {
#ifndef AT_WORK
        tmphash.id.insert(q.value(0).toInt(),
                          q.value(1).toString());
        tmphash.di.insert(q.value(1).toString(),
                          q.value(0).toInt());
#endif

#ifdef AT_WORK
        tmphash.id.insert(                  q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmphash.di.insert(decode->toUnicode(q.value(1).toString().toLocal8Bit()),
                                            q.value(0).toInt());
#endif
    }
    return tmphash;
}

HashDict DataBaseManager::getMapStaff()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    HashDict tmphash;
    QSqlQuery q = execSimpleQuery("CALL getStaffList()");
    while (q.next()) {
#ifndef AT_WORK
        tmphash.id.insert(q.value(0).toInt(),
                          q.value(1).toString());
        tmphash.di.insert(q.value(1).toString(),
                          q.value(0).toInt());
#endif

#ifdef AT_WORK
        tmphash.id.insert(                  q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmphash.di.insert(decode->toUnicode(q.value(1).toString().toLocal8Bit()),
                                            q.value(0).toInt());
#endif
    }
    return tmphash;
}

IdMap DataBaseManager::getMapGroupToCategory()
{
    IdMap tmpmap;

    QSqlQuery q = execSimpleQuery("CALL getGroupToCategoryMap()");

    while (q.next()) {
        tmpmap.insert(q.value(1).toInt(), q.value(0).toInt());
    }
    return tmpmap;
}

QSqlQuery DataBaseManager::getStockStats(const ReportRequest &req)
{
    // TODO: fix this shit
    if (req.projectId == 0) {
        if (req.categoryId == 0) {
            if (req.groupId == 0) {
                return execSimpleQuery("CALL getStockStat("+
                                       QString::number(req.projectId) +", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.groupId)   +", '"+
                                       req.searchString+"')");
//                return execSimpleQuery("CALL getStockStatFull('"+
//                                       req.searchString+"')");
            } else {
                return execSimpleQuery("CALL getStockStat("+
                                       QString::number(req.projectId) +", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.groupId)   +", '"+
                                       req.searchString+"')");
//                return execSimpleQuery("CALL getStockStatByGroup("+QString::number(req.groupId)+", '"+
//                                       req.searchString+"')");
            }
        } else {
            if (req.groupId == 0) {
                return execSimpleQuery("CALL getStockStat("+
                                       QString::number(req.projectId) +", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.groupId)   +", '"+
                                       req.searchString+"')");
//                return execSimpleQuery("CALL getStockStatByCategory("+QString::number(req.categoryId)+", '"+
//                                       req.searchString+"')");
            } else {
                return execSimpleQuery("CALL getStockStat("+
                                       QString::number(req.projectId) +", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.groupId)   +", '"+
                                       req.searchString+"')");
//                return execSimpleQuery("CALL getStockStatByGroup("+QString::number(req.groupId)+", '"+
//                                       req.searchString+"')");
            }
        }
    } else {
        if (req.categoryId == 0) {
            if (req.groupId == 0) {
                return execSimpleQuery("CALL getStockStat("+
                                       QString::number(req.projectId) +", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.groupId)   +", '"+
                                       req.searchString+"')");
//                return execSimpleQuery("CALL getStockStatByProject("+QString::number(req.projectId)+", '"+
//                                       req.searchString+"')");
            } else {
                return execSimpleQuery("CALL getStockStat("+
                                       QString::number(req.projectId) +", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.groupId)   +", '"+
                                       req.searchString+"')");
//                return execSimpleQuery("CALL getStockStatByGroupAndProject("+
//                                       QString::number(req.groupId)+", "+
//                                       QString::number(req.projectId)+", '"+
//                                       req.searchString+"')");
            }
        } else {
            if (req.groupId == 0) {
                return execSimpleQuery("CALL getStockStat("+
                                       QString::number(req.projectId) +", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.groupId)   +", '"+
                                       req.searchString+"')");
//                return execSimpleQuery("CALL getStockStatByCategoryAndProject("+
//                                       QString::number(req.categoryId)+", "+
//                                       QString::number(req.projectId)+", '"+
//                                       req.searchString+"')");
            } else {
                return execSimpleQuery("CALL getStockStat("+
                                       QString::number(req.projectId) +", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.groupId)   +", '"+
                                       req.searchString+"')");
//                return execSimpleQuery("CALL getStockStatByGroupAndProject("+
//                                       QString::number(req.groupId)+", "+
//                                       QString::number(req.projectId)+", '"+
//                                       req.searchString+"')");
            }
        }
    }
    return QSqlQuery();
}

QSqlQuery DataBaseManager::getTransactStats(const ReportRequest &req)
{    
    // TODO: fix this shit
    if (req.projectId == 0) {
        if (req.categoryId == 0) {
            if (req.groupId == 0) {
                return execSimpleQuery("CALL getTransactStatFull('"+
                                       req.fromDate.toString(Qt::ISODate)+"', '"+
                                       req.untilDate.toString(Qt::ISODate)+"', "+
                                       QString::number(req.flag)+", '"+
                                       req.searchString+"')");
            } else {
                return execSimpleQuery("CALL getTransactStatByGroup('"+
                                       req.fromDate.toString(Qt::ISODate)+"', '"+
                                       req.untilDate.toString(Qt::ISODate)+"', "+
                                       QString::number(req.flag)+", "+
                                       QString::number(req.groupId)+", '"+
                                       req.searchString+"')");
            }
        } else {
            if (req.groupId == 0) {
                return execSimpleQuery("CALL getTransactStatByCategory('"+
                                       req.fromDate.toString(Qt::ISODate)+"', '"+
                                       req.untilDate.toString(Qt::ISODate)+"', "+
                                       QString::number(req.flag)+", "+
                                       QString::number(req.categoryId)+", '"+
                                       req.searchString+"')");
            } else {
                return execSimpleQuery("CALL getTransactStatByGroup('"+
                                       req.fromDate.toString(Qt::ISODate)+"', '"+
                                       req.untilDate.toString(Qt::ISODate)+"', "+
                                       QString::number(req.flag)+", "+
                                       QString::number(req.groupId)+", '"+
                                       req.searchString+"')");
            }
        }
    } else {
        if (req.categoryId == 0) {
            if (req.groupId == 0) {
                return execSimpleQuery("CALL getTransactStatByProject('"+
                                       req.fromDate.toString(Qt::ISODate)+"', '"+
                                       req.untilDate.toString(Qt::ISODate)+"', "+
                                       QString::number(req.flag)+", "+
                                       QString::number(req.projectId)+", '"+
                                       req.searchString+"')");
            } else {
                return execSimpleQuery("CALL getTransactStatByGroupAndProject('"+
                                       req.fromDate.toString(Qt::ISODate)+"', '"+
                                       req.untilDate.toString(Qt::ISODate)+"', "+
                                       QString::number(req.flag)+", "+
                                       QString::number(req.groupId)+", "+
                                       QString::number(req.projectId)+", '"+
                                       req.searchString+"')");
            }
        } else {
            if (req.groupId == 0) {
                return execSimpleQuery("CALL getTransactStatByCategoryAndProject('"+
                                       req.fromDate.toString(Qt::ISODate)+"', '"+
                                       req.untilDate.toString(Qt::ISODate)+"', "+
                                       QString::number(req.flag)+", "+
                                       QString::number(req.categoryId)+", "+
                                       QString::number(req.projectId)+", '"+
                                       req.searchString+"')");
            } else {
                return execSimpleQuery("CALL getTransactStatByGroupAndProject('"+
                                       req.fromDate.toString(Qt::ISODate)+"', '"+
                                       req.untilDate.toString(Qt::ISODate)+"', "+
                                       QString::number(req.flag)+", "+
                                       QString::number(req.groupId)+", "+
                                       QString::number(req.projectId)+", '"+
                                       req.searchString+"')");
            }
        }
    }
    return QSqlQuery();
}

qint32 DataBaseManager::insertCategory(const QString &name)
{
    // TODO: FIXME
    qDebug()<<"db: insert cat:" << name;
    return 100;
}

void DataBaseManager::updateCategory(const CategoryItem &item)
{
    // TODO: FIXME
    qDebug() << "db: update cat:" << item;
}

void DataBaseManager::deleteCategory(const CategoryItem &item)
{
    // TODO: FIXME
    qDebug() << "db: delete cat:" << item;
}

qint32 DataBaseManager::insertGroup(const QString &name)
{
    // TODO: FIXME
    qDebug()<<"db: insert group:" << name;
    return 100;
}

void DataBaseManager::updateGroup(const GroupItem &item)
{
    // TODO: FIXME
    qDebug() << "db: update group:" << item;
}

void DataBaseManager::deleteGroup(const GroupItem &item)
{
    // TODO: FIXME
    qDebug() << "db: delete group:" << item;
}

qint32 DataBaseManager::insertProduct(const ProductItem &item)
{
    qDebug()<<"db: insert product:"<<item;
    return 100;
}

void DataBaseManager::updateProduct(const ProductItem &item)
{
    qDebug()<<"db update product:"<<item;
}

void DataBaseManager::deleteProduct(const ProductItem &item)
{
    qDebug()<<"db delete prodcut:"<<item;
}

qint32 DataBaseManager::insertStock(const StockItem &item)
{
    qDebug()<<"db insert stock:"<<item;
    return 100;
}

void DataBaseManager::updateStock(const StockItem &item)
{
    qDebug()<<"db update stock:"<<item;
}

void DataBaseManager::deleteStock(const StockItem &item)
{
    qDebug()<<"db delete stock:"<<item;
}

qint32 DataBaseManager::insertTransact(const TransactItem &item)
{
    qDebug()<<"db insert transact:"<<item;
    return 100;
}

void DataBaseManager::updateTransact(const TransactItem &item)
{
    qDebug()<<"db update transact:"<<item;
}

void DataBaseManager::deleteTransact(const TransactItem &item)
{
    qDebug()<<"db delete transact:"<<item;
}

void DataBaseManager::convertDB()
{
#ifdef AT_WORK
//    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
//    QTextCodec *encode = QTextCodec::codecForLocale();
#endif

#ifndef AT_WORK
#endif

#ifdef AT_WORK
#endif
// ------------ read sqlite, write mysql --------------
/*
    qDebug() << "connecting to db...";

    QSqlDatabase lite = QSqlDatabase::addDatabase("QSQLITE", "lite");

    lite.setDatabaseName("C:\\sqlite.db");
    lite.open();

    QSqlQuery lq("     SELECT `sklad`.`date`, `product`.`productname`, `sklad`.`amount`, `sklad`.`kto_vz`"
                 "       FROM `sklad`"
                 " INNER JOIN `product` ON `sklad`.`id_product` = `product`.`id`"
                 "   ORDER BY `sklad`.`date` ASC", lite);

    QVector<QString> dates;
    QVector<QString> names;
    QVector<qint32> amounts;
    QVector<QString> persons;
    while(lq.next()) {
        dates   << lq.value(0).toDate().toString(Qt::ISODate);
        names   << lq.value(1).toString();
        amounts << lq.value(2).toInt();
        persons << lq.value(3).toString();
    }

    QSqlQuery mq(" SELECT `product`.`product_id`, `product`.`product_name`"
                 "   FROM `product`");

    QSqlQuery wq;
    QMap<QString, qint32> mproduct;
    while (mq.next()) {
        mproduct.insert(decode->toUnicode(mq.value(1).toString().toLocal8Bit()),
                        mq.value(0).toInt());
    }

    for (qint32 i=0; i<dates.size(); ++i) {
        if (mproduct.contains(names.at(i))) {
            wq.prepare(" INSERT INTO `transact` "
                       " (`transact_id`, `transact_date`, `transact_diff`, `transact_note`, `transact_productRef`)"
                       " VALUES (NULL, :date, :diff, :note, :prod) ");
            wq.bindValue(":date", dates.at(i));
            wq.bindValue(":diff", amounts.at(i));
            wq.bindValue(":note", encode->toUnicode(persons.at(i).toUtf8()));
            wq.bindValue(":prod", mproduct.value(names.at(i)));
            wq.exec();
//            qDebug() << i << dates.at(i) << mproduct.value(names.at(i)) << names.at(i) << amounts.at(i);
        }
    }
    qDebug() << wq.lastError();
    qDebug() << lq.lastError();
/**/
// ---------------------------------------
// ------------ update mysql -------------
}
