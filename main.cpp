#include "mainwindow.h"
#include <QApplication>

#include <QDebug>

#include <QMessageBox>

//#include <QSqlDatabase>
#include <QSqlError>
//#include <QSqlQuery>
#include <QFileDevice>

#define XLSX_NO_LIB

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyle("plastique");

    MainWindow w;
    w.showMaximized();

    try {
        w.initApplication();
    }
    catch (QSqlError err) {
        qDebug() << "DB connection error:" << err;
        QMessageBox::warning(&w, "Ошибка!", "\n           DB connection error: "+err.text());
        return 1;
    }
    catch (QFileDevice::FileError err) {
        qDebug() << "Settings.ini read error:" << err;
        QMessageBox::warning(&w, "Ошибка!", "\n           Settings.ini read error: "+(int)err);
        return 2;
    }

    return a.exec();
}
