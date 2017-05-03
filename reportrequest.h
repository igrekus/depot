#ifndef REPORTREQUEST_H
#define REPORTREQUEST_H

#include <QObject>
#include <QDate>
#include <QDebug>

class ReportRequest {
public:

    qint32  projectId;
    qint32  categoryId;
    qint32  groupId;
    QDate   fromDate;
    QDate   untilDate;
    qint32  flag;
    QString searchString;

    ReportRequest() = default;
    ~ReportRequest(){}
    ReportRequest(const ReportRequest &copy) = default;
    ReportRequest &operator=(const ReportRequest &right) = default;
    ReportRequest(const qint32   projectId,
                  const qint32   categoryId,
                  const qint32   groupId,
                  const QDate   &fromDate,
                  const QDate   &untilDate,
                  const qint32   flag,
                  const QString &str):
        projectId(projectId),
        categoryId(categoryId),
        groupId(groupId),
        fromDate(fromDate),
        untilDate(untilDate),
        flag(flag),
        searchString(str)
    {}
    friend QDebug operator<<(QDebug dbg, const ReportRequest &right) {
        dbg.nospace() << "ReportRequest("
                      << "proj:"   << right.projectId
                      << " cat:"   << right.categoryId
                      << " grp:"   << right.groupId
                      << " from:"  << right.fromDate
                      << " until:" << right.untilDate
                      << " flag:"  << right.flag
                      << "  str:"  << right.searchString
                      << ")";
        return dbg.maybeSpace();
    }

    class ReportRequestBuilder {
    public:
        qint32  projectId  = 0;
        qint32  categoryId = 0;
        qint32  groupId    = 0;
        QDate   fromDate   = QDate();
        QDate   untilDate  = QDate();
        qint32  flag       = 0;
        QString searchStr  = QString();

        ReportRequestBuilder& setProject   (const qint32   proj)  { this->projectId  = proj; return *this; }
        ReportRequestBuilder& setCategory  (const qint32   cat)   { this->categoryId = cat;  return *this; }
        ReportRequestBuilder& setGroup     (const qint32   grp)   { this->groupId    = grp;  return *this; }
        ReportRequestBuilder& setFromDate  (const QDate   &from)  { this->fromDate   = from; return *this; }
        ReportRequestBuilder& setUntilDate (const QDate   &till)  { this->untilDate  = till; return *this; }
        ReportRequestBuilder& setFlag      (const qint32   flag)  { this->flag       = flag; return *this; }
        ReportRequestBuilder& setSearch    (const QString &str)   { this->searchStr  = str;  return *this; }

        ReportRequest build() {
            return ReportRequest(this->projectId,
                                 this->categoryId,
                                 this->groupId,
                                 this->fromDate,
                                 this->untilDate,
                                 this->flag,
                                 this->searchStr);
        }
    };
};

#endif // REPORTREQUEST_H
