#include "databasemanager.h"

//#define AT_WORK

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

        tmplist.append(b.buildCategoryItem());
    }
    return tmplist;
}


GroupItem::GroupList DataBaseManager::getGroupList(qint32 catId)
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

        tmplist.append(b.buildGroupItem());
    }
    return tmplist;
}

StockItem::StockList DataBaseManager::getStockList(qint32 catId, qint32 groupId)
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
        b.setId         (q.value(0).toInt());
        b.setName       (q.value(1).toString());
        b.setType       (StockItem::ItemProduct);
        b.setLevel      (StockItem::Level_2);
        b.setAmount     (q.value(2).toInt());
        b.setSerialn    (q.value(3).toString());
        b.setProjectTag (q.value(4).toString());
        b.setLocationRef(q.value(4).toInt());
#endif

#ifdef AT_WORK
        b.setId         (q.value(0).toInt());
        b.setName       (decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        b.setType       (StockItem::ItemProduct);
        b.setLevel      (StockItem::Level_2);
        b.setAmount     (q.value(2).toInt());
        b.setSerialn    (decode->toUnicode(q.value(3).toString().toLocal8Bit()));
        b.setProjectTag (decode->toUnicode(q.value(4).toString().toLocal8Bit()));
        b.setLocationRef(q.value(4).toInt());
#endif

        tmplist.append(b.buildStockItem());
    }
    return tmplist;
}
