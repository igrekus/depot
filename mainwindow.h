#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlError>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QImage>

#include <linkeddict.h>
#include <databasemanager.h>
#include <dictmodel.h>
#include <stockmodel.h>
#include <productitem.h>

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

    // action processing
    void procActRefreshView();

    // contrlo elements
    void on_btnCategory_clicked();

private:
    Ui::MainWindow *ui;

    DataBaseManager *m_dbman;

    // model instances
    DictModel  *m_dictModel;
    StockModel *m_stockModel;

    // actions
    QAction *actRefreshView;

    // events
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
