#ifndef INVENTORYDIALOG_H
#define INVENTORYDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QSortFilterProxyModel>

#include <databasemanager.h>
#include <inventorymodel.h>
#include <inventorydatadialog.h>
#include <linkeddict.h>
#include <dictmodel.h>
#include <stockdatadialog.h>
#include <recursivefilterproxymodel.h>
#include <delegatehighligtabltreetext.h>

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

    class TextDelegate : public QStyledItemDelegate
    {
    public:
        explicit TextDelegate(QObject * parent = 0) : QStyledItemDelegate(parent) {Q_UNUSED(parent)}
        ~TextDelegate() {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

    InventoryModel *m_inventoryModel;

    RecursiveFilterProxyModel *m_searchProxyModel;
//    QSortFilterProxyModel *m_searchProxyModel;

    DataBaseManager *m_dbman;
    DictModel *m_dictModel;

    // edit actions
    QAction *actRefreshView;

    QAction *actEdit;
    QAction *actDelete;

    QAction *actCategoryAdd;
    QAction *actCategoryEdit;
    QAction *actCategoryDelete;

    QAction *actGroupAdd;
    QAction *actGroupEdit;
    QAction *actGroupDelete;

    QAction *actInventoryAdd;
    QAction *actInventoryCopy;
    QAction *actInventoryEdit;
    QAction *actInventoryDelete;

    QAction *actRegisterStock;

    bool treeUpdated = false;

    explicit InventoryDialog(QWidget *parent = nullptr);
    ~InventoryDialog();

    InventoryDialog &setDbManager(DataBaseManager *dbman){m_dbman     = dbman; return *this;}
    InventoryDialog &setDictModel(      DictModel *dict) {m_dictModel = dict; return *this;}

    void createActions();

    void initDialog();

public slots:
    void procActRefreshView();

    void procActEdit();
    void procActDelete();

    void procActCategoryAdd();
    void procActCategoryEdit();
    void procActCategoryDelete();

    void procActGroupAdd();
    void procActGroupEdit();
    void procActGroupDelete();

    void procActInventoryAdd();
    void procActInventoryCopy();
    void procActInventoryEdit();
    void procActInventoryDelete();

    void procActRegisterStock();

    // events
protected:
    void resizeEvent(QResizeEvent *event) override;

    void changeEvent(QEvent *e);

private slots:
    void on_btnAddCategory_clicked();
    void on_btnAddGroup_clicked();
    void on_btnAddInventory_clicked();
    void on_btnCopy_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();
    void on_treeInventory_doubleClicked(const QModelIndex &index);
    void on_btnRegisterStock_clicked();
    void on_editSearch_textChanged(const QString &arg1);

private:
    Ui::InventoryDialog *ui;

    void testAddCat();
    void testRemCat();

    void testAddGrp();
    void testRemGrp();
};

#endif // INVENTORYDIALOG_H
