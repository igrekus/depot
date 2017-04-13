#ifndef HASHDICT_H
#define HASHDICT_H

#include <QObject>
#include <QDebug>

class HashDict
{

public:
    using MapIdToData = QMultiHash<qint32, QString>;
    using MapDataToId = QMultiHash<QString, qint32>;

    MapIdToData id;
    MapDataToId di;
    HashDict():
        id(),
        di()
    {}
    HashDict(MapIdToData &id, MapDataToId &di):
        id(id),
        di(di)
    {}
    HashDict(const HashDict &cpy):
        id(cpy.id),
        di(cpy.di)
    {}
    HashDict &operator=(const HashDict &right)
    {
        if (this != &right) {
            id = right.id;
            di = right.di;
        }
        return *this;
    }
    bool operator==(const HashDict &right) const
    {
        return (id == right.id &&
                di == right.di);
    }
    friend QDebug operator<<(QDebug dbg, const HashDict &right){
        dbg.nospace() << "HashDict("
                      << ">id:" << right.id
                      << ">di:" << right.di
                      << ")";
        return dbg.maybeSpace();
    }

signals:

public slots:
};

#endif // HASHDICT_H
