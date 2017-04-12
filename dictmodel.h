#ifndef DICTMODEL_H
#define DICTMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include <linkeddict.h>
#include <databasemanager.h>
#include <hashdict.h>

class DictModel : public QObject
{
    Q_OBJECT
public:

    LinkedDict m_dictCategory;
    LinkedDict m_dictGroup;
    LinkedDict m_dictProduct;

    HashDict m_mapCategory;    // списко категорий
    HashDict m_mapLocation;    // список мест хранения на складе
    HashDict m_mapProject;  // теги проектов
//    HashDict m_mapMisc;        // разные теги

    DataBaseManager *m_dbman;

    explicit DictModel(DataBaseManager *dbman, QObject *parent = 0);

    void initModel();       // throws SqlError exception

signals:

public slots:
};

#endif // DICTMODEL_H
