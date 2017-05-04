#include "dictmodel.h"

DictModel::DictModel(DataBaseManager *dbman, QObject *parent) : QObject(parent)
{
    m_dbman = dbman;

    m_categoryListModel = new MapModel(this);
    m_projectListModel  = new MapModel(this);
    m_locationListModel = new MapModel(this);
    m_groupListModel    = new MapModel(this);
    m_staffListModel    = new MapModel(this);

    m_productListModel  = new IdStringModel(this);
}

void DictModel::initModel()
{
    qDebug()<< "loading dicts from db";
//    m_mapLocation = m_dbman->getMapLocation();
//    m_mapProject  = m_dbman->getMapProject();
////    m_mapMisc     = m_dbman->getMapMiscTag();
//    m_mapCategory = m_dbman->getMapCategory();
//    m_mapGroup    = m_dbman->getMapGroup();
//    m_mapStaff    = m_dbman->getMapStaff();

    m_mapGroupToCategory = m_dbman->getMapGroupToCategory();

    m_categoryListModel->initModel(m_dbman->getMapCategory());
    m_categoryListModel->addItemAtPosition(0, 0, "Все");
    m_projectListModel->initModel(m_dbman->getMapProject());
    m_projectListModel->addItemAtPosition(0, 0, "Все");
    m_locationListModel->initModel(m_dbman->getMapLocation());
    m_groupListModel->initModel(m_dbman->getMapGroup());
    m_groupListModel->addItemAtPosition(0, 0, "Все");
    m_staffListModel->initModel(m_dbman->getMapStaff());

    m_productListModel->setData(m_dbman->getIdProductList());
}
