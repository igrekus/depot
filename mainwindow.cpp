#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 TODO:
      - обработчик ошибок через throw, сделать в одном месте -- где?;
      - singleton для констант типа DATA_INVALID
      - переписать определение позиции для вставки из свитча на лямбды

создание объектов:
      - https://ru.wikipedia.org/wiki/%D0%A4%D0%B0%D0%B1%D1%80%D0%B8%D1%87%D0%BD%D1%8B%D0%B9_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4_(%D1%88%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F)
      - https://ru.wikipedia.org/wiki/%D0%A1%D1%82%D1%80%D0%BE%D0%B8%D1%82%D0%B5%D0%BB%D1%8C_(%D1%88%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F)


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
    m_projectTagListModel = new MapModel(this);

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
    m_dictModel->initModel();
    m_transactModel->initModel();

    m_categoryListModel->initModel(m_dictModel->m_mapCategory);
    m_projectTagListModel->initModel(m_dictModel->m_mapProjectTag);

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

//    actReFetchRootData = new QAction(tr("Fetch Data"), this);
//    connect(actReFetchRootData, &QAction::triggered, this, &MainWindow::procActReFetchRootData);

    actRefreshView = new QAction(tr("Refresh View"), this);
    connect(actRefreshView, &QAction::triggered, this, &MainWindow::procActRefreshView);

    actAddCategory = new QAction(tr("Add Category"), this);
    connect(actAddCategory, &QAction::triggered, this, &MainWindow::procActAddCategory);
    actEditCategory = new QAction(tr("Edit Category"), this);
    connect(actEditCategory, &QAction::triggered, this, &MainWindow::procActEditCategory);
    actDeleteCategory = new QAction(tr("Edit Category"), this);
    connect(actDeleteCategory, &QAction::triggered, this, &MainWindow::procActDeleteCategory);

    actAddGroup = new QAction(tr("Add Group"), this);
    connect(actAddGroup, &QAction::triggered, this, &MainWindow::procActAddGroup);
    actEditGroup = new QAction(tr("Edit Group"), this);
    connect(actEditGroup, &QAction::triggered, this, &MainWindow::procActEditGroup);
    actDeleteGroup = new QAction(tr("Delete Group"), this);
    connect(actDeleteGroup, &QAction::triggered, this, &MainWindow::procActDeleteGroup);

    actAddStock = new QAction(tr("Add Stock"), this);
    connect(actAddStock, &QAction::triggered, this, &MainWindow::procActAddStock);
    actEditStock = new QAction(tr("Edit Stock"), this);
    connect(actEditStock, &QAction::triggered, this, &MainWindow::procActEditStock);
    actDeleteStock = new QAction(tr("Delete Stock"), this);
    connect(actDeleteStock, &QAction::triggered, this, &MainWindow::procActDeleteStock);
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

void MainWindow::procActAddCategory()
{
    bool ok;
    QString catname = QInputDialog::getText(this, "Добавить категорию",
                                         "Введите название:", QLineEdit::Normal,
                                         QString(), &ok);
    if (ok & !catname.isEmpty()) {
        catname.replace(0, 1, catname.at(0).toUpper());
        QModelIndex ind = m_stockModel->addCategory(catname);
        ui->treeStock->selectionModel()->clear();
        ui->treeStock->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void MainWindow::procActEditCategory()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    bool ok;
    QString catName = indexes.first().data(Qt::DisplayRole).toString();
    QString newName = QInputDialog::getText(this,
                                            "Изменить категорию",
                                            "Введите новое название",
                                            QLineEdit::Normal,
                                            indexes.first().data(Qt::DisplayRole).toString(),
                                            &ok);
    if (ok && !catName.isEmpty() && catName != newName) {
        m_stockModel->editCategory(indexes.first(), newName);
    }
}

void MainWindow::procActDeleteCategory()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    if (indexes.first().data(ROLE_NODE_HAS_CHILDREN).toBool()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Нельзя удалить непустую категорию. "
                             "Сначала удалите все группы.");
        return;
    } else {
        qint32 res = QMessageBox::question(this,
                                           "Внимание!",
                                           "Вы действительно хотите удалить выбранную категорию?",
                                           QMessageBox::Yes,
                                           QMessageBox::No | QMessageBox::Default);
        if (res == QMessageBox::Yes) {
            m_stockModel->deleteCategory(indexes.first());
        } else {
            return;
        }
    }
}

