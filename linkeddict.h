#ifndef LINKEDDICT_H
#define LINKEDDICT_H

#include <QObject>
#include <QDebug>

#include <productitem.h>

class LinkedDict : public QObject
{
    Q_OBJECT
public:

    using DictIdList    = QVector<qint32>;
    using DictMapList   = QVector<DictIdList>;
    using DictChildList = QVector<LinkedDict *>;

    // data members
    DictIdList    m_dictData;             // список id словаря
    DictChildList m_dictChildList;        // список указателей на словари-потомки
    DictMapList   m_dictMapList;          // список зависимостей parent id <- [childid... n]
    LinkedDict   *m_dictParent;           // указатель на словарь-предок

    explicit LinkedDict(QObject *parent = 0):
        QObject(parent),
        m_dictData(),
        m_dictChildList(),
        m_dictMapList(),
        m_dictParent(nullptr)
    {}

    explicit LinkedDict(const LinkedDict &copy, QObject *parent = 0):
        QObject(parent),
        m_dictData     (copy.m_dictData),
        m_dictChildList(copy.m_dictChildList),
        m_dictMapList  (copy.m_dictMapList),
        m_dictParent   (copy.m_dictParent)
    {}

    LinkedDict &operator=(const LinkedDict &right) {
        if (this != &right) {
            m_dictData      = right.m_dictData;
            m_dictChildList = right.m_dictChildList;
            m_dictMapList   = right.m_dictMapList;
            m_dictParent    = right.m_dictParent;
        }
        return *this;
    }

    bool operator==(const LinkedDict &right) const {
        return (m_dictData      == right.m_dictData      &&
                m_dictChildList == right.m_dictChildList &&
                m_dictMapList   == right.m_dictMapList   &&
                m_dictParent    == right.m_dictParent);
    }

    friend QDebug operator<<(QDebug dbg, const LinkedDict &right) {
        dbg.nospace() << "LinkedDict("
                      << "data:" << right.m_dictData
                      << "chld:" << right.m_dictChildList
                      << "maps:" << right.m_dictMapList
                      << "prnt:" << right.m_dictParent
                      << ")";
        return dbg.maybeSpace();
    }

signals:

public slots:
};

#endif // LINKEDDICT_H
