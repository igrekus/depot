#include "databasemanager.h"

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

CategoryItem::CategoryList DataBaseManager::getCategoryList()
{
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");

    CategoryItem::CategoryList tmplist;

    QSqlQuery q = execSimpleQuery("SELECT `category_id`, `category_name` FROM `category` WHERE `category_id`<>0");

    while (q.next()) {
//        CategoryItem tmpitem(q.value(0).toInt(),
//                             q.value(1).toString());
        CategoryItem tmpitem(q.value(0).toInt(),
                             decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmplist.append(tmpitem);
    }
    return tmplist;
}


GroupItem::GroupList DataBaseManager::getGroupList(qint32 catId)
{
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");

    GroupItem::GroupList tmplist;

    QSqlQuery q = execSimpleQuery("SELECT `group_id`, `group_name` "
                                  "  FROM `group` "
                                  " WHERE `group_id`<>0 AND `group_categoryRef`="+QString::number(catId));

    while (q.next()) {
//        GroupItem tmpitem(q.value(0).toInt(),
//                          q.value(1).toString());
        GroupItem tmpitem(q.value(0).toInt(),
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()));
        tmplist.append(tmpitem);
    }
    return tmplist;
}

StockItem::StockList DataBaseManager::getStockList(qint32 catId, qint32 groupId)
{
    QTextCodec *decode = QTextCodec::codecForName("UTF-8");

    StockItem::StockList tmplist;

    // TODO: неправильный запрос, запрашивать сток
    QSqlQuery q = execSimpleQuery("CALL getStockByCategoryAndGroup("+QString::number(catId)+", "+QString::number(groupId)+")");

    while (q.next()) {
//        StockItem tmpitem(StockItem::ItemProduct,
//                          StockItem::Level_2,
//                          q.value(0).toInt(),     // id
//                          q.value(1).toString(),  // name
//                          q.value(2).toInt(),     // amount
//                          q.value(3).toString(),  // serialn
//                          q.value(4).toString()); // location

        StockItem tmpitem(StockItem::ItemProduct,
                          StockItem::Level_2,
                          q.value(0).toInt(),     // id
                          decode->toUnicode(q.value(1).toString().toLocal8Bit()),  // name
                          q.value(2).toInt(),     // amount
                          decode->toUnicode(q.value(3).toString().toLocal8Bit()),  // serialn
                          decode->toUnicode(q.value(4).toString().toLocal8Bit())); // location

        tmplist.append(tmpitem);
    }
    return tmplist;
}
