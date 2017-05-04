#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlError>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QImage>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QMessageBox>

#include <reportmanager.h>
#include <mapmodel.h>
#include <linkeddict.h>
#include <databasemanager.h>
#include <dictmodel.h>
#include <stockmodel.h>
#include <transactmodel.h>
#include <inventorydialog.h>
#include <transactdatadialog.h>
#include <stockdatadialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static QImage iconOpenFolder;
    static QImage iconClosedFolder;
    static QImage iconDisabledFolder;

    class BranchDelegate : public QStyledItemDelegate
    {
    public:
        BranchDelegate(QObject *parent = nullptr) {Q_UNUSED(parent)}
        ~BranchDelegate() {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

    class TextDelegate : public QStyledItemDelegate
    {
    public:
        TextDelegate(QObject *parent = nullptr) {Q_UNUSED(parent)}
        ~TextDelegate() {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initApplication();
    void createActions();
    void refreshStock();
//    void createStatusBar();

    TransactItem makeTransactItemFromStockItem(const StockItem &stock);
private slots:

    // обработка action
    void procActRefreshView();

    void procActStockAdd();
    void procActStockEdit();
    void procActStockDelete();

    void procActTransactAdd();
    void procActTransactEdit();
    void procActTransactDelete();

    // events
    void resizeEvent(QResizeEvent *event) override;

    // control events
    void on_btnInventoryEditor_clicked();
    void on_btnOrderEditor_clicked();

    void on_btnAddTransact_clicked();
    void on_btnEditTransact_clicked();
    void on_btnDelTransact_clicked();

    void on_btnAddStock_clicked();
    void on_btnEditStock_clicked();
    void on_btnDelStock_clicked();

    void on_btnReloadData_clicked();
    void on_btnReport_clicked();

    void on_treeStock_doubleClicked(const QModelIndex &index);

    void on_tableTransact_doubleClicked(const QModelIndex &index);    
private:
    Ui::MainWindow *ui;

    DataBaseManager *m_dbman;

    // model instances
    DictModel  *m_dictModel;
    StockModel *m_stockModel;
    TransactModel *m_transactModel;

    // actions
    QAction *actRefreshView;

    QAction *actStockAdd;
    QAction *actStockEdit;
    QAction *actStockDelete;

    QAction *actTransactAdd;
    QAction *actTransactEdit;
    QAction *actTransactDelete;
};

#endif // MAINWINDOW_H
