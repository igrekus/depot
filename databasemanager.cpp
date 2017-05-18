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
//    db.setDatabaseName("depot");
    db.setDatabaseName("wh");

//    db.setHostName("10.10.15.9");
//    db.setPort(3306);
//    db.setUserName("root");
//    db.setPassword("123456");
//    db.setDatabaseName("wh");

//    db.open();
    if (!db.open()) {
        throw db.lastError();
    }

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
                       .setName(q.value(1).toString().toLocal8Bit())
//                       .setName(decode->toUnicode(q.value(1).toString().toLocal8Bit()))
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
                       .setName(q.value(1).toString().toLocal8Bit())
//                       .setName(decode->toUnicode(q.value(1).toString().toLocal8Bit()))
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

ProductItem::ProductList DataBaseManager::getProductListByGroup(const qint32 catId, const qint32 groupId)
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
//                       .setName    (decode->toUnicode(q.value(1).toString().toLocal8Bit()))
//                       .setFullname(decode->toUnicode(q.value(2).toString().toLocal8Bit()))
//                       .setSerialn (decode->toUnicode(q.value(3).toString().toLocal8Bit()))
//                       .setUnit    (decode->toUnicode(q.value(4).toString().toLocal8Bit()))
//                       .setMiscTag (decode->toUnicode(q.value(5).toString().toLocal8Bit()))
                       .setName    (q.value(1).toString().toLocal8Bit())
                       .setFullname(q.value(2).toString().toLocal8Bit())
                       .setSerialn (q.value(3).toString().toLocal8Bit())
                       .setUnit    (q.value(4).toString().toLocal8Bit())
                       .setMiscTag (q.value(5).toString().toLocal8Bit())
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
    return execSimpleQuery("CALL getStockStat("+
                           QString::number(req.projectId) +", "+
                           QString::number(req.categoryId)+", "+
                           QString::number(req.groupId)   +", '"+
                           req.searchString+"')");
}

QSqlQuery DataBaseManager::getTransactStats(const ReportRequest &req)
{    
    return execSimpleQuery("CALL getTransactStat('"+
                           req.fromDate.toString(Qt::ISODate)+"', '"+
                           req.untilDate.toString(Qt::ISODate)+"', "+
                           QString::number(req.flag)+", "+
                           QString::number(req.projectId) +", "+
                           QString::number(req.categoryId)+", "+
                           QString::number(req.groupId)   +", '"+
                           req.searchString+"')");
}

qint32 DataBaseManager::insertCategory(const QString &name)
{
    QTextCodec *encode = QTextCodec::codecForLocale();
    QString encodedName = encode->toUnicode(name.toUtf8());
    QSqlQuery q = execSimpleQuery("CALL insertCategory('"+encodedName+"')");
    q.next();
//    qDebug() << q.value(0).toInt();
    return q.value(0).toInt();
}

void DataBaseManager::updateCategory(const CategoryItem &item)
{
    QTextCodec *encode = QTextCodec::codecForLocale();
    QString encodedName = encode->toUnicode(item.itemName.toUtf8());
    QSqlQuery q = execSimpleQuery("CALL updateCategory("+QString::number(item.itemId)+", '"
                                                        +encodedName+"')");
}

void DataBaseManager::deleteCategory(const CategoryItem &item)
{
    QSqlQuery q = execSimpleQuery("CALL deleteCategory("+QString::number(item.itemId)+")");
}

qint32 DataBaseManager::insertGroup(const GroupItem &item)
{
    QTextCodec *encode = QTextCodec::codecForLocale();
    QString encodedName = encode->toUnicode(item.itemName.toUtf8());
    QSqlQuery q = execSimpleQuery("CALL insertGroup('"+encodedName+"', "+QString::number(item.itemCategoryRef)+")");
    q.next();
//    qDebug() << q.value(0).toInt();
    return q.value(0).toInt();
}

void DataBaseManager::updateGroup(const GroupItem &item)
{
    QTextCodec *encode = QTextCodec::codecForLocale();
    QString encodedName = encode->toUnicode(item.itemName.toUtf8());
    QSqlQuery q = execSimpleQuery("CALL updateGroup("+QString::number(item.itemId)+", '"
                                                     +encodedName+"')");
}

void DataBaseManager::deleteGroup(const GroupItem &item)
{
    QSqlQuery q = execSimpleQuery("CALL deleteGroup("+QString::number(item.itemId)+")");
}

qint32 DataBaseManager::insertProduct(const ProductItem &item)
{
    QTextCodec *encode = QTextCodec::codecForLocale();

    QString encodedName     = encode->toUnicode(item.itemName.toUtf8());
    QString encodedFullname = encode->toUnicode(item.itemFullname.toUtf8());
    QString encodedSerialn  = encode->toUnicode(item.itemSerialn.toUtf8());
    QString encodedUnit     = encode->toUnicode(item.itemUnit.toUtf8());
    QString encodedMisctag  = encode->toUnicode(item.itemMiscTag.toUtf8());

    QSqlQuery q = execSimpleQuery("CALL insertProduct('"
                                  +encodedName    +"', '"
                                  +encodedFullname+"', '"
                                  +encodedSerialn +"', '"
                                  +encodedUnit    +"', '"
                                  +encodedMisctag +"', "
                                  +QString::number(item.itemGroupRef)+", "
                                  +QString::number(item.itemCategoryRef)+")");
    q.next();
//    qDebug() << q.value(0).toInt();
    return q.value(0).toInt();
}