void MainWindow::procActAddGroup()
{
    // TODO: перенести отсюда логику запроса выделения категории, индекс получать лямбдой
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    QModelIndex pindex;
    if (indexes.first().data(ROLE_NODE_TYPE).toInt() == StockItem::ItemCategory) {
        pindex = indexes.first();
    } else if (indexes.first().data(ROLE_NODE_TYPE).toInt() == StockItem::ItemGroup) {
        pindex = indexes.first().parent();
    } else if (indexes.first().data(ROLE_NODE_TYPE).toInt() == StockItem::ItemStock) {
        pindex = indexes.first().parent().parent();
    }
    bool ok;
    QString text = QInputDialog::getText(this, "Добавить группу",
                                         "Введите название:", QLineEdit::Normal,
                                         QString(), &ok);
    if (ok & !text.isEmpty()) {
        qDebug() << "add to:" << pindex.data(Qt::DisplayRole).toString() << text;
    }
}

void MainWindow::procActEditGroup()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    bool ok;
    QString text = QInputDialog::getText(this,
                                         "Изменить группу",
                                         "Введите новое название",
                                         QLineEdit::Normal,
                                         indexes.first().data(Qt::DisplayRole).toString(),
                                         &ok);
    if (ok && !text.isEmpty())
        qDebug() << text;
}

void MainWindow::procActDeleteGroup()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    qDebug() << "del group" << indexes.first().data(Qt::DisplayRole).toString()
                            << indexes.first().data(ROLE_NODE_ID).toInt();
}

void MainWindow::procActAddStock()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    if (indexes.first().data(ROLE_NODE_TYPE).toInt() == StockItem::ItemCategory) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите группу для добавления позиции хранения.");
        return;
    }
    QModelIndex pindex;
    if (indexes.first().data(ROLE_NODE_TYPE).toInt() == StockItem::ItemGroup) {
        pindex = indexes.first();
    } else if (indexes.first().data(ROLE_NODE_TYPE).toInt() == StockItem::ItemStock) {
        pindex = indexes.first().parent();
    }
    qDebug() << "custom add stock item dialog: select product + project + amount (auto-register positive transact)";
    qDebug() << "add to:" << pindex.data(Qt::DisplayRole).toString()
                          << pindex.data(ROLE_NODE_ID).toInt();
}

void MainWindow::procActEditStock()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    qDebug() << "custom edit stock item dialog: deny select product, select project + amount (auto transact)";
}

void MainWindow::procActDeleteStock()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    qDebug() << "del stock item:" << indexes.first().data(Qt::DisplayRole).toString()
                                  << indexes.first().data(ROLE_NODE_ID).toInt();
}

// -------------------- Control Events -----------------------------
void MainWindow::on_btnAddCategory_clicked()
{
    actAddCategory->trigger();
}

void MainWindow::on_btnAddGroup_clicked()
{
    // TODO: Перенести сюда логику запроса выделения категории
    actAddGroup->trigger();
}

void MainWindow::on_btnAddStock_clicked()
{
    actAddStock->trigger();
}

void MainWindow::on_btnEditStockItem_clicked()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите запись для редактирования.");
        return;
    }
    switch (indexes.first().data(ROLE_NODE_TYPE).toInt()) {
    case StockItem::ItemCategory:
        actEditCategory->trigger();
        break;
    case StockItem::ItemGroup:
        actEditGroup->trigger();
        break;
    case StockItem::ItemStock:
        actEditStock->trigger();
        break;
    default:
        break;
    }
}

void MainWindow::on_btnDeleteStockItem_clicked()
{
    QModelIndexList indexes = ui->treeStock->selectionModel()->selectedIndexes();
    if (indexes.isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите запись для удаления.");
        return;
    }
    switch (indexes.first().data(ROLE_NODE_TYPE).toInt()) {
    case StockItem::ItemCategory:
        actDeleteCategory->trigger();
        break;
    case StockItem::ItemGroup:
        actDeleteGroup->trigger();
        break;
    case StockItem::ItemStock:
        actDeleteStock->trigger();
        break;
    default:
        break;
    }
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
    case StockItem::ItemStock: {
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

    if (index.data(ROLE_NODE_TYPE).toInt() == StockItem::ItemStock) {
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
