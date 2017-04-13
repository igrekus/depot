#include "dictmodel.h"

DictModel::DictModel(DataBaseManager *dbman, QObject *parent) : QObject(parent)
{
    m_dbman = dbman;
}

void DictModel::initModel()
{
    qDebug()<< "loading dicts from db";
    m_mapLocation = m_dbman->getMapLocation();
    m_mapProject  = m_dbman->getMapProject();
//    m_mapMisc     = m_dbman->getMapMiscTag();
    m_mapCategory = m_dbman->getMapCategory();
    m_mapGroup    = m_dbman->getMapGroup();

    m_mapGroupToCategory = m_dbman->getMapGroupToCategory();
}
