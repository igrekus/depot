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
#include <QRegExp>
#include <QCompleter>

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
#include <recursivefilterproxymodel.h>
#include <projectrecursivefilterproxymodel.h>
#include <delegatehighligtabltreetext.h>
#include <dicteditordialog.h>
#include <transactrecursivefilterproxymodel.h>

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

    class StockBranchDelegate : public QStyledItemDelegate
    {
    public:
        StockBranchDelegate(QObject *parent = nullptr) {Q_UNUSED(parent)}
        ~StockBranchDelegate() {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

    class StockTextDelegate : public QStyledItemDelegate
    {
    public:
        StockTextDelegate(QObject *parent = nullptr) {Q_UNUSED(parent)}
        ~StockTextDelegate() {}
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initApplication();
    void createActions();
    void refreshStock();
    void refreshTransact();
//    void createStatusBar();

    void searchExpand();

private slots:

    // обработка action
    void procActRefreshView();

    void procActStockAdd();
    void procActStockEdit();
    void procActStockDelete();

    void procActTransactAdd();
    void procActTransactEdit();
    void procActTransactDelete();

    void procActSetSearchFilter(const QString &searchStr, const qint32 projectId, const qint32 locationId);

    void procActInventoryEditorOpen();
    void procActReportManagerOpen();
    void procActDictEditorOpen();

    // events
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

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
    void on_btnDictEditor_clicked();

    void on_treeStock_doubleClicked(const QModelIndex &index);
    void on_tableTransact_doubleClicked(const QModelIndex &index);    

    void on_editSearch_textChanged(const QString &searchStr);
    void on_comboProject_currentIndexChanged(int index);
    void on_comboLocation_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    DataBaseManager *m_dbman;

    // model instances
    DictModel  *m_dictModel;
    StockModel *m_stockModel;
    TransactModel *m_transactModel;

    ProjectRecursiveFilterProxyModel *m_stockSearchProxyModel;
//    RecursiveFilterProxyModel *m_transactSearchProxyModel;
    TransactRecursiveFilterProxyModel *m_transactSearchProxyModel;

    // actions
    QAction *actRefreshView;

    QAction *actStockAdd;
    QAction *actStockEdit;
    QAction *actStockDelete;

    QAction *actTransactAdd;
    QAction *actTransactEdit;
    QAction *actTransactDelete;

    QAction *actInventoryEditorOpen;
    QAction *actReportManagerOpen;
    QAction *actDictEditorOpen;
};

#endif // MAINWINDOW_H
