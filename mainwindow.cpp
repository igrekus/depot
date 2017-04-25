#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 TODO:
      - ф-ция копирования номенклатуры
      - ф-ция регистрации созданной номенклатуры на складе в окне редактора номенклатуры
      - обработчик ошибок через throw, сделать в одном месте -- где?;

Если в классе только слоты и нет сигналов, и используется new signal/slots syntax (connect в компайл-тайм к указателю на метод / лямбде вместо макросов SIGNAL/SLOT), то класс не должен иметь макрос Q_OBJECT и для него не будет сгенерирован *_moc.cpp.

Весь Qt позволяет подписываться на события и просто функциями. А учётом того, что этой функций может быть и лямбда, мы легко получаем механизм подписки на события любым своим (не входящий в метасистему Qt и соответственно хоть с шаблонами, хоть с чем угодно) классом в виде конструкции connect(sender, signal, [=]{reciver_object.handler();}).

rework model:
      - store navigational data in the nodes (self row, parent row, etc.):
      https://www.hardcoded.net/articles/using_qtreeview_with_qabstractitemmodel.htm

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

    ui->treeStock->setModel(m_stockModel);
    ui->treeStock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeStock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeStock->setUniformRowHeights(false);
    ui->treeStock->setIndentation(0);
    ui->treeStock->setItemDelegate(new TextDelegate(ui->treeStock));
    ui->treeStock->setItemDelegateForColumn(0, new BranchDelegate(ui->treeStock));

    ui->tableTransact->setModel(m_transactModel);
    ui->tableTransact->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableTransact->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTransact->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableTransact->verticalHeader()->setVisible(false);
    ui->tableTransact->verticalHeader()->setDefaultSectionSize(14);
    ui->tableTransact->horizontalHeader()->setHighlightSections(false);
    ui->tableTransact->horizontalHeader()->setFixedHeight(20);

    ui->comboCategory->setModel(m_dictModel->m_categoryListModel);
    ui->comboProject->setModel(m_dictModel->m_projectListModel);

    ui->comboCategory->setCurrentIndex(0);
    ui->comboProject->setCurrentIndex(0);

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

    actRefreshView = new QAction("Обновить", this);
    connect(actRefreshView, &QAction::triggered, this, &MainWindow::procActRefreshView);
//---
    actStockAdd = new QAction("Добавить номенклатуру на склад", this);
    connect(actStockAdd, &QAction::triggered, this, &MainWindow::procActStockAdd);
    actStockEdit = new QAction("Изменить позицию хранения", this);
    connect(actStockEdit, &QAction::triggered, this, &MainWindow::procActStockEdit);
    actStockDelete = new QAction("Удалить позицию хранения", this);
    connect(actStockDelete, &QAction::triggered, this, &MainWindow::procActStockDelete);
//---
    actTransactAdd = new QAction("Добавить приход/расход", this);
    connect(actTransactAdd, &QAction::triggered, this, &MainWindow::procActTransactAdd);
    actTransactEdit = new QAction("Изменить приход/расход", this);
    connect(actTransactEdit, &QAction::triggered, this, &MainWindow::procActTransactEdit);
    actTransactDelete = new QAction("Удалить приход/расход", this);
    connect(actTransactDelete, &QAction::triggered, this, &MainWindow::procActTransactDelete);
}

void MainWindow::refreshStock()
{
    m_stockModel->clear();
    m_stockModel->initModel();
}

TransactItem MainWindow::makeTransactItemFromStockItem(const StockItem &stock)
{
    return (TransactItem::TransactItemBuilder()
//            .setId(0)                           // new transact
            .setName(stock.itemName)
            .setDate(QDate::currentDate())
//            .setDiff(0)
//            .setNote(QString())
            .setStock(stock.itemId)             // ref to appropriate stock item
//            .setStaff(0)
            .setProject(stock.itemProject)
//            .setBill(0)
            .build());
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
    ui->tableTransact->setColumnWidth(2, tbwidth*0.07);
    ui->tableTransact->setColumnWidth(3, tbwidth*0.13);
    ui->tableTransact->setColumnWidth(4, tbwidth*0.10);
    ui->tableTransact->setColumnWidth(5, tbwidth*0.20);
    ui->tableTransact->show();
}

void MainWindow::procActStockAdd()
{
//    QModelIndex cur = ui->treeStock->selectionModel()->selectedIndexes().first();
//    QModelIndex pindex = [cur]() -> QModelIndex {
//        switch (cur.data(Constants::RoleNodeType).toInt()) {
//        case Constants::ItemGroup:
//            return cur;
//            break;
//        case Constants::ItemItem:
//            return cur.parent();     // TODO: fix cur.parent chain
//            break;
//        }
//    }();

    StockItem newStockItem = StockItem::StockItemBuilder().build();

    StockDataDialog dialog(this);
    dialog.setData(newStockItem)
          .setDictModel(m_dictModel)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    newStockItem = dialog.getData();

    m_stockModel->addStock(newStockItem);
//    qDebug() << pindex;
    statusBar()->showMessage("Позиция добавлена на склад.", 10000);
//    refreshStock(); // TODO: search and add to the correct branch
}

