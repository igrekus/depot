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

#include <mapmodel.h>
#include <linkeddict.h>
#include <databasemanager.h>
#include <dictmodel.h>
#include <stockmodel.h>
#include <transactmodel.h>

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
//    void createStatusBar();

private slots:

    // обработка action
    void procActRefreshView();

    // редактировние категории
    // TODO: объединить однотипные действия в один метод?
    void procActAddCategory();
    void procActEditCategory();
    void procActDeleteCategory();

    void procActAddGroup();
    void procActEditGroup();
    void procActDeleteGroup();

    void procActAddStock();
    void procActEditStock();
    void procActDeleteStock();

    // control events
    void on_btnAddCategory_clicked();
    void on_btnAddGroup_clicked();
    void on_btnAddStock_clicked();
    void on_btnDeleteStockItem_clicked();
    void on_btnEditStockItem_clicked();

private:
    Ui::MainWindow *ui;

    DataBaseManager *m_dbman;

    // model instances
    DictModel  *m_dictModel;
    StockModel *m_stockModel;
    TransactModel *m_transactModel;

    MapModel *m_categoryListModel;
    MapModel *m_projectTagListModel;
//    MapModel *m_miscTagListModel;

    // actions
    QAction *actRefreshView;

    QAction *actAddCategory;
    QAction *actEditCategory;
    QAction *actDeleteCategory;

    QAction *actAddGroup;
    QAction *actEditGroup;
    QAction *actDeleteGroup;

    QAction *actAddStock;
    QAction *actEditStock;
    QAction *actDeleteStock;

    // events
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
