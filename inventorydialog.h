#ifndef INVENTORYDIALOG_H
#define INVENTORYDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

#include <databasemanager.h>
#include <inventorymodel.h>
#include <inventorydatadialog.h>
#include <linkeddict.h>

namespace Ui {
class InventoryDialog;
}

namespace Utility {
QString rndString(qint32 len);
}

class InventoryDialog : public QDialog
{
    Q_OBJECT

public:

    InventoryModel *m_inventoryModel;

    DataBaseManager *m_dbman;

    // edit actions
    QAction *actRefreshView;

    QAction *actEdit;
    QAction *actDelete;

    QAction *actAddCategory;
    QAction *actEditCategory;
    QAction *actDeleteCategory;

    QAction *actAddGroup;
    QAction *actEditGroup;
    QAction *actDeleteGroup;

    QAction *actAddInventory;
    QAction *actEditInventory;
    QAction *actDeleteInventory;

    explicit InventoryDialog(DataBaseManager *dbman, QWidget *parent = 0);
    ~InventoryDialog();

    void createActions();

    void initDialog();

public slots:
    void procActRefreshView();

    void procActEdit();
    void procActDelete();

    void procActAddCategory();
    void procActEditCategory();
    void procActDeleteCategory();

    void procActAddGroup();
    void procActEditGroup();
    void procActDeleteGroup();

    void procActAddInventory();
    void procActEditInventory();
    void procActDeleteInventory();

    // events
    void resizeEvent(QResizeEvent *event) override;

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_btnAddCategory_clicked();
    void on_btnAddGroup_clicked();
    void on_btnAddInventory_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();

private:
    Ui::InventoryDialog *ui;

    void testAddCat();
    void testRemCat();

    void testAddGrp();
    void testRemGrp();
};

#endif // INVENTORYDIALOG_H
