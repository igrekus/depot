#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 TODO:
      - обработчик ошибок через throw, сделать в одном месте -- где?;
      - singleton для констант типа DATA_INVALID
      - переписать определение позиции для вставки из свитча на лямбды

создание объектов:

      - при создании объектов через билдер, делать запрос в БД при вызове конструктора? (автоматизация, но намного больше вызовов)
        при инициализации приложения можно получать из БД список, а при получении данных о записи по ИД - через вызов в конструкторе?

persistence layer:
    persistCategory(CatItem); - database manager(s) as a pluging for persistence layer

данные на запись:
writeDBrecord("record :param").addparam(1).addparam(2)

data for dialog: QMapper

Order, Bill, Transact:
constructors:
    private void makeNormal(Customer customer) {
        Order o1 = new Order();
        customer.addOrder(o1);
        OrderLine line1 = new OrderLine(6, Product.find("TAL"));
        o1.addLine(line1);
        OrderLine line2 = new OrderLine(5, Product.find("HPK"));
        o1.addLine(line2);
        OrderLine line3 = new OrderLine(3, Product.find("LGV"));
        o1.addLine(line3);
        line2.setSkippable(true);
        o1.setRush(true);
    }
method chain:
   private void makeFluent(Customer customer) {
        customer.newOrder()
                .with(6, "TAL")
                .with(5, "HPK").skippable()
                .with(3, "LGV")
                .priorityRush();
    }

data validation:
http://rlacovara.blogspot.ru/2009/03/what-is-difference-between-dto-and-poco.html

tests:
https://sourceforge.net/p/cxxmock/wiki/Home/
https://github.com/eranpeer/FakeIt
https://github.com/google/googletest
http://artlang.net/article/view/14/
*/

// TODO: убрать в общий класс настроек, не использовать .qrc
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
    m_transactModel = new TransactModel(m_dbman, m_dictModel, this);

    m_categoryListModel = new MapModel(this);
    m_projectListModel = new MapModel(this);

    ui->treeStock->setModel(m_stockModel);
    ui->treeStock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeStock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeStock->setUniformRowHeights(false);
//    ui->treeStock->setAlternatingRowColors(true);
    ui->treeStock->setIndentation(0);
    ui->treeStock->setItemDelegate(new TextDelegate(ui->treeStock));
    ui->treeStock->setItemDelegateForColumn(0, new BranchDelegate(ui->treeStock));
//    ui->treeStock->setItemDelegateForColumn(1, new TextDelegate(ui->treeStock));
//    ui->treeStock->setItemDelegateForColumn(2, new TextDelegate(ui->treeStock));
//    ui->treeStock->setItemDelegateForColumn(3, new TextDelegate(ui->treeStock));
//    ui->treeStock->setItemDelegateForColumn(4, new TextDelegate(ui->treeStock));
//    ui->treeStock->setItemDelegateForColumn(5, new TextDelegate(ui->treeStock));

    ui->tableTransact->setModel(m_transactModel);
    ui->tableTransact->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableTransact->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTransact->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableTransact->verticalHeader()->setVisible(false);
    ui->tableTransact->verticalHeader()->setDefaultSectionSize(14);
    ui->tableTransact->horizontalHeader()->setHighlightSections(false);
    ui->tableTransact->horizontalHeader()->setFixedHeight(20);

    ui->comboCategory->setModel(m_categoryListModel);
    ui->comboProject->setModel(m_projectListModel);
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
    m_dictModel->initModel();
    m_transactModel->initModel();

    m_categoryListModel->initModel(m_dictModel->m_mapCategory);
    m_categoryListModel->addItem(0, 0, "Все");
    m_projectListModel->initModel(m_dictModel->m_mapProject);

    ui->comboCategory->setCurrentIndex(0);
    ui->comboProject->setCurrentText("Общее");

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

    actRefreshView = new QAction("Обновить", this);
    connect(actRefreshView, &QAction::triggered, this, &MainWindow::procActRefreshView);
}

