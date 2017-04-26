#ifndef TRANSACTDATADIALOG_H
#define TRANSACTDATADIALOG_H

#include <QDialog>
#include <QCompleter>
#include <QMessageBox>

#include <transactitem.h>
#include <constants.h>
#include <dictmodel.h>


namespace Ui {
class TransactDataDialog;
}

class TransactDataDialog : public QDialog
{
    Q_OBJECT

public:

    TransactItem m_data;
    TransactItem m_oldData;

    DictModel *m_dictModel;

    QCompleter *comProject;
    QCompleter *comStaff;

    explicit TransactDataDialog(QWidget *parent = nullptr);
    ~TransactDataDialog();

    TransactDataDialog &setData     (const TransactItem &data) {m_data      = data; return *this;}
    TransactDataDialog &setDictModel(      DictModel    *dict) {m_dictModel = dict; return *this;}

    void updateWidgets();
    void initDialog();

    TransactItem getData();

protected:
    TransactItem collectData();

    void changeEvent(QEvent *e);

private slots:
    void on_spinDiff_valueChanged(int arg1);

    void on_btnOk_clicked();

private:
    Ui::TransactDataDialog *ui;
};

#endif // TRANSACTDATADIALOG_H
