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
    CategoryItem::CategoryItemBuilder b;

    QSqlQuery q = execSimpleQuery("CALL getCategoryList()");

    while (q.next()) {
#ifndef AT_WORK
        b.setId  (q.value(0).toInt());
        b.setName(q.value(1).toString());
#endif

#ifdef AT_WORK
        b.setId  (q.value(0).toInt());
        b.setName(decode->toUnicode(q.value(1).toString().toLocal8Bit()));
#endif

        tmplist.append(b.build());
    }
    return tmplist;
}


GroupItem::GroupList DataBaseManager::getGroupList(const qint32 catId)
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif
    GroupItem::GroupList tmplist;
    GroupItem::GroupItemBuilder b;
    QVariantList params;

    params.append(QVariant(catId));

//    QSqlQuery q = execParametrizedQuery("CALL getGroupListByCategory(?)", params);
    QSqlQuery q = execSimpleQuery("CALL getGroupListByCategory("+QString::number(catId)+");");

    while (q.next()) {
#ifndef AT_WORK
        b.setId  (q.value(0).toInt());
        b.setName(q.value(1).toString());
#endif

#ifdef AT_WORK
        b.setId  (q.value(0).toInt());
        b.setName(decode->toUnicode(q.value(1).toString().toLocal8Bit()));
#endif

        tmplist.append(b.build());
    }
    return tmplist;
}

StockItem::StockList DataBaseManager::getStockList(const qint32 catId, const qint32 groupId)
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    StockItem::StockList tmplist;
    StockItem::StockItemBuilder b;

    // TODO: неправильный запрос, запрашивать сток
    QSqlQuery q = execSimpleQuery("CALL getStockByCategoryAndGroup("+QString::number(catId)+", "+QString::number(groupId)+")");

    while (q.next()) {

#ifndef AT_WORK
        b.setId       (q.value(0).toInt());
        b.setName     (q.value(1).toString());
        b.setType     (StockItem::ItemProduct);
        b.setLevel    (StockItem::Level_2);
        b.setAmount   (q.value(2).toInt());
        b.setSerialn  (q.value(3).toString());
        b.setProject  (q.value(4).toString());
        b.setLocation (q.value(5).toString());

#endif

#ifdef AT_WORK
        b.setId         (q.value(0).toInt());
        b.setName       (decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        b.setType       (StockItem::ItemStock);
        b.setLevel      (StockItem::Level_2);
        b.setAmount     (q.value(2).toInt());
        b.setSerialn    (decode->toUnicode(q.value(3).toString().toLocal8Bit()));
        b.setProject    (decode->toUnicode(q.value(4).toString().toLocal8Bit()));
        b.setLocation   (decode->toUnicode(q.value(5).toString().toLocal8Bit()));
#endif

        tmplist.append(b.build());
    }
    return tmplist;
}

TransactItem::TransactList DataBaseManager::getTransactList()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    TransactItem::TransactList tmplist;
    TransactItem::TransactItemBuilder b;

    // TODO: неправильный запрос, запрашивать сток
//    QSqlQuery q = execSimpleQuery("CALL getTransactList1k()");
    QSqlQuery q = execSimpleQuery("CALL getTransactListFull()");

    while (q.next()) {
#ifndef AT_WORK
        b.setId     (q.value(0).toInt());
        b.setDate   (q.value(1).toDate());
        b.setDiff   (q.value(2).toInt());
        b.setNote   (q.value(3).toString());
        b.setStaff  (q.value(4).toString());
        b.setName   (q.value(5).toString());
        b.setBillRef(q.value(6).toInt());
#endif

#ifdef AT_WORK
        b.setId     (q.value(0).toInt());
        b.setDate   (q.value(1).toDate());
        b.setDiff   (q.value(2).toInt());
        b.setNote   (decode->toUnicode(q.value(3).toString().toLocal8Bit()));
        b.setStaff  (decode->toUnicode(q.value(4).toString().toLocal8Bit()));
        b.setName   (decode->toUnicode(q.value(5).toString().toLocal8Bit()));
        b.setBillRef(q.value(6).toInt());
#endif

        tmplist.append(b.build());
    }
    return tmplist;
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
        tmphash.id.insert(q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmphash.di.insert(decode->toUnicode(q.value(1).toString().toLocal8Bit()),
                          q.value(0).toInt());
#endif
    }
    return tmphash;
}

HashDict DataBaseManager::getMapProjectTag()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
#endif

    HashDict tmphash;
    QSqlQuery q = execSimpleQuery("CALL getProjectTagList()");
    while (q.next()) {
#ifndef AT_WORK
        tmphash.id.insert(q.value(0).toInt(),
                          q.value(1).toString());
        tmphash.di.insert(q.value(1).toString(),
                          q.value(0).toInt());
#endif

#ifdef AT_WORK
        tmphash.id.insert(q.value(0).toInt(),
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
        tmphash.id.insert(q.value(0).toInt(),
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
        tmphash.id.insert(q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmphash.di.insert(decode->toUnicode(q.value(1).toString().toLocal8Bit()),
                          q.value(0).toInt());
#endif
    }
    return tmphash;
}

void DataBaseManager::convertDB()
{
#ifdef AT_WORK
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");
    QTextCodec *encode = QTextCodec::codecForLocale();
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
