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
#include <constants.h>
#include <mapmodel.h>

class DictModel : public QObject
{
    Q_OBJECT
public:

//    LinkedDict m_dictCategory;
//    LinkedDict m_dictGroup;
//    LinkedDict m_dictProduct;

//    HashDict m_mapCategory;    // список категорий
//    HashDict m_mapGroup;       // групп
//    HashDict m_mapLocation;    // мест хранения на складе
//    HashDict m_mapProject;     // проектов
//    HashDict m_mapStaff;       // сотрудников

    IdMap m_mapGroupToCategory;

    MapModel *m_categoryListModel;
    MapModel *m_projectListModel;
    MapModel *m_locationListModel;
    MapModel *m_groupListModel;
    MapModel *m_staffListModel;
//    MapModel *m_miscTagListModel;

//    HashDict m_mapMisc;        // разные теги

    DataBaseManager *m_dbman;

    explicit DictModel(DataBaseManager *dbman, QObject *parent = 0);

    void initModel();       // throws SqlError exception

signals:

public slots:
};

#endif // DICTMODEL_H
