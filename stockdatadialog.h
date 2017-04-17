#ifndef STOCKDATADIALOG_H
#define STOCKDATADIALOG_H

#include <QDialog>

namespace Ui {
class StockDataDialog;
}

class StockDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StockDataDialog(QWidget *parent = 0);
    ~StockDataDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::StockDataDialog *ui;
};

#endif // STOCKDATADIALOG_H
