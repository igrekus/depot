#ifndef INVENTORYDIALOG_H
#define INVENTORYDIALOG_H

#include <QDialog>
#include <QDebug>

#include <databasemanager.h>
#include <inventorymodel.h>

namespace Ui {
class InventoryDialog;
}

class InventoryDialog : public QDialog
{
    Q_OBJECT

public:

    InventoryModel *m_inventoryModel;

    DataBaseManager *m_dbman;

    explicit InventoryDialog(DataBaseManager *dbman, QWidget *parent = 0);
    ~InventoryDialog();

    void initDialog();

    // events
    void resizeEvent(QResizeEvent *event) override;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InventoryDialog *ui;
};

#endif // INVENTORYDIALOG_H
