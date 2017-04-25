#ifndef TRANSACTDATADIALOG_H
#define TRANSACTDATADIALOG_H

#include <QDialog>
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

    explicit TransactDataDialog(QWidget *parent = nullptr);
    ~TransactDataDialog();

    TransactDataDialog &setData     (const TransactItem &data) {m_data      = data; return *this;}
    TransactDataDialog &setDictModel(      DictModel    *dict) {m_dictModel = dict; return *this;}

    void updateWidgets();
    void initDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TransactDataDialog *ui;
};

#endif // TRANSACTDATADIALOG_H