// -------------------- Action Processing -----------------------------
void MainWindow::procActRefreshView()
{
    qint32 trwidth = ui->treeStock->frameGeometry().width()-30;
    ui->treeStock->hide();
    ui->treeStock->setColumnWidth(0, trwidth*0.15);
    ui->treeStock->setColumnWidth(1, trwidth*0.45);
    ui->treeStock->setColumnWidth(2, trwidth*0.10);
    ui->treeStock->setColumnWidth(3, trwidth*0.10);
    ui->treeStock->setColumnWidth(4, trwidth*0.13);
    ui->treeStock->setColumnWidth(5, trwidth*0.07);
    ui->treeStock->show();

    qint32 tbwidth = ui->tableTransact->frameGeometry().width()-20;
    ui->tableTransact->hide();
    ui->tableTransact->setColumnWidth(0, tbwidth*0.15);
    ui->tableTransact->setColumnWidth(1, tbwidth*0.35);
    ui->tableTransact->setColumnWidth(2, tbwidth*0.10);
    ui->tableTransact->setColumnWidth(3, tbwidth*0.20);
    ui->tableTransact->setColumnWidth(4, tbwidth*0.20);
    ui->tableTransact->show();
}

// -------------------- Control Events --------------------------------
void MainWindow::on_btnInventoryEditor_clicked()
{
    InventoryDialog dialog(m_dbman, this);

    dialog.initDialog();

    dialog.exec();
}

void MainWindow::on_btnReloadData_clicked()
{
    qDebug() << "add test";
//    testAddCat();
//    testAddGrp();
}

void MainWindow::on_btnReport_clicked()
{
    qDebug() << "rem test";
//    testRemCat();
//    testRemGrp();
}

void MainWindow::on_treeStock_doubleClicked(const QModelIndex &index)
{
    qDebug() << "r:" << index.row() << "id:" << index.data(ROLE_NODE_ID).toInt() << "name:" << index.data(Qt::DisplayRole).toString();
    m_stockModel->debugInfo(index);
}

// -------------------- Misc Events -----------------------------------
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    actRefreshView->trigger();
}

// -------------------- Delegates -------------------------------------
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

    switch (index.data(ROLE_NODE_TYPE).toInt()) {
    case StockItem::ItemCategory: {
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
        }
        else {
            pal.setColor(QPalette::ButtonText, pal.text().color());
        }
        option.widget->style()->drawItemText(painter, option.rect.adjusted(+25, 0, 200, 0),
                                             1, pal, true,
                                             index.data(Qt::DisplayRole).toString(),
                                             QPalette::ButtonText);
        break;
    }
    case StockItem::ItemGroup: {
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
        }
        else {
            pal.setColor(QPalette::ButtonText, pal.text().color());
        }
        option.widget->style()->drawItemText(painter, option.rect.adjusted(+25 + 10, 0, 200, 0),
                                             1, pal, true,
                                             index.data(Qt::DisplayRole).toString(),
                                             QPalette::ButtonText);
        break;
    }
    case StockItem::ItemItem: {
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
//    bool hasChildren = (option.state & QStyle::State_Children);
//    bool branchOpen = (option.state & QStyle::State_Open);

    if (index.data(ROLE_NODE_TYPE).toInt() == StockItem::ItemItem) {
        option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

        QPen pen(QColor(Qt::lightGray));
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawLine(QLine(option.rect.x()+1, option.rect.y(),
                                option.rect.x()+1, option.rect.y()+option.rect.height()));

        QPalette pal = option.palette;
        if (cellSelected) {
            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
        }
        else {
            pal.setColor(QPalette::ButtonText, pal.text().color());
        }
        option.widget->style()->drawItemText(painter, option.rect.adjusted(+5, 0, 0, 0),
                                             1, pal, true,
                                             index.data(Qt::DisplayRole).toString(),
                                             QPalette::ButtonText);
    }
    painter->restore();
}

// -------------------- Testing routines ------------------------------

// -------------------- Utility ---------------------------------------
