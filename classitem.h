#ifndef CLASSITEM_H
#define CLASSITEM_H

#include <QObject>
#include <QDebug>

#include <abstractitem.h>

class ClassItem : public AbstractItem
{
public:
    ClassItem():
        AbstractItem()
    {}

    explicit ClassItem(const qint32 id, const QString &name):
        AbstractItem(id, name)
    {}

    ClassItem(const ClassItem &copy):
        AbstractItem(copy)
    {}

    ClassItem &operator=(const ClassItem &right) {
        if (this != &right) {
            itemId   = right.itemId;
            itemName = right.itemName;
        }
        return *this;
    }

    bool operator==(const ClassItem &right) const {
        return (itemId   == right.itemId &&
                itemName == right.itemName);
    }

    friend QDebug operator<<(QDebug dbg, const ClassItem &right) {
        dbg.nospace() << "Class("
                      << "id:"    << right.itemId
                      << " name:" << right.itemName
                      << ")";
        return dbg.maybeSpace();
    }

    using ClassList = QVector<ClassItem>;

    class ClassItemBuilder {
    public:
        qint32  classId = 0;
        QString className = QString();
        ClassItemBuilder& setId  (const qint32   id)   { this->classId   = id;   return *this; }
        ClassItemBuilder& setName(const QString &name) { this->className = name; return *this; }
        ClassItem build() {
            return ClassItem(this->classId, this->className);
        }
    };
};

#endif // CLASSITEM_H
