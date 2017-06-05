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
//    if (!query.lastQuery().contains("get")) {
//        QMessageBox::information(nullptr, QString("info"), "db message: "+query.lastQuery()+" "+query.lastError().text());
//    }
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

ClassItem::ClassList DataBaseManager::getClassList()
{
    // TODO: refactor with functors
    ClassItem::ClassList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getClassList()");
    while (q.next()) {
        tmplist.append(ClassItem::ClassItemBuilder()
                       .setId  (q.value(0).toInt())
                       .setName(q.value(1).toString().toLocal8Bit())
                       .build());
    }
    return tmplist;
}

CategoryItem::CategoryList DataBaseManager::getCategoryList(const qint32 classId)
{
    CategoryItem::CategoryList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getCategoryListByClass("+QString::number(classId)+")");
    while (q.next()) {
        tmplist.append(CategoryItem::CategoryItemBuilder()
                       .setId   (q.value(0).toInt())
                       .setName (q.value(1).toString().toLocal8Bit())
                       .build());
    }
    return tmplist;
}

GroupItem::GroupList DataBaseManager::getGroupList(const qint32 catId)
{
    GroupItem::GroupList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getGroupListByCategory("+QString::number(catId)+");");
    while (q.next()) {
        tmplist.append(GroupItem::GroupItemBuilder()
                       .setId      (q.value(0).toInt())
                       .setName    (q.value(1).toString().toLocal8Bit())
                       .build());
    }
    return tmplist;
}

StockItem::StockList DataBaseManager::getStockList(const qint32 groupId)
{
    StockItem::StockList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getStockByGroup("+QString::number(groupId)+")");
    while (q.next()) {
        tmplist.append(StockItem::StockItemBuilder()
                       .setId      (q.value(0).toInt())
                       .setName    (q.value(1).toString().toLocal8Bit())
                       .setType    (Constants::ItemItem)
                       .setLevel   (Constants::Level_3)
                       .setAmount  (q.value(2).toInt())
                       .setSerialn (q.value(3).toString().toLocal8Bit())
                       .setProject (q.value(4).toInt())
                       .setLocation(q.value(5).toInt())
                       .setProduct (q.value(6).toInt())
                       .setFullname(q.value(7).toString().toLocal8Bit())
                       .setUnit    (q.value(8).toString().toLocal8Bit())
                       .build());
    }
    return tmplist;
}

ProductItem::ProductList DataBaseManager::getProductListByGroup(const qint32 groupId)
{
    ProductItem::ProductList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getProductByGroup("+QString::number(groupId)+")");
    while (q.next()) {
        tmplist.append(ProductItem::ProductItemBuilder()
                       .setId      (q.value(0).toInt())
                       .setName    (q.value(1).toString().toLocal8Bit())
                       .setFullname(q.value(2).toString().toLocal8Bit())
                       .setSerialn (q.value(3).toString().toLocal8Bit())
                       .setUnit    (q.value(4).toString().toLocal8Bit())
                       .setMiscTag (q.value(5).toString().toLocal8Bit())
                       .build());
    }
    return tmplist;
}

TransactItem::TransactList DataBaseManager::getTransactList()
{
    TransactItem::TransactList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getTransactListFull()");

    while (q.next()) {
        tmplist.append(TransactItem::TransactItemBuilder()
                       .setId     (q.value(0).toInt())
                       .setDate   (q.value(1).toDate())
                       .setDiff   (q.value(2).toInt())
                       .setNote   (q.value(3).toString().toLocal8Bit())
                       .setStaff  (q.value(4).toInt())
                       .setName   (q.value(5).toString().toLocal8Bit())
                       .setProject(q.value(6).toInt())
                       .setStock  (q.value(7).toInt())
                       .setBill   (q.value(8).toInt())
                       .build());
    }
    return tmplist;
}

IdStringList DataBaseManager::getIdProductList()
{
    IdStringList tmplist;
    QSqlQuery q = execSimpleQuery("CALL getIdProductList()");
    while (q.next()) {
        QPair<qint32, QString> tmppair;
        tmppair.first  = q.value(0).toInt();
        tmppair.second = q.value(1).toString().toLocal8Bit();
        tmplist.append(tmppair);
    }
    return tmplist;
}

StockItem DataBaseManager::getStockByProductId(const qint32 prodId)
{
    QSqlQuery q = execSimpleQuery("CALL getStockByProductId("+QString::number(prodId)+")");

    if (!q.next()) {
        return StockItem::StockItemBuilder().build();
    }

    return StockItem::StockItemBuilder()
           .setId         (q.value(0).toInt())
           .setName       (q.value(1).toString().toLocal8Bit())
           .setType       (Constants::ItemItem)
           .setLevel      (Constants::Level_3)
           .setAmount     (q.value(2).toInt())
           .setSerialn    (q.value(3).toString().toLocal8Bit())
           .setProject    (q.value(4).toString().toInt())
           .setLocation   (q.value(5).toString().toInt())
           .setProduct    (q.value(6).toString().toInt())
           .build();
}

ProductRelation DataBaseManager::getProductParents(const qint32 prodId)
{
    QSqlQuery q = execSimpleQuery("CALL getProductParents("+QString::number(prodId)+")");

    ProductRelation p;
    if (!q.next()) {
        return p;
    }
    p.parentClass    = q.value(0).toInt();
    p.parentCategory = q.value(1).toInt();
    p.parentGroup    = q.value(2).toInt();
    return p;
}

