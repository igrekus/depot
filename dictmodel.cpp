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

    updateMapGroupToCategory();
    updateCategoryList();
    updateProjectList();
    updateGroupList();
    updateProductList();
    updateLocationList();
    updateStaffList();
}

void DictModel::updateMapGroupToCategory() {
    if (!m_mapGroupToCategory.isEmpty()) {
        m_mapGroupToCategory.clear();
    }
    m_mapGroupToCategory = m_dbman->getMapGroupToCategory();
}

void DictModel::updateCategoryList()
{
    if (!m_categoryListModel->isEmpty()) {
        m_categoryListModel->clear();
    }
    m_categoryListModel->initModel(m_dbman->getMapCategory());
    m_categoryListModel->addItemAtPosition(0, 0, "Все");
}

void DictModel::updateProjectList()
{
    if (!m_projectListModel->isEmpty()) {
        m_projectListModel->clear();
    }
    m_projectListModel->initModel(m_dbman->getMapProject());
    m_projectListModel->addItemAtPosition(0, 0, "Все");

}

void DictModel::updateGroupList()
{
    if (!m_groupListModel->isEmpty()) {
        m_groupListModel->clear();
    }
    m_groupListModel->initModel(m_dbman->getMapGroup());
    m_groupListModel->addItemAtPosition(0, 0, "Все");
}

void DictModel::updateProductList()
{
    if (!m_productListModel->isEmpty()) {
        m_productListModel->clear();
    }
    m_productListModel->setData(m_dbman->getIdProductList());
}

void DictModel::updateLocationList()
{
    if (!m_locationListModel->isEmpty()) {
        m_locationListModel->clear();
    }
    m_locationListModel->initModel(m_dbman->getMapLocation());
}

void DictModel::updateStaffList()
{
    if (!m_staffListModel->isEmpty()) {
        m_staffListModel->clear();
    }
    m_staffListModel->initModel(m_dbman->getMapStaff());
}
