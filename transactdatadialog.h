#ifndef TRANSACTDATADIALOG_H
#define TRANSACTDATADIALOG_H

#include <QDialog>
#include <QCompleter>
#include <QMessageBox>

#include <transactitem.h>
#include <constants.h>
#include <dictmodel.h>
#include <stockmodel.h>
#include <projectrecursivefilterproxymodel.h>
#include <delegatehighligtabltreetext.h>

namespace Ui {
class TransactDataDialog;
}

class TransactDataDialog : public QDialog
{
    Q_OBJECT

public:

    TransactItem m_data;

    DictModel *m_dictModel;

    StockModel *m_stockModel;

    ProjectRecursiveFilterProxyModel *m_searchFilterModel;

    QCompleter *comProject;
    QCompleter *comStaff;

    explicit TransactDataDialog(QWidget *parent = nullptr);
    ~TransactDataDialog();

    TransactDataDialog &setData      (const TransactItem &data)  {m_data       = data;  return *this;}
    TransactDataDialog &setDictModel (      DictModel    *dict)  {m_dictModel  = dict;  return *this;}
    TransactDataDialog &setStockModel(      StockModel   *stock) {m_stockModel = stock; return *this;}

    void resetWidgets();
    void updateWidgets();
    void initDialog();

    void refreshView();

    TransactItem getData();

protected:
    TransactItem collectData();

    void changeEvent(QEvent *e);

private slots:
    void on_spinDiff_valueChanged(int arg1);

    void on_btnOk_clicked();

    void on_editSearch_textChanged(const QString &arg1);

    void on_treeStock_clicked(const QModelIndex &index);

    void on_treeStock_doubleClicked(const QModelIndex &index);

    void treeStockSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::TransactDataDialog *ui;
};

#endif // TRANSACTDATADIALOG_H