HashDict DataBaseManager::fillHashDict(QSqlQuery &&q) {
    HashDict tmphash;
    while (q.next()) {
        tmphash.id.insert(q.value(0).toInt(),
                          q.value(1).toString().toLocal8Bit());
        tmphash.di.insert(q.value(1).toString().toLocal8Bit(),
                          q.value(0).toInt());
    }
    return tmphash;
}

HashDict DataBaseManager::getMapLocation()
{
    return fillHashDict(std::move(execSimpleQuery("CALL getLocationList()")));
}

HashDict DataBaseManager::getMapProject()
{
    return fillHashDict(std::move(execSimpleQuery("CALL getProjectList()")));
}

HashDict DataBaseManager::getMapMiscTag()
{
    return fillHashDict(std::move(execSimpleQuery("CALL getMiscTagList()")));
}

HashDict DataBaseManager::getMapCategory()
{
    // TODO: move to getListX method?
    return fillHashDict(std::move(execSimpleQuery("CALL getCategoryList()")));
}

HashDict DataBaseManager::getMapGroup()
{
    // TODO: move to getListX method?
    return fillHashDict(std::move(execSimpleQuery("CALL getGroupList()")));
}

HashDict DataBaseManager::getMapStaff()
{
    return fillHashDict(std::move(execSimpleQuery("CALL getStaffList()")));
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

qint32 DataBaseManager::insertCategory(const qint32 classId, const QString &name)
{
    QTextCodec *encode = QTextCodec::codecForLocale();
    QString encodedName = encode->toUnicode(name.toUtf8());
    QSqlQuery q = execSimpleQuery("CALL insertCategory("+QString::number(classId)+", '"+encodedName+"')");
    q.next();
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

qint32 DataBaseManager::insertGroup(const qint32 catId, const QString &grpName)
{
    QTextCodec *encode = QTextCodec::codecForLocale();
    QString encodedName = encode->toUnicode(grpName.toUtf8());
    QSqlQuery q = execSimpleQuery("CALL insertGroup("+QString::number(catId)+", '"+encodedName+"')");
    q.next();
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

qint32 DataBaseManager::insertProduct(const ProductRelation &relation, const ProductItem &item)
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
                                  +QString::number(relation.parentGroup)+", "
                                  +QString::number(relation.parentCategory)+")");
    q.next();
    return q.value(0).toInt();
}

void DataBaseManager::updateProduct(const ProductRelation &relation, const ProductItem &item)
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
                                  +QString::number(relation.parentGroup)+", "
                                  +QString::number(relation.parentCategory)+")");
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

qint32 DataBaseManager::insertDictRecord(const qint32 &tableId, const QString &name)
{
    QTextCodec *encode = QTextCodec::codecForLocale();
    QString encodedName = encode->toUnicode(name.toUtf8());
    QSqlQuery q;
    // TODO: FIXME refactor this crap
    switch (tableId) {
    case 0:
        q = execSimpleQuery("CALL insertLocation('"+encodedName+"')");
        break;
    case 1:
        q = execSimpleQuery("CALL insertStaff('"+encodedName+"')");
        break;
    case 2:
        q = execSimpleQuery("CALL insertProject('"+encodedName+"')");
        break;
    }
    q.next();
    return q.value(0).toInt();
}

void DataBaseManager::updateDictRecord(const qint32 &tableId, const qint32 recId, const QString &name)
{
    QTextCodec *encode = QTextCodec::codecForLocale();
    QString encodedName = encode->toUnicode(name.toUtf8());
    QSqlQuery q;
    switch (tableId) {
    case 0:
        q = execSimpleQuery("CALL updateLocation("+QString::number(recId)+", '"
                                                  +encodedName+"')");
        break;
    case 1:
        q = execSimpleQuery("CALL updateStaff("+QString::number(recId)+", '"
                                               +encodedName+"')");
        break;
    case 2:
        q = execSimpleQuery("CALL updateProject("+QString::number(recId)+", '"
                                                 +encodedName+"')");
        break;
    }
}

void DataBaseManager::deleteDictRecord(const qint32 &tableId, const qint32 recId)
{
    // TODO: refactor with functors
    QSqlQuery q;
    switch (tableId) {
    case 0:
        q = execSimpleQuery("CALL deleteLocation("+QString::number(recId)+")");
        break;
    case 1:
        q = execSimpleQuery("CALL deleteStaff("+QString::number(recId)+")");
        break;
    case 2:
        q = execSimpleQuery("CALL deleteProject("+QString::number(recId)+")");
        break;
    }
}

bool DataBaseManager::checkLocationFk(const qint32 locId)
{
    QSqlQuery q = execSimpleQuery("CALL checkLocationStockFk("+QString::number(locId)+")");
    q.next();
    return q.value(0).toBool();
}

bool DataBaseManager::checkStaffFk(const qint32 staffId)
{
    QSqlQuery q = execSimpleQuery("CALL checkStaffTransactFk("+QString::number(staffId)+")");
    q.next();
    return q.value(0).toBool();
}

bool DataBaseManager::checkProjectFk(const qint32 projId)
{
    QSqlQuery q = execSimpleQuery("CALL checkProjectStockTransactFk("+QString::number(projId)+")");
    q.next();
    return q.value(0).toBool();
}

// ------------------------------- utility -----------------------------------
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
