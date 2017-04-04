#include "dictmodel.h"

DictModel::DictModel(DataBaseManager *dbman, QObject *parent) : QObject(parent)
{
    m_dbman = dbman;
}

void DictModel::initModel()
{
    qDebug()<< "loading dicts from db";
    m_mapLocation = m_dbman->getMapLocation();
    m_mapProjectTag  = m_dbman->getMapProjectTag();
//    m_mapMisc     = m_dbman->getMapMiscTag();
    m_mapCategory = m_dbman->getMapCategory();
}
