#ifndef STOCKDATADIALOG_H
#define STOCKDATADIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSortFilterProxyModel>
#include <QMessageBox>

#include <idstringmodel.h>
#include <stockitem.h>
#include <constants.h>
#include <mapmodel.h>
#include <dictmodel.h>

namespace Ui {
class StockDataDialog;
}

class StockDataDialog : public QDialog
{
    Q_OBJECT

public:

    StockItem m_data;
    StockItem m_oldData;

    DictModel *m_dictModel;

    QSortFilterProxyModel *m_searchFilterModel;

    DataBaseManager *m_dbman;

    bool m_dataModified = false;

    explicit StockDataDialog(QWidget *parent = nullptr);
    ~StockDataDialog();

    StockDataDialog &setDbManager(      DataBaseManager *dbman){m_dbman     = dbman; return *this;}
    StockDataDialog &setData     (const StockItem &data)       {m_data      = data;  return *this;}
    StockDataDialog &setDictModel(      DictModel *dict)       {m_dictModel = dict;  return *this;}

    void updateWidgetsWithStock(const StockItem &tmpstock);
    void updateWidgetsWithProduct(const qint32 prodId);
    void initDialog();

    StockItem getData();

protected:
    void changeEvent(QEvent *e);

    StockItem collectData();

private slots:
    void on_editSearchProduct_textChanged(const QString &arg1);

    void on_listProduct_doubleClicked(const QModelIndex &index);

    void on_listProduct_clicked(const QModelIndex &index);

    void on_btnOk_clicked();

private:
    Ui::StockDataDialog *ui;
};

#endif // STOCKDATADIALOG_H