void DataBaseManager::updateProduct(const ProductItem &item)
{
    QTextCodec *encode = QTextCodec::codecForLocale();

    QString encodedName     = encode->toUnicode(item.itemName.toUtf8());
    QString encodedFullname = encode->toUnicode(item.itemFullname.toUtf8());
    QString encodedSerialn  = encode->toUnicode(item.itemSerialn.toUtf8());
    QString encodedUnit     = encode->toUnicode(item.itemUnit.toUtf8());
    QString encodedMisctag  = encode->toUnicode(item.itemMiscTag.toUtf8());

    QSqlQuery q = execSimpleQuery("CALL updateProduct("
                                  +QString::number(item.itemId)+", '"
                                  +encodedName    +"', '"
                                  +encodedFullname+"', '"
                                  +encodedSerialn +"', '"
                                  +encodedUnit    +"', '"
                                  +encodedMisctag +"', "
                                  +QString::number(item.itemGroupRef)+", "
                                  +QString::number(item.itemCategoryRef)+")");
}

void DataBaseManager::deleteProduct(const ProductItem &item)
{
    QSqlQuery q = execSimpleQuery("CALL deleteProduct("+QString::number(item.itemId)+")");
}

qint32 DataBaseManager::insertStock(const StockItem &item)
{
    QSqlQuery q = execSimpleQuery("CALL insertStock("
                                  +QString::number(item.itemAmount)+", "
                                  +QString::number(item.itemProduct)+", "
                                  +QString::number(item.itemLocation)+", "
                                  +QString::number(item.itemProject)+")");
    q.next();
    qDebug() << q.value(0).toInt();
    return q.value(0).toInt();
}

void DataBaseManager::updateStock(const StockItem &item)
{
    QSqlQuery q = execSimpleQuery("CALL updateStock("
                                  +QString::number(item.itemId)+", "
                                  +QString::number(item.itemAmount)+", "
                                  +QString::number(item.itemProduct)+", "
                                  +QString::number(item.itemLocation)+", "
                                  +QString::number(item.itemProject)+")");
}

void DataBaseManager::deleteStock(const StockItem &item)
{
    // TODO: FIX: transact foreign key fail
    QSqlQuery q = execSimpleQuery("CALL deleteStock("+QString::number(item.itemId)+")");
}

qint32 DataBaseManager::insertTransact(const TransactItem &item)
{
    QTextCodec *encode = QTextCodec::codecForLocale();

    QString encodedNote = encode->toUnicode(item.itemNote.toUtf8());

    QSqlQuery q = execSimpleQuery("CALL insertTransact('"
                                  +item.itemDate.toString(Qt::ISODate)+"', "
                                  +QString::number(item.itemDiff)+", '"
                                  +encodedNote+"', "
                                  +QString::number(item.itemStockRef)+", "
                                  +QString::number(item.itemStaffRef)+", "
                                  +QString::number(item.itemProjectRef)+", "
                                  +QString::number(item.itemBillRef)+")");
    q.next();
//    qDebug() << q.value(0).toInt();
    return q.value(0).toInt();
}

void DataBaseManager::updateTransact(const TransactItem &item)
{
    QTextCodec *encode = QTextCodec::codecForLocale();

    QString encodedNote = encode->toUnicode(item.itemNote.toUtf8());

    QSqlQuery q = execSimpleQuery("CALL updateTransact("
                                  +QString::number(item.itemId)+", '"
                                  +item.itemDate.toString(Qt::ISODate)+"', "
                                  +QString::number(item.itemDiff)+", '"
                                  +encodedNote+"', "
                                  +QString::number(item.itemStockRef)+", "
                                  +QString::number(item.itemStaffRef)+", "
                                  +QString::number(item.itemProjectRef)+", "
                                  +QString::number(item.itemBillRef)+")");
}

void DataBaseManager::deleteTransact(const TransactItem &item)
{
    QSqlQuery q = execSimpleQuery("CALL deleteTransact("+QString::number(item.itemId)+")");
}

TransactItem DataBaseManager::makeTransactItemFromStockItem(const StockItem &stock)
{
    return (TransactItem::TransactItemBuilder()
//            .setId(0)                           // new transact
            .setName(stock.itemName)
            .setDate(QDate::currentDate())
//            .setDiff(0)
//            .setNote(QString())
            .setStock(stock.itemId)               // ref to appropriate stock item
//            .setStaff(0)
            .setProject(stock.itemProject)
//            .setBill(0)
            .build());
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
