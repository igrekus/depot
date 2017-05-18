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
#include <idstringmodel.h>

class DictModel : public QObject
{
    Q_OBJECT
public:

    IdMap m_mapGroupToCategory;

    MapModel *m_categoryListModel;
    MapModel *m_projectListModel;
    MapModel *m_groupListModel;
    MapModel *m_locationListModel;
    MapModel *m_staffListModel;
//    MapModel *m_miscTagListModel;

    IdStringModel *m_productListModel;

    DataBaseManager *m_dbman;

    explicit DictModel(DataBaseManager *dbman, QObject *parent = nullptr);

    void initModel();       // SqlError exception

    // обновление словарей
    // TODO: FIX переделать на вставку/изменение/удаление поэлементно, если будет слишком большая нагрузка на БД
    void updateMapGroupToCategory();
    void updateCategoryList();
    void updateProjectList();
    void updateGroupList();
    void updateProductList();
    void updateLocationList();
    void updateStaffList();

signals:

public slots:
};

#endif // DICTMODEL_H
