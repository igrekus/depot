#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 TODO:
      - обработчик ошибок через throw, сделать в одном месте -- где?;
      - singleton для констант типа DATA_INVALID

создание объектов:
      - https://ru.wikipedia.org/wiki/%D0%A4%D0%B0%D0%B1%D1%80%D0%B8%D1%87%D0%BD%D1%8B%D0%B9_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4_(%D1%88%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F)
      - https://ru.wikipedia.org/wiki/%D0%A1%D1%82%D1%80%D0%BE%D0%B8%D1%82%D0%B5%D0%BB%D1%8C_(%D1%88%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F)


*/


// TODO: убрать в общий класс настроек
QImage MainWindow::iconOpenFolder = QImage(":/gfx/openfolder.ico");
QImage MainWindow::iconClosedFolder = QImage(":/gfx/closedfolder.ico");
QImage MainWindow::iconDisabledFolder = QImage(":/gfx/disabledfolder.ico");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();

    m_dbman = new DataBaseManager(this);
    m_dictModel = new DictModel(m_dbman, this);
    m_stockModel = new StockModel(m_dbman, m_dictModel, this);

    m_categoryListModel = new MapModel(this);
    m_projectTagListModel = new MapModel(this);

    ui->treeStock->setModel(m_stockModel);

    ui->treeStock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeStock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeStock->setUniformRowHeights(true);
