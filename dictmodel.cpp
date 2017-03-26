#include "dictmodel.h"

DictModel::DictModel(DataBaseManager *dbman, QObject *parent) : QObject(parent)
{
    m_dbman = dbman;
}

void DictModel::initDicts()
{
    qDebug()<< "loading dicts from db";
}
