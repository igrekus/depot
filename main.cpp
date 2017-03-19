#include "mainwindow.h"
#include <QApplication>

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "mysql");
//    db.setHostName("localhost");
//    db.setPort(3306);
//    db.setUserName("root");
//    db.setPassword("");
//    db.setDatabaseName("depot");
//    if (!db.open()) {
//        QSqlError err = db.lastError();
//        qDebug() << "db connection error: "+err.text();
//    }

//    QSqlDatabase sqlite = QSqlDatabase::addDatabase("QSQLITE", "sqlite");
//    sqlite.setDatabaseName("c:/sqlite.db");
//    if (!sqlite.open()) {
//        QSqlError err = sqlite.lastError();
//        qDebug() << "db err:"+err.text();
//    }



//    QSqlQuery q1("select `location_id`, `location_name` from `location` where `location_id`<>0", db);
//    QMap<QString, int> loc_id;
//    while (q1.next()) {
//        loc_id.insert(q1.value(1).toString(), q1.value(0).toInt());
//    }

//    QSqlQuery q3("select `product_id`, `product_name` from `product` where `product_id`<>0", db);
//    QHash<QString, int> prod_id;
//    while (q3.next()) {
//        prod_id.insert(q3.value(1).toString(), q3.value(0).toInt());
//    }

//    QStringList list;
//    QStringList store;
//    QVector<int> amt;
//    QSqlQuery q2("select `productname`, `gde_hran`, "
//                 "((SELECT TOTAL(sklad.amount) FROM sklad WHERE sklad.id_product=product.id) - "
//                 "(SELECT TOTAL(order_details.amount) FROM order_details WHERE order_details.id_product=product.id)) AS cnt"
//                 " from `product`", sqlite);
//    while (q2.next()) {
////        QString s = q2.value(0).toString();
////        list << s.left(s.size() -(s.size() - s.indexOf("(")));
//        list << q2.value(0).toString();
//        store << q2.value(1).toString();
//        amt << q2.value(2).toInt();
//    }

//    for (QString &it : store) {
//        it.replace("A", "А");
//        it.replace("B", "В");
//        it.replace("E", "Е");
//    }

//    QSqlQuery qw(db);

//    for (int i=0; i<list.size(); ++i) {
//        qw.prepare("insert into stock (stock_id, stock_product, stock_amount, stock_location) "
//                   "values (null, :prod, :amt, :loc)");
//        qw.bindValue(":prod", prod_id.value(list.at(i)));
//        qw.bindValue(":amt",  amt.at(i));
//        qw.bindValue(":loc",  loc_id.value(store.at(i)));
//        qDebug() << qw.boundValue(":prod") << qw.boundValue(":amt") << qw.boundValue(":loc");
//        if (qw.boundValue(":prod") != 0)
//            qw.exec();
//    }
//    qDebug()<<qw.lastError();

    return a.exec();
}