//    ui->treeStock->setAlternatingRowColors(true);
    ui->treeStock->setIndentation(0);

    ui->treeStock->setItemDelegateForColumn(0, new BranchDelegate(ui->treeStock));
    ui->treeStock->setItemDelegateForColumn(1, new TextDelegate(ui->treeStock));
    ui->treeStock->setItemDelegateForColumn(2, new TextDelegate(ui->treeStock));
    ui->treeStock->setItemDelegateForColumn(3, new TextDelegate(ui->treeStock));
    ui->treeStock->setItemDelegateForColumn(4, new TextDelegate(ui->treeStock));

    ui->comboCategory->setModel(m_categoryListModel);
    ui->comboProject->setModel(m_projectTagListModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initApplication()
{
    try {
        m_dbman->connectToDatabase();
    }
    catch (QSqlError err) {
        throw err;
    }

    m_stockModel->initModel();
    m_dictModel->initDicts();

    m_categoryListModel->initModel(m_dictModel->m_mapCategory);
    m_projectTagListModel->initModel(m_dictModel->m_mapProjectTag);

    actRefreshView->trigger();
}


void MainWindow::createActions()
{
//  Этот метод создаёт actions для обмена сигналами между частями приложения

//  Главное меню
//    QMenu *fileMenu = menuBar()->addMenu(tr("Файл(&F)"));

////  Если будет нужен тулбар:
////  QToolBar *fileToolBar = addToolBar(tr("File"));
////  const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));

//    actLogIn = new QAction(tr("Зайти как(&I)..."), this);
//    actLogIn->setStatusTip(tr("Зайти в систему как..."));
//    connect(actLogIn, &QAction::triggered, this, &MainWindow::procActLogIn);
//    fileMenu->addAction(actLogIn);
//    actLogOut = new QAction(tr("Сменить пользователя(&O)"), this);
//    actLogOut->setStatusTip(tr("Сменить текущего пользователя системы"));
//    connect(actLogOut, &QAction::triggered, this, &MainWindow::procActLogOut);
//    fileMenu->addAction(actLogOut);

////    fileToolBar->addAction(newAct);
//    fileMenu->addSeparator();

//    QAction *actExit = fileMenu->addAction(tr("Выход(&X)"), this, &MainWindow::procActExit);
//    actExit->setShortcuts(QKeySequence::Quit);
//    actExit->setStatusTip(tr("Выйти из программы"));

//    QMenu *toolsMenu = menuBar()->addMenu(tr("Инструменты(&T)"));
//    QAction *actAdminTools = toolsMenu->addAction(tr("Администрирование(&A)"), this, &MainWindow::procActAdminTools);
//    actAdminTools->setStatusTip(tr("Открыть окно инструментов администратора"));
//    QAction *actSettings = toolsMenu->addAction(tr("Настройки(&S)"), this, &MainWindow::procActEditSettings);
//    actSettings->setStatusTip(tr("Открыть окно настроек программы"));

//    QMenu *helpMenu = menuBar()->addMenu(tr("Помощь(&H)"));
//    QAction *actAbout = helpMenu->addAction(tr("О программе"), this, &MainWindow::procActAbout);
//    actAbout->setStatusTip(tr("Показать информацию о программе"));

//    actReFetchRootData = new QAction(tr("Fetch Data"), this);
//    connect(actReFetchRootData, &QAction::triggered, this, &MainWindow::procActReFetchRootData);

    actRefreshView = new QAction(tr("Refresh View"), this);
    connect(actRefreshView, &QAction::triggered, this, &MainWindow::procActRefreshView);

//    actAddRec = new QAction(tr("Add Record"), this);
//    connect(actAddRec, &QAction::triggered, this, &MainWindow::procActAddRec);

//    actEditRec = new QAction(tr("EditRecord"), this);
//    connect(actEditRec, &QAction::triggered, this, &MainWindow::procActEditRec);

//    actDelRec = new QAction(tr("DelRecord"), this);
//    connect(actDelRec, &QAction::triggered, this, &MainWindow::procActDelRec);
}


void MainWindow::procActRefreshView()
{
    qint32 twidth = ui->treeStock->frameGeometry().width()-30;
    ui->treeStock->hide();
    ui->treeStock->setColumnWidth(0, twidth*0.15);
    ui->treeStock->setColumnWidth(1, twidth*0.55);
    ui->treeStock->setColumnWidth(2, twidth*0.10);
    ui->treeStock->setColumnWidth(3, twidth*0.10);
    ui->treeStock->setColumnWidth(4, twidth*0.10);
    ui->treeStock->show();
}

void MainWindow::on_btnCategory_clicked()
{
    m_dbman->getTransactList();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    actRefreshView->trigger();
}

void MainWindow::BranchDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;

    painter->save();

    bool cellSelected = (option.state & QStyle::State_Selected);
    bool hasChildren = (option.state & QStyle::State_Children);
    bool branchOpen = (option.state & QStyle::State_Open);

//    QStyle::State_Item
//    0x00100000
//    Used by item views to indicate if a horizontal branch should be drawn.

//    QStyle::State_Sibling
//    0x00200000
//    Used by item views to indicate if a vertical line needs to be drawn (for siblings).


    switch (index.data(ROLE_LEVEL_ID).toInt()) {
    case StockItem::LevelRoot: {
        opt.rect.adjust(0, 0, opt.widget->frameGeometry().width(), 0);
        option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

        QPalette pal = option.palette;

        QPoint iconPos(option.rect.topLeft());
        iconPos.rx() += 3;
        QSize iconSize(option.rect.size());

        QImage icon;
        if (!hasChildren) {
            icon = iconDisabledFolder;
        } else if (branchOpen) {
            icon = iconOpenFolder;
        } else {
            icon = iconClosedFolder;
        }
        painter->drawImage(iconPos, icon.scaled(iconSize, Qt::KeepAspectRatio, Qt::FastTransformation));
        if (cellSelected) {
            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
            option.widget->style()->drawItemText(painter, option.rect.adjusted(+25, 0, 200, 0),
                                                 1, pal, true,
                                                 index.data(Qt::DisplayRole).toString(),
                                                 QPalette::ButtonText);
        }
        else {
            pal.setColor(QPalette::ButtonText, pal.text().color());
            option.widget->style()->drawItemText(painter, option.rect.adjusted(+25, 0, 200, 0),
                                                 1, pal, true,
                                                 index.data(Qt::DisplayRole).toString(),
                                                 QPalette::ButtonText);
        }
        break;
    }
    case StockItem::Level_1: {
        opt.rect.adjust(0, 0, opt.widget->frameGeometry().width(), 0);
        option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

        QPalette pal = option.palette;

        QPoint iconPos(option.rect.topLeft());
        QSize iconSize(option.rect.size());
        iconPos.rx() += 3 + 10;

        QImage icon;
        if (!hasChildren) {
            icon = iconDisabledFolder;
        } else if (branchOpen) {
            icon = iconOpenFolder;
        } else {
            icon = iconClosedFolder;
        }
        painter->drawImage(iconPos, icon.scaled(iconSize, Qt::KeepAspectRatio, Qt::FastTransformation));

        if (cellSelected) {
            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
            option.widget->style()->drawItemText(painter, option.rect.adjusted(+25 + 10, 0, 200, 0),
                                                 1, pal, true,
                                                 index.data(Qt::DisplayRole).toString(),
                                                 QPalette::ButtonText);
        }
        else {
            pal.setColor(QPalette::ButtonText, pal.text().color());
            option.widget->style()->drawItemText(painter, option.rect.adjusted(+25 + 10, 0, 200, 0),
                                                 1, pal, true,
                                                 index.data(Qt::DisplayRole).toString(),
                                                 QPalette::ButtonText);
        }
        break;
    }
    case StockItem::Level_2: {
        option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

        QPalette pal = option.palette;

        QPen pen(QColor(Qt::lightGray));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawLine(QLine(option.rect.x()+41, option.rect.y(),
                                option.rect.x()+41, option.rect.y()+option.rect.height()));

        if (cellSelected) {
            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
            option.widget->style()->drawItemText(painter, option.rect.adjusted(+25 + 20, 0, 0, 0),
                                                 1, pal, true,
                                                 index.data(Qt::DisplayRole).toString(),
                                                 QPalette::ButtonText);
        }
        else {
            pal.setColor(QPalette::ButtonText, pal.text().color());
            option.widget->style()->drawItemText(painter, option.rect.adjusted(+25 + 20, 0, 0, 0),
                                                 1, pal, true,
                                                 index.data(Qt::DisplayRole).toString(),
                                                 QPalette::ButtonText);
        }
        break;
    }
    }

    painter->restore();
}

void MainWindow::TextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;

    painter->save();

    bool cellSelected = (option.state & QStyle::State_Selected);
    bool hasChildren = (option.state & QStyle::State_Children);
    bool branchOpen = (option.state & QStyle::State_Open);

    if (index.data(ROLE_NODE_TYPE).toInt() == StockItem::ItemProduct) {
        option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

        QPen pen(QColor(Qt::lightGray));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawLine(QLine(option.rect.x()+1, option.rect.y(),
                                option.rect.x()+1, option.rect.y()+option.rect.height()));

        QPalette pal = option.palette;
        if (cellSelected) {
            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
            option.widget->style()->drawItemText(painter, option.rect.adjusted(+5, 0, 0, 0),
                                                 1, pal, true,
                                                 index.data(Qt::DisplayRole).toString(),
                                                 QPalette::ButtonText);
        }
        else {
            pal.setColor(QPalette::ButtonText, pal.text().color());
            option.widget->style()->drawItemText(painter, option.rect.adjusted(+5, 0, 0, 0),
                                                 1, pal, true,
                                                 index.data(Qt::DisplayRole).toString(),
                                                 QPalette::ButtonText);
        }
    }
    painter->restore();
}
