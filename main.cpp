#include "mainwindow.h"
#include <QApplication>

#include <QDebug>

#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyle("plastique");

    MainWindow w;
    w.show();

    try {
        w.initApplication();
    }
    catch (QSqlError err) {
        qDebug() << "DB connection error:" << err;
        QMessageBox::warning(&w, "Ошибка!", "Ошибка подключения к БД: "+err.text());
        return 1;
    }

    return a.exec();
}
