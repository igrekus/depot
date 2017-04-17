#ifndef TRANSACTDATADIALOG_H
#define TRANSACTDATADIALOG_H

#include <QDialog>

namespace Ui {
class TransactDataDialog;
}

class TransactDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactDataDialog(QWidget *parent = 0);
    ~TransactDataDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TransactDataDialog *ui;
};

#endif // TRANSACTDATADIALOG_H