void MainWindow::procActStockEdit()
{    
    QModelIndex cur = ui->treeStock->selectionModel()->selectedIndexes().first();

    StockItem oldStockItem = m_stockModel->getStockItemByIndex(cur);

    StockDataDialog dialog(this);
    dialog.setData(oldStockItem)
          .setDictModel(m_dictModel)
          .setDbManager(m_dbman)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    oldStockItem = dialog.getData();

    m_stockModel->editStock(cur, oldStockItem);
}

void MainWindow::procActStockDelete()
{
    QModelIndex index = ui->treeStock->selectionModel()->selectedIndexes().first();
    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную позицию?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res == QMessageBox::Yes) {
        m_stockModel->deleteStock(index);
    }
}

void MainWindow::procActTransactAdd()
{
    QModelIndex stockIndex = ui->treeStock->selectionModel()->selectedIndexes().first();
    StockItem stock = m_stockModel->getStockItemByIndex(stockIndex);

    qDebug()<<stock;
    TransactItem newTransItem = makeTransactItemFromStockItem(stock);
    qDebug() << newTransItem;

    TransactDataDialog dialog(this);
    dialog.setData(newTransItem)
          .setDictModel(m_dictModel)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

//    newTransItem = dialog.getData();

//    QModelIndex index = m_transactModel->addTransact(newTransItem);
}

void MainWindow::procActTransactEdit()
{
    QModelIndex index = ui->tableTransact->selectionModel()->selectedIndexes().first();
    TransactItem oldTransItem = m_transactModel->getTransactItemByIndex(index);

    qDebug()<<oldTransItem;

    TransactDataDialog dialog(this);
    dialog.setData(oldTransItem)
          .setDictModel(m_dictModel)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
}

void MainWindow::procActTransactDelete()
{
    qDebug() << "del transact";
}

// -------------------- Misc Events -----------------------------------
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    actRefreshView->trigger();
}

// -------------------- Control Events --------------------------------
void MainWindow::on_btnInventoryEditor_clicked()
{
    InventoryDialog dialog(this);

    dialog.setDbManager(m_dbman)
          .setDictModel(m_dictModel)
          .initDialog();

    dialog.exec();

    if (dialog.treeUpdated) {
        refreshStock();
    }
}

void MainWindow::on_btnAddTransact_clicked()
{
    if (!ui->treeStock->selectionModel()->hasSelection() ||
         ui->treeStock->selectionModel()->selectedIndexes().first().data(Constants::RoleNodeType).toInt() != Constants::ItemItem) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите продукт на складе для создания записи о приходе/расходе.");
        return;
    }
    actTransactAdd->trigger();
}

void MainWindow::on_btnEditTransact_clicked()
{
    if (ui->tableTransact->selectionModel()->selectedIndexes().isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите запись о приходе/расходе для редактирования.");
        return;
    }
    actTransactEdit->trigger();
}

void MainWindow::on_btnDelTransact_clicked()
{
    if (ui->tableTransact->selectionModel()->selectedIndexes().isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите запись о приходе/расходе для удаления.");
        return;
    }
    actTransactDelete->trigger();
}

void MainWindow::on_btnAddStock_clicked()
{
    QModelIndexList list = ui->treeStock->selectionModel()->selectedIndexes();
    if (list.isEmpty() ||
        (list.first().data(Constants::RoleNodeType).toInt() != Constants::ItemGroup &&
         list.first().data(Constants::RoleNodeType).toInt() != Constants::ItemItem)) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите группу для добавления позиции хранения.");
        return;
    }
    actStockAdd->trigger();
}

void MainWindow::on_btnEditStock_clicked()
{
    QModelIndexList list = ui->treeStock->selectionModel()->selectedIndexes();
    if (list.isEmpty() || list.first().data(Constants::RoleNodeType).toInt() != Constants::ItemItem) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите позицию хранения для редактирования.");
        return;
    }
    actStockEdit->trigger();
}

void MainWindow::on_btnDelStock_clicked()
{
    QModelIndexList list = ui->treeStock->selectionModel()->selectedIndexes();
    if (list.isEmpty() || list.first().data(Constants::RoleNodeType).toInt() != Constants::ItemItem) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите позицию хранения для удаления.");
        return;
    }
    actStockDelete->trigger();
}

void MainWindow::on_btnReloadData_clicked()
{
    qDebug() << "add test";
    refreshStock();
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
    if (index.data(Constants::RoleNodeType).toInt() == Constants::ItemItem) {
        actTransactAdd->trigger();
    }
//    qDebug() << "r:" << index.row() << "id:" << index.data(ROLE_NODE_ID).toInt() << "name:" << index.data(Qt::DisplayRole).toString();
//    m_stockModel->debugInfo(index);
}

void MainWindow::on_tableTransact_doubleClicked(const QModelIndex &index)
{
    actTransactEdit->trigger();
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

    switch (index.data(Constants::RoleNodeType).toInt()) {
    case Constants::ItemCategory: {
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
    case Constants::ItemGroup: {
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
    case Constants::ItemItem: {
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

    if (index.data(Constants::RoleNodeType).toInt() == Constants::ItemItem) {
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

