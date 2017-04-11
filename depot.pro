#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T13:26:26
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = depot
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    linkeddict.cpp \
    databasemanager.cpp \
    stockmodel.cpp \
    dictmodel.cpp \
    productitem.cpp \
    stockitem.cpp \
    categoryitem.cpp \
    abstractitem.cpp \
    groupitem.cpp \
    mapmodel.cpp \
    hashdict.cpp \
    transactitem.cpp \
    transactmodel.cpp \
    inventorydialog.cpp \
    inventorydatadialog.cpp \
    inventorymodel.cpp \
    inventoryitem.cpp \
    abstracttreeitem.cpp

HEADERS  += mainwindow.h \
    linkeddict.h \
    databasemanager.h \
    stockmodel.h \
    dictmodel.h \
    productitem.h \
    stockitem.h \
    categoryitem.h \
    abstractitem.h \
    groupitem.h \
    mapmodel.h \
    hashdict.h \
    transactitem.h \
    transactmodel.h \
    inventorydialog.h \
    inventorydatadialog.h \
    inventorymodel.h \
    inventoryitem.h \
    abstracttreeitem.h

FORMS    += mainwindow.ui \
    inventorydialog.ui \
    inventorydatadialog.ui

RESOURCES += \
    resource.qrc
