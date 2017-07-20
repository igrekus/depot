#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T13:26:26
#
#-------------------------------------------------

include(c:/devtools/!lib/qtxlsx/src/xlsx/qtxlsx.pri)

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
    abstracttreeitem.cpp \
    transactdatadialog.cpp \
    stockdatadialog.cpp \
    idstringmodel.cpp \
    reportmanager.cpp \
    reportrequest.cpp \
    recursivefilterproxymodel.cpp \
    projectrecursivefilterproxymodel.cpp \
    decoderproxymodel.cpp \
    delegatehighligtabletabletext.cpp \
    delegatehighligtabltreetext.cpp \
    dicteditordialog.cpp \
    classitem.cpp \
    transactrecursivefilterproxymodel.cpp \
    decoratorproxymodel.cpp

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
    abstracttreeitem.h \
    constants.h \
    transactdatadialog.h \
    stockdatadialog.h \
    idstringmodel.h \
    reportmanager.h \
    reportrequest.h \
    recursivefilterproxymodel.h \
    projectrecursivefilterproxymodel.h \
    decoderproxymodel.h \
    delegatehighligtabletabletext.h \
    delegatehighligtabltreetext.h \
    dicteditordialog.h \
    classitem.h \
    transactrecursivefilterproxymodel.h \
    decoratorproxymodel.h

FORMS    += mainwindow.ui \
    inventorydialog.ui \
    inventorydatadialog.ui \
    transactdatadialog.ui \
    stockdatadialog.ui \
    reportmanager.ui \
    dicteditordialog.ui

RESOURCES += \
    resource.qrc
