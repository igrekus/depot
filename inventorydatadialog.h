#ifndef INVENTORYDATADIALOG_H
#define INVENTORYDATADIALOG_H

#include <QDialog>
#include <QDebug>

#include <inventorydatadialog.h>

namespace Ui {
class InventoryDataDialog;
}

class InventoryDataDialog : public QDialog
{
    Q_OBJECT

public:

    explicit InventoryDataDialog(QWidget *parent = 0);
    ~InventoryDataDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InventoryDataDialog *ui;
};

#endif // INVENTORYDATADIALOG_H
