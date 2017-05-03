#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QDialog>
#include <QDebug>
#include <QDate>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include <databasemanager.h>
#include <dictmodel.h>
#include <mapmodel.h>
#include <reportrequest.h>

#include "xlsxdocument.h"
#include "xlsxworkbook.h"
#include "xlsxcellrange.h"
#include "xlsxformat.h"
#include "xlsxabstractsheet.h"
#include "xlsxconditionalformatting.h"

namespace Ui {
class ReportManager;
}

class ReportManager : public QDialog
{
    Q_OBJECT

public:

    enum ReportType {
        ReportStock = 0,
        ReportTransact
    };

    DataBaseManager *m_dbman;

    DictModel *m_dictModel;

    MapModel *m_filteredGroupModel_1;
    MapModel *m_filteredGroupModel_2;

    QSqlQueryModel *m_reportModel;
    QSortFilterProxyModel *m_proxyModel;

    QString m_reportDir;
    QString m_exportFileName;
    ReportType m_reportType = ReportStock;

    explicit ReportManager(QWidget *parent = 0);
    ~ReportManager();

    ReportManager &setDbManager(DataBaseManager *dbman) {m_dbman     = dbman; return *this;}
    ReportManager &setDictModel(DictModel       *dict)  {m_dictModel = dict;  return *this;}

    void filterGroupCombo_1(const qint32 catId);
    void filterGroupCombo_2(const qint32 catId);

    void initDialog();

    ReportRequest collectStockRequestData();
    ReportRequest collectTransactRequestData();

    void resizeStockTable();
    void resizeTransactTable();
    QString makeFileName();

    void xlsxWriteHeader(QXlsx::Document &doc,
                          const QXlsx::CellReference &topleft,
                          const QVector<qint32> columnWidths,
                          const QStringList &hdrData);

    void saveStockReport();
    void saveTransactReport();

protected:
    void resizeEvent(QResizeEvent *event) override;

    void changeEvent(QEvent *e);

private slots:
    void on_comboCategory_currentIndexChanged(int index);

    void on_comboCategory_2_currentIndexChanged(int index);

    void on_comboGroup_currentTextChanged(const QString &arg1);

    void on_comboGroup_2_currentTextChanged(const QString &arg1);

    void on_btnShow_clicked();

    void on_btnExport_clicked();

private:
    Ui::ReportManager *ui;
};

#endif // REPORTMANAGER_H
