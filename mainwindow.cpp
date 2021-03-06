#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataWidgetMapper>
/*
 TODO:

    общая архитектура:
    !!!При расширении программы, переписать модель, выделить доменную модель, в которую запихнуть всю логику из procActX!!!

    система настроек
    комбобоксы -- мультивыбор
    оформление

    Редактор словарей:

    Склад:

    Номенклатура:

    Транзакции:

    Отчёты:

    - !!!rewrite deletes to bool toggle!!!
    - удаление номенклатуры, только если на складе нет регистрации данной номенклатуры

    - обработчик ошибок через throw, сделать в одном месте -- где?;

Если в классе только слоты и нет сигналов, и используется new signal/slots syntax (connect в компайл-тайм к
указателю на метод / лямбде вместо макросов SIGNAL/SLOT), то класс не должен иметь макрос Q_OBJECT и для него не
будет сгенерирован *_moc.cpp.

Весь Qt позволяет подписываться на события и просто функциями. А учётом того, что этой функций может быть и лямбда,
мы легко получаем механизм подписки на события любым своим (не входящий в метасистему Qt и соответственно хоть с
шаблонами, хоть с чем угодно) классом в виде конструкции connect(sender, signal, [=]{reciver_object.handler();}).

rework model for performace:
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
                .with(3, "LGV").priorityRush();
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
QImage MainWindow::iconOpenFolder     = QImage(":/gfx/openfolder.ico");
QImage MainWindow::iconClosedFolder   = QImage(":/gfx/closedfolder.ico");
QImage MainWindow::iconDisabledFolder = QImage(":/gfx/disabledfolder.ico");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->btnAddStock->setVisible(false);

    createActions();

    m_dbman = new DataBaseManager(this);
    m_dictModel = new DictModel(m_dbman, this);
    m_stockModel = new StockModel(m_dbman, m_dictModel, this);
    m_transactModel = new TransactModel(m_dbman, m_dictModel, this);

    m_stockSearchProxyModel = new ProjectRecursiveFilterProxyModel(this);
    m_stockSearchProxyModel->setSourceModel(m_stockModel);
    m_stockSearchProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_transactSearchProxyModel= new TransactRecursiveFilterProxyModel(this);
    m_transactSearchProxyModel->setSourceModel(m_transactModel);
    m_transactSearchProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    qApp->installEventFilter(this);
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

    ui->treeStock->setModel(m_stockSearchProxyModel);
    ui->treeStock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeStock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeStock->setUniformRowHeights(false);
    ui->treeStock->header()->setDefaultAlignment(Qt::AlignCenter); // Qt::TextWordWrap
//    ui->treeStock->setAlternatingRowColors(true);
    ui->treeStock->setItemDelegate(new DelegateHighligtableTreeText(ui->treeStock));
    ui->treeStock->header()->setStretchLastSection(true);
//    ui->treeStock->setIndentation(0);
//    ui->treeStock->setItemDelegate(new StockTextDelegate(ui->treeStock));
//    ui->treeStock->setItemDelegateForColumn(0, new StockBranchDelegate(ui->treeStock));

    ui->tableTransact->setModel(m_transactSearchProxyModel);
    ui->tableTransact->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableTransact->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableTransact->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableTransact->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableTransact->horizontalHeader()->setHighlightSections(false);
    ui->tableTransact->horizontalHeader()->setFixedHeight(20);
    ui->tableTransact->horizontalHeader()->setStretchLastSection(true);
    ui->tableTransact->verticalHeader()->setVisible(false);
    ui->tableTransact->verticalHeader()->setDefaultSectionSize(14);
    ui->tableTransact->setItemDelegate(new DelegateHighligtableTableText(ui->tableTransact));

    projectCompleter = new QCompleter(this);
    projectCompleter->setModel(m_dictModel->m_projectListModel);
//    projectCompleter->setCompletionRole(Qt::DisplayRole);
//    projectCompleter->setCompletionColumn(0);
    projectCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    projectCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    projectCompleter->setMaxVisibleItems(20);
    projectCompleter->setFilterMode(Qt::MatchContains);

    ui->comboProject->setModel(m_dictModel->m_projectListModel);
    ui->comboProject->setInsertPolicy(QComboBox::NoInsert);
    ui->comboProject->setCompleter(projectCompleter);
    ui->comboProject->setCurrentIndex(0);

    actRefreshView->trigger();

    ui->statusBar->showMessage("Готов к работе", 5000);
}

void MainWindow::createActions()
{
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
    actTransactAdd = new QAction("Новый приход/расход", this);
    actTransactAdd->setShortcut(QKeySequence("Ctrl+A"));
    ui->menuTools->addAction(actTransactAdd);
    connect(actTransactAdd, &QAction::triggered, this, &MainWindow::procActTransactAdd);
    actTransactEdit = new QAction("Изменить приход/расход", this);
    connect(actTransactEdit, &QAction::triggered, this, &MainWindow::procActTransactEdit);
    actTransactDelete = new QAction("Удалить приход/расход", this);
    connect(actTransactDelete, &QAction::triggered, this, &MainWindow::procActTransactDelete);
//---
    actInventoryEditorOpen = new QAction("Номенклатура...", this);
    actInventoryEditorOpen->setShortcut(QKeySequence("Ctrl+E"));
    ui->menuTools->addAction(actInventoryEditorOpen);
    connect(actInventoryEditorOpen, QAction::triggered, this, &MainWindow::procActInventoryEditorOpen);

    actReportManagerOpen = new QAction("Отчёты...", this);
    actReportManagerOpen->setShortcut(QKeySequence("Ctrl+W"));
    ui->menuTools->addAction(actReportManagerOpen);
    connect(actReportManagerOpen, QAction::triggered, this, &MainWindow::procActReportManagerOpen);

    actDictEditorOpen = new QAction("Словари...", this);
    actDictEditorOpen->setShortcut(QKeySequence("Ctrl+D"));
    ui->menuTools->addAction(actDictEditorOpen);
    connect(actDictEditorOpen, QAction::triggered, this, &MainWindow::procActDictEditorOpen);
}

void MainWindow::refreshStock()
{
    m_stockModel->clear();
    m_stockModel->initModel();
}

void MainWindow::refreshTransact()
{
    m_transactModel->clear();
    m_transactModel->initModel();
}

void MainWindow::searchExpand()
{
    if (ui->editSearch->text().size() > 1) {
        ui->treeStock->expandAll();
    }
}

// -------------------- Action Processing -----------------------------
void MainWindow::procActRefreshView()
{
    qint32 trwidth = ui->treeStock->frameGeometry().width()-30;
    ui->treeStock->hide();
    ui->treeStock->setColumnWidth(0, trwidth*0.20);
    ui->treeStock->setColumnWidth(1, trwidth*0.45);
    ui->treeStock->setColumnWidth(2, trwidth*0.05);
    ui->treeStock->setColumnWidth(3, trwidth*0.05);
    ui->treeStock->setColumnWidth(4, trwidth*0.05);
    ui->treeStock->setColumnWidth(5, trwidth*0.07);
    ui->treeStock->setColumnWidth(6, trwidth*0.08);
    ui->treeStock->setColumnWidth(7, trwidth*0.05);
    ui->treeStock->show();

    qint32 tbwidth = ui->tableTransact->frameGeometry().width()-20;
    if (tbwidth < 200) {
        tbwidth = ui->tabTransact->frameGeometry().width()+570;
    }
    ui->tableTransact->hide();
    ui->tableTransact->setColumnWidth(0, tbwidth*0.15);
    ui->tableTransact->setColumnWidth(1, tbwidth*0.35);
    ui->tableTransact->setColumnWidth(2, tbwidth*0.05);
    ui->tableTransact->setColumnWidth(3, tbwidth*0.13);
    ui->tableTransact->setColumnWidth(4, tbwidth*0.10);
    ui->tableTransact->setColumnWidth(5, tbwidth*0.22);
    ui->tableTransact->show();
}

void MainWindow::procActStockAdd()
{
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
//    refreshStock(); // TODO: search and add to the correct branch
}

void MainWindow::procActStockEdit()
{    
    QModelIndex selectedIndex = ui->treeStock->selectionModel()->selectedIndexes().first();
    QModelIndex sourceIndex = m_stockSearchProxyModel->mapToSource(selectedIndex);

    StockItem oldStockItem = m_stockModel->getStockItemByIndex(sourceIndex);

//    qDebug() << "old stock:" << oldStockItem;

    StockDataDialog dialog(this);
    dialog.setData(oldStockItem)
          .setDictModel(m_dictModel)
          .setDbManager(m_dbman)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    oldStockItem = dialog.getData();

//    qDebug() << "edited stock:" << oldStockItem;

    m_stockModel->editStock(sourceIndex, oldStockItem);
}

void MainWindow::procActStockDelete()
{
    QModelIndex selectedIndex = ui->treeStock->selectionModel()->selectedIndexes().first();
    QModelIndex index = m_stockSearchProxyModel->mapToSource(selectedIndex);
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
    StockItem stockItem;
//    QModelIndex stockIndex;
    if (!ui->treeStock->selectionModel()->hasSelection()) {
        ui->treeStock->selectionModel()->clear();
        ui->treeStock->selectionModel()->setCurrentIndex(m_stockSearchProxyModel->index(0, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
    QModelIndex selectedIndex = ui->treeStock->selectionModel()->selectedIndexes().first();
    if (selectedIndex.data(Constants::RoleNodeType) == Constants::ItemItem) {
        stockItem = m_stockModel->getStockItemByIndex(m_stockSearchProxyModel->mapToSource(selectedIndex));
    }

    TransactItem newTransactItem = TransactItem::TransactItemBuilder().fromStockItem(stockItem);

    TransactDataDialog dialog(this);
    dialog.setData(newTransactItem)
          .setDictModel(m_dictModel)
          .setStockModel(m_stockModel)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    newTransactItem = dialog.getData();

    QModelIndex transactIndex = m_transactModel->addTransact(newTransactItem);

    m_stockModel->modifyStockByTransact(newTransactItem);

    QModelIndex indexToSelect = m_transactSearchProxyModel->mapFromSource(transactIndex);

    ui->tableTransact->selectionModel()->clear();
    ui->tableTransact->selectionModel()->setCurrentIndex(indexToSelect, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void MainWindow::procActTransactEdit()
{
    // TODO: affect stock item
    QModelIndex selectedIndex = ui->tableTransact->selectionModel()->selectedIndexes().first();
    QModelIndex index = m_transactSearchProxyModel->mapToSource(selectedIndex);
    TransactItem oldTransItem = m_transactModel->getTransactItemByIndex(index);

    TransactDataDialog dialog(this);
    dialog.setData(oldTransItem)
          .setDictModel(m_dictModel)
          .setStockModel(m_stockModel)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    TransactItem editedTransactItem = dialog.getData();

    m_transactModel->editTransact(index, editedTransactItem);

    editedTransactItem.itemDiff -= oldTransItem.itemDiff;

    m_stockModel->modifyStockByTransact(editedTransactItem);
}

void MainWindow::procActTransactDelete()
{
    // TODO: affect stock item
    QModelIndex selectedIndex = ui->tableTransact->selectionModel()->selectedIndexes().first();
    QModelIndex index = m_transactSearchProxyModel->mapToSource(selectedIndex);

    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную запись о приходе/расходе?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res != QMessageBox::Yes) {
        return;
    }

    TransactItem deletedTransactItem = m_transactModel->getTransactItemByIndex(index);

    deletedTransactItem.itemDiff = -deletedTransactItem.itemDiff;

    m_stockModel->modifyStockByTransact(deletedTransactItem);

    m_transactModel->deleteTransact(index);

    ui->tableTransact->selectionModel()->clear();
}

void MainWindow::procActSetSearchFilter(const QString &searchStr, const qint32 searchIndex) {
//    qDebug() << searchStr << searchIndex;
    m_stockSearchProxyModel->setFilterWildcard(searchStr);
    m_stockSearchProxyModel->setFilterProjectId(searchIndex);

    m_transactSearchProxyModel->setFilterWildcard(searchStr);
    m_transactSearchProxyModel->setFilterProjectId(searchIndex);

    m_stockSearchProxyModel->invalidate();
    m_transactSearchProxyModel->invalidate();

    if (searchStr.isEmpty()) {
        ui->treeStock->collapseAll();
    } else if (searchStr.size() > 1) {
        ui->treeStock->expandAll();
    }
}

void MainWindow::procActInventoryEditorOpen()
{
    InventoryDialog dialog(this);

    dialog.setDbManager(m_dbman)
          .setDictModel(m_dictModel)
          .initDialog();

    dialog.exec();

    if (dialog.treeUpdated) {
        refreshStock();
        refreshTransact();
    }
}

void MainWindow::procActReportManagerOpen()
{
    ReportManager dialog;

    dialog.setDbManager(m_dbman)
          .setDictModel(m_dictModel)
          .initDialog();

    dialog.exec();
}

void MainWindow::procActDictEditorOpen()
{
    DictEditorDialog dialog;

    dialog.setDbManager(m_dbman)
          .setDictModel(m_dictModel)
          .initDialog();

    dialog.exec();

    m_dictModel->updateLocationList();
    m_dictModel->updateStaffList();
    m_dictModel->updateProjectList();

    ui->comboProject->setCurrentIndex(0);
}

// -------------------- Misc Events -----------------------------------
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    actRefreshView->trigger();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        if (key->key() == Qt::Key_Escape) {
            if (ui->editSearch->text().isEmpty()) {
                return QObject::eventFilter(watched, event);
            }
            if (watched == ui->editSearch     ||
                    watched == ui->tableTransact  ||
                    watched == ui->treeStock      ||
                    watched == ui->tabStock       ||
                    watched == ui->tabTransact    ||
                    watched == ui->tabWidget      ||
                    watched == ui->btnEditStock   ||
                    watched == ui->btnDelStock    ||
                    watched == ui->btnReport      ||
                    watched == ui->btnAddTransact ||
                    watched == ui->btnReloadData  ||
                    watched == ui->btnEditTransact||
                    watched == ui->btnDelTransact ||
                    watched == ui->comboProject) {
                procActSetSearchFilter(QString(""), 0);
                ui->editSearch->setText(QString(""));
                ui->comboProject->setCurrentIndex(0);
                return true;
            }
        }
    }
    return QObject::eventFilter(watched, event);
}

// -------------------- Control Events --------------------------------
// -------------------- buttons ---------------------------------------
void MainWindow::on_btnInventoryEditor_clicked()
{
    actInventoryEditorOpen->trigger();
}

void MainWindow::on_btnOrderEditor_clicked()
{
    qDebug() << "project: "
             << ui->comboProject->currentData(Constants::RoleNodeId).toInt()
             << ui->comboProject->currentData(Qt::DisplayRole);
}

void MainWindow::on_btnAddTransact_clicked()
{
//    if (!ui->treeStock->selectionModel()->hasSelection() ||
//         ui->treeStock->selectionModel()->selectedIndexes().first().data(Constants::RoleNodeType).toInt() != Constants::ItemItem) {
//        QMessageBox::warning(this,
//                             "Ошибка!",
//                             "Выберите продукт на складе для создания записи о приходе/расходе.");
//        return;
//    }
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
    refreshTransact();
    procActSetSearchFilter(ui->editSearch->text(), ui->comboProject->currentData(Constants::RoleNodeId).toInt());
//    searchExpand();
}

void MainWindow::on_btnReport_clicked()
{
    actReportManagerOpen->trigger();
}

void MainWindow::on_btnDictEditor_clicked()
{
    actDictEditorOpen->trigger();
}

// ---------------------- other controls ------------------------------
void MainWindow::on_treeStock_doubleClicked(const QModelIndex &index)
{
    qDebug() << m_stockModel->getStockItemByIndex(m_stockSearchProxyModel->mapToSource(index));
    if (index.data(Constants::RoleNodeType).toInt() == Constants::ItemItem) {
        actTransactAdd->trigger();
    }
}

void MainWindow::on_tableTransact_doubleClicked(const QModelIndex &index)
{
    qDebug() << m_transactModel->getTransactItemByIndex(m_transactSearchProxyModel->mapToSource(index));
    actTransactEdit->trigger();
}

void MainWindow::on_editSearch_textChanged(const QString &arg1)
{    
    procActSetSearchFilter(arg1, ui->comboProject->currentData(Constants::RoleNodeId).toInt());
//    searchExpand();
    actRefreshView->trigger();
}

void MainWindow::on_comboProject_currentIndexChanged(int index)
{
    Q_UNUSED(index)
//    qDebug() << ui->comboProject->currentData(Constants::RoleNodeId).toInt();
    procActSetSearchFilter(ui->editSearch->text(), ui->comboProject->currentData(Constants::RoleNodeId).toInt());
//    searchExpand();
}

// -------------------- Delegates -------------------------------------
void MainWindow::StockBranchDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // TODO: FIX: refactor this crap
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

        QString searchStr = qobject_cast<const QSortFilterProxyModel *>(index.model())->filterRegExp().pattern();
        QString text = index.data(Qt::DisplayRole).toString();
        if (text.contains(searchStr, Qt::CaseInsensitive)) {
            qint32 pos = text.indexOf(searchStr, 0, Qt::CaseInsensitive);
            qint32 len = searchStr.size();
            QString hlstr = text.mid(pos, len);

            QFontMetrics fm = painter->fontMetrics();

            QRect trect = fm.boundingRect(hlstr);
            QRect shiftrect = fm.boundingRect(text.left(pos));

            shiftrect.moveTo(option.rect.topLeft());
            trect.moveTo(shiftrect.topRight());
            painter->fillRect(trect.adjusted(+4, 0, +6, 0), QColor(Qt::yellow));
        }

        if (cellSelected) {
//            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
//            pal.setColor(QPalette::ButtonText, pal.brightText().color());
            pal.setColor(QPalette::ButtonText, pal.text().color());
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
        iconPos.rx() += 3 + 10;
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

        QString searchStr = qobject_cast<const QSortFilterProxyModel *>(index.model())->filterRegExp().pattern();
        QString text = index.data(Qt::DisplayRole).toString();
        if (text.contains(searchStr, Qt::CaseInsensitive)) {
            qint32 pos = text.indexOf(searchStr, 0, Qt::CaseInsensitive);
            qint32 len = searchStr.size();
            QString hlstr = text.mid(pos, len);

            QFontMetrics fm = painter->fontMetrics();

            QRect trect = fm.boundingRect(hlstr);
            QRect shiftrect = fm.boundingRect(text.left(pos));

            shiftrect.moveTo(option.rect.topLeft());
            trect.moveTo(shiftrect.topRight());
            painter->fillRect(trect.adjusted(+4, 0, +6, 0), QColor(Qt::yellow));
        }

        if (cellSelected) {
//            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
//            pal.setColor(QPalette::ButtonText, pal.brightText().color());
            pal.setColor(QPalette::ButtonText, pal.text().color());
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

        QString searchStr = qobject_cast<const QSortFilterProxyModel *>(index.model())->filterRegExp().pattern();
        QString text = index.data(Qt::DisplayRole).toString();
        if (text.contains(searchStr, Qt::CaseInsensitive)) {
            qint32 pos = text.indexOf(searchStr, 0, Qt::CaseInsensitive);
            qint32 len = searchStr.size();
            QString hlstr = text.mid(pos, len);

            QFontMetrics fm = painter->fontMetrics();

            QRect trect = fm.boundingRect(hlstr);
            QRect shiftrect = fm.boundingRect(text.left(pos));

            shiftrect.moveTo(option.rect.topLeft());
            trect.moveTo(shiftrect.topRight());
            painter->fillRect(trect.adjusted(+4, 0, +6, 0), QColor(Qt::yellow));
        }

        if (cellSelected) {
//            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
//            pal.setColor(QPalette::ButtonText, pal.brightText().color());
            pal.setColor(QPalette::ButtonText, pal.text().color());
        }
        else {
            pal.setColor(QPalette::ButtonText, pal.text().color());
        }
        option.widget->style()->drawItemText(painter, option.rect.adjusted(+25 + 20, 0, 0, 0),
                                             1, pal, true,
                                             index.data(Qt::DisplayRole).toString(),
                                             QPalette::ButtonText);
        break;
    }
    }
    painter->restore();
}

void MainWindow::StockTextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

        QString searchStr = qobject_cast<const QSortFilterProxyModel *>(index.model())->filterRegExp().pattern();
        QString text = index.data(Qt::DisplayRole).toString();
        if (text.contains(searchStr, Qt::CaseInsensitive)) {
            qint32 pos = text.indexOf(searchStr, 0, Qt::CaseInsensitive);
            qint32 len = searchStr.size();
            QString hlstr = text.mid(pos, len);

            QFontMetrics fm = painter->fontMetrics();

            QRect trect = fm.boundingRect(hlstr);
            QRect shiftrect = fm.boundingRect(text.left(pos));

            shiftrect.moveTo(option.rect.topLeft());
            trect.moveTo(shiftrect.topRight());
            painter->fillRect(trect.adjusted(+4, 0, +6, 0), QColor(Qt::yellow));
        }

        QPalette pal = option.palette;
        if (cellSelected) {
//            pal.setColor(QPalette::ButtonText, pal.brightText().color());
//            pal.setColor(QPalette::ButtonText, pal.highlightedText().color());
            pal.setColor(QPalette::ButtonText, pal.text().color());
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
