#ifndef DICTMODEL_H
#define DICTMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include <linkeddict.h>
#include <databasemanager.h>

class DictModel : public QObject
{
    Q_OBJECT
public:
    LinkedDict m_dictStaff;
    LinkedDict m_dictCategory;
    LinkedDict m_dictGroup;
    LinkedDict m_dictLocation;
    LinkedDict m_dictProduct;

    DataBaseManager *m_dbman;

    explicit DictModel(DataBaseManager *dbman, QObject *parent = 0);

    void initDicts(); // throws SqlError exception

signals:

public slots:
};

#endif // DICTMODEL_H
