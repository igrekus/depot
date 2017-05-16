#include "inventorydialog.h"
#include "ui_inventorydialog.h"

InventoryDialog::InventoryDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowFlags() | Qt::Window),
    ui(new Ui::InventoryDialog)
{
    ui->setupUi(this);

    createActions();
}

InventoryDialog::~InventoryDialog()
{
    delete ui;
}

void InventoryDialog::createActions()
{
    actRefreshView = new QAction("Обновить", this);
    connect(actRefreshView, &QAction::triggered, this, &InventoryDialog::procActRefreshView);

    actEdit = new QAction("Редктировать", this);
    connect(actEdit, &QAction::triggered, this, &InventoryDialog::procActEdit);

    actDelete= new QAction("Удалить", this);
    connect(actDelete, &QAction::triggered, this, &InventoryDialog::procActDelete);

    actCategoryAdd = new QAction("Добавить категорию", this);
    connect(actCategoryAdd, &QAction::triggered, this, &InventoryDialog::procActCategoryAdd);
    actCategoryEdit = new QAction("Редактировать категорию", this);
    connect(actCategoryEdit, &QAction::triggered, this, &InventoryDialog::procActCategoryEdit);
    actCategoryDelete = new QAction("Удалить категорию", this);
    connect(actCategoryDelete, &QAction::triggered, this, &InventoryDialog::procActCategoryDelete);

    actGroupAdd = new QAction("Добавить группу", this);
    connect(actGroupAdd, &QAction::triggered, this, &InventoryDialog::procActGroupAdd);
    actGroupEdit = new QAction("Редактировать группу", this);
    connect(actGroupEdit, &QAction::triggered, this, &InventoryDialog::procActGroupEdit);
    actGroupDelete = new QAction("Удалить группу", this);
    connect(actGroupDelete, &QAction::triggered, this, &InventoryDialog::procActGroupDelete);

    actInventoryAdd = new QAction("Добавить номенклатуру", this);
    connect(actInventoryAdd, &QAction::triggered, this, &InventoryDialog::procActInventoryAdd);
    actInventoryCopy = new QAction("Скопировать номенклатуру", this);
    connect(actInventoryCopy, &QAction::triggered, this, &InventoryDialog::procActInventoryCopy);
    actInventoryEdit = new QAction("Редактировать номеклатуру", this);
    connect(actInventoryEdit, &QAction::triggered, this, &InventoryDialog::procActInventoryEdit);
    actInventoryDelete = new QAction("Удалить номенклатуру", this);
    connect(actInventoryDelete, &QAction::triggered, this, &InventoryDialog::procActInventoryDelete);

    actRegisterStock = new QAction("Удалить номенклатуру", this);
    connect(actRegisterStock, &QAction::triggered, this, &InventoryDialog::procActRegisterStock);
}
void InventoryDialog::initDialog()
{
    m_inventoryModel = new InventoryModel(this);
    m_inventoryModel->setDatabaseManager(m_dbman);
    m_inventoryModel->setDictModel(m_dictModel);
    m_inventoryModel->initModel();

    // TODO: proxy crashes because of double click debug info
    m_searchProxyModel = new QSortFilterProxyModel(this);
    m_searchProxyModel->setSourceModel(m_inventoryModel);
    m_searchProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_searchProxyModel->setFilterWildcard("");
    m_searchProxyModel->setDynamicSortFilter(false);

    ui->treeInventory->setModel(m_inventoryModel);
//    ui->treeInventory->setModel(m_searchProxyModel);
    ui->treeInventory->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeInventory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeInventory->setUniformRowHeights(false);
    ui->treeInventory->setAlternatingRowColors(true);
    ui->treeInventory->setItemDelegateForColumn(2, new TextDelegate(ui->treeInventory));
    ui->treeInventory->setItemDelegateForColumn(3, new TextDelegate(ui->treeInventory));
    ui->treeInventory->setItemDelegateForColumn(4, new TextDelegate(ui->treeInventory));
    ui->treeInventory->setItemDelegateForColumn(5, new TextDelegate(ui->treeInventory));
    ui->treeInventory->hideColumn(1);

    actRefreshView->trigger();
}

void InventoryDialog::procActRefreshView()
{
    qint32 trwidth = ui->treeInventory->frameGeometry().width()-30;
    if (trwidth < 200) {
        trwidth = ui->grpInventory->frameGeometry().width()-50;
    }
    ui->treeInventory->hide();
    ui->treeInventory->setColumnWidth(0, trwidth*0.20);
//    ui->treeInventory->setColumnWidth(1, trwidth*0.05);
    ui->treeInventory->setColumnWidth(2, trwidth*0.35);
    ui->treeInventory->setColumnWidth(3, trwidth*0.05);
    ui->treeInventory->setColumnWidth(4, trwidth*0.10); //+5
    ui->treeInventory->setColumnWidth(5, trwidth*0.30);
    ui->treeInventory->show();
}

void InventoryDialog::procActEdit()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    switch (index.data(Constants::RoleNodeType).toInt()) {
    case Constants::ItemCategory:
        actCategoryEdit->trigger();
        break;
    case Constants::ItemGroup:
        actGroupEdit->trigger();
        break;
    case Constants::ItemItem:
        actInventoryEdit->trigger();
        break;
    default:
        break;
    }
}

void InventoryDialog::procActDelete()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    switch (index.data(Constants::RoleNodeType).toInt()) {
    case Constants::ItemCategory:
        actCategoryDelete->trigger();
        break;
    case Constants::ItemGroup:
        actGroupDelete->trigger();
        break;
    case Constants::ItemItem:
        actInventoryDelete->trigger();
        break;
    default:
        break;
    }
}

void InventoryDialog::procActCategoryAdd()
{
    bool ok;
    QString newName = QInputDialog::getText(this, "Добавить категорию",
                                         "Введите название:", QLineEdit::Normal,
                                         QString(), &ok);
    if (ok & !newName.isEmpty()) {
        newName.replace(0, 1, newName.at(0).toUpper());
        QModelIndex ind = m_inventoryModel->addCategory(newName);

        m_dictModel->updateCategoryList();

        ui->treeInventory->selectionModel()->clear();
        ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        treeUpdated = true;
    }
}

void InventoryDialog::procActCategoryEdit()
{
    // TODO: вынести одинаковые с editgroup участки в отдельный метод
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    bool ok;
    QString oldName = index.data(Qt::DisplayRole).toString();
    QString newName = QInputDialog::getText(this,
                                            "Изменить категорию",
                                            "Введите новое название",
                                            QLineEdit::Normal,
                                            oldName,
                                            &ok);
    if (ok && !oldName.isEmpty() && oldName != newName) {
        newName.replace(0, 1, newName.at(0).toUpper());
        m_inventoryModel->editCategory(index, newName);

        m_dictModel->updateCategoryList();

        treeUpdated = true;
    }
}

void InventoryDialog::procActCategoryDelete()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    if (index.data(Constants::RoleNodeHasChildren).toBool()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Нельзя удалить непустую категорию. "
                             "Сначала удалите все группы.");
        return;
    }
    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную категорию?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res == QMessageBox::Yes) {
        m_inventoryModel->deleteCategory(index);

        m_dictModel->updateCategoryList();

        treeUpdated = true;
    }
}

void InventoryDialog::procActGroupAdd()
{
    QModelIndex cur = ui->treeInventory->selectionModel()->selectedIndexes().first();
    QModelIndex pindex = [cur]() -> QModelIndex {
        switch (cur.data(Constants::RoleNodeType).toInt()) {
        case Constants::ItemCategory:
            return cur;
        case Constants::ItemGroup:
            return cur.parent();
        case Constants::ItemItem:
            return cur.parent().parent();   // TODO: FIX parent search model->getparentroot
        }
            return QModelIndex();
    }();
    bool ok;
    QString newName = QInputDialog::getText(this, "Добавить группу",
                                         "Введите название:", QLineEdit::Normal,
                                         QString(), &ok);

    if (ok & !newName.isEmpty()) {
        newName.replace(0, 1, newName.at(0).toUpper());
        QModelIndex ind = m_inventoryModel->addGroup(pindex, newName);

        m_dictModel->updateGroupList();
        m_dictModel->updateMapGroupToCategory();

        ui->treeInventory->selectionModel()->clear();
        ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        treeUpdated = true;
    }
}

void InventoryDialog::procActGroupEdit()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    bool ok;
    QString oldName = index.data(Qt::DisplayRole).toString();
    QString newName = QInputDialog::getText(this,
                                            "Изменить группу",
                                            "Введите новое название",
                                            QLineEdit::Normal,
                                            oldName,
                                            &ok);
    if (ok && !oldName.isEmpty() && oldName != newName) {
        newName.replace(0, 1, newName.at(0).toUpper());
        m_inventoryModel->editGroup(index, newName);

        m_dictModel->updateGroupList();
//        m_dictModel->updateMapGroupToCategory();

        treeUpdated = true;
    }
}

void InventoryDialog::procActGroupDelete()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    if (index.data(Constants::RoleNodeHasChildren).toBool()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Нельзя удалить непустую группу. "
                             "Сначала удалите все позиции хранения.");
        return;
    }
    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную группу?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res == QMessageBox::Yes) {
        m_inventoryModel->deleteGroup(index);

        m_dictModel->updateGroupList();
        m_dictModel->updateMapGroupToCategory();

        treeUpdated = true;
    }
}

void InventoryDialog::procActInventoryAdd()
{
    QModelIndex cur = ui->treeInventory->selectionModel()->selectedIndexes().first();
    QModelIndex pindex = [cur]() -> QModelIndex {
        switch (cur.data(Constants::RoleNodeType).toInt()) {
        case Constants::ItemGroup:
            return cur;
        case Constants::ItemItem:
            return cur.parent();
        }
        return QModelIndex();
    }();
// TODO: fix ind.parent chain;
    ProductItem dummyProduct = ProductItem::ProductItemBuilder()
                               .setGroup(pindex.data(Constants::RoleNodeId).toInt())
                               .setCategory(pindex.parent().data(Constants::RoleNodeId).toInt())
                               .setUnit("шт")
                               .build();

    InventoryDataDialog dialog(this);

    dialog.setData(dummyProduct)
          .setDictModel(m_dictModel)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QModelIndex ind = m_inventoryModel->addInventory(pindex, dialog.getData());

    m_dictModel->updateProductList();

    ui->treeInventory->selectionModel()->clear();
    ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void InventoryDialog::procActInventoryCopy()
{
    QModelIndex cur = ui->treeInventory->selectionModel()->selectedIndexes().first();

    ProductItem CopyProduct = m_inventoryModel->getProductItemByIndex(cur);
    CopyProduct.itemId = 0;

    InventoryDataDialog dialog(this);

    dialog.setData(CopyProduct)
          .setDictModel(m_dictModel)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QModelIndex ind = m_inventoryModel->addInventory(cur.parent(), dialog.getData());

    m_dictModel->updateProductList();

    ui->treeInventory->selectionModel()->clear();
    ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void InventoryDialog::procActInventoryEdit()
{
    QModelIndex cur = ui->treeInventory->selectionModel()->selectedIndexes().first();

    ProductItem oldProduct = m_inventoryModel->getProductItemByIndex(cur);

    InventoryDataDialog dialog(this);

    dialog.setData(oldProduct)
          .setDictModel(m_dictModel)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    m_inventoryModel->editInventory(cur, dialog.getData());

    m_dictModel->updateProductList();
}

void InventoryDialog::procActInventoryDelete()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную группу?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res == QMessageBox::Yes) {
        m_inventoryModel->deleteInventory(index);

        m_dictModel->updateProductList();
    }
}

void InventoryDialog::procActRegisterStock()
{
    QModelIndex cur = ui->treeInventory->selectionModel()->selectedIndexes().first();
    QModelIndex pindex = [cur]() -> QModelIndex {
        switch (cur.data(Constants::RoleNodeType).toInt()) {
        case Constants::ItemGroup:
            return cur;
            break;
        case Constants::ItemItem:
            return cur.parent();     // TODO: fix cur.parent chain
            break;
        }
        return QModelIndex();
    }();

    StockItem newStockItem = StockItem::StockItemBuilder()
                             .setName(cur.sibling(cur.row(), 2).data(Qt::DisplayRole).toString())
                             .setProduct(cur.data(Constants::RoleNodeId).toInt())
                             .build();

    StockDataDialog dialog(this);
    dialog.setData(newStockItem)
          .setDictModel(m_dictModel)
          .setDbManager(m_dbman)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    newStockItem = dialog.getData();

    m_dbman->insertStock(newStockItem);

    treeUpdated = true;

    QMessageBox::information(this,
                             "Информация",
                             "Позиция успешно зарегистрирована на складе.");
}

void InventoryDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    actRefreshView->trigger();
}

void InventoryDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    case QEvent::Resize:
//        actRefreshView->trigger();
//        break;
    default:
        break;
    }
}

void InventoryDialog::on_btnAddCategory_clicked()
{
    actCategoryAdd->trigger();
}

void InventoryDialog::on_btnAddGroup_clicked()
{
    if (!ui->treeInventory->selectionModel()->hasSelection()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите категорию для добавления группы.");
        return;
    }
    actGroupAdd->trigger();
}

void InventoryDialog::on_btnAddInventory_clicked()
{
    if ((!ui->treeInventory->selectionModel()->hasSelection()) ||
         (ui->treeInventory->selectionModel()->selectedIndexes().first().data(Constants::RoleNodeType) == Constants::ItemCategory)) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите группу для добавления номенклатуры.");
        return;
    }
    actInventoryAdd->trigger();
}

void InventoryDialog::on_btnCopy_clicked()
{
    if ((!ui->treeInventory->selectionModel()->hasSelection()) ||
         (ui->treeInventory->selectionModel()->selectedIndexes().first().data(Constants::RoleNodeType) != Constants::ItemItem)) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите номенклатуру для создания копии.");
        return;
    }
    actInventoryCopy->trigger();
}

void InventoryDialog::on_btnEdit_clicked()
{
    if (!ui->treeInventory->selectionModel()->hasSelection()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите запись для редактирования.");
        return;
    }
    actEdit->trigger();
}

void InventoryDialog::on_btnDelete_clicked()
{
    if (!ui->treeInventory->selectionModel()->hasSelection()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите запись для удаления.");
        return;
    }
    actDelete->trigger();
}


void InventoryDialog::on_btnRegisterStock_clicked()
{
    if ((!ui->treeInventory->selectionModel()->hasSelection()) ||
         (ui->treeInventory->selectionModel()->selectedIndexes().first().data(Constants::RoleNodeType) != Constants::ItemItem)) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите номенклатуру для регистрации на складе.");
        return;
    }
    actRegisterStock->trigger();
}


void InventoryDialog::on_treeInventory_doubleClicked(const QModelIndex &index)
{
    if (index.data(Constants::RoleNodeType) == Constants::ItemItem) {
        actInventoryEdit->trigger();
    }
    qDebug() << m_inventoryModel->getInventoryItemByIndex(index);
}


// ---------------------- column delegate ----------------------------
void InventoryDialog::TextDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    QStyleOptionViewItem opt = option;
    painter->save();
    painter->setPen(QColor(Qt::lightGray));

    if (index.column() != 0 ) {
        painter->drawLine(QLine(option.rect.x()-1, option.rect.y(),
                                option.rect.x()-1, option.rect.y()+option.rect.height()));
    }
    QStyledItemDelegate::paint(painter, option, index);
//    option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

//    bool mouseOver = (option.state & QStyle::State_MouseOver);
//    bool cellSelected = (option.state & QStyle::State_Selected);

//    if (mouseOver) {
//        QBrush br(QColor(208, 224, 240, 150));
//        painter->fillRect(option.rect, br);
//    }

//    QPalette pal = option.palette;
//    if (cellSelected) {
//        pal.setColor(QPalette::ButtonText, Qt::black);
//    }
//    else {
//        pal.setColor(QPalette::ButtonText, Qt::black);
//    }

//    // TODO: immediately invoked lambda
//    QString text;
//    if (index.column() != 3) {
//        text = index.data(Qt::DisplayRole).toString();
//    } else {
//        text = index.data(Qt::DisplayRole).toDateTime().toString(Qt::LocalDate);
//    }

//    QString search = index.data(ROLE_SEARCHSTR).toString();
//    if (text.contains(search, Qt::CaseInsensitive)) {
//        qint32 pos = text.indexOf(search, 0, Qt::CaseInsensitive);
//        qint32 len = search.size();
//        QString hlstr = text.mid(pos, len);

//        QFontMetrics fm = painter->fontMetrics();

//        QRect trect = fm.boundingRect(hlstr);
//        QRect shiftrect = fm.boundingRect(text.left(pos));

//        shiftrect.moveTo(option.rect.topLeft());
//        trect.moveTo(shiftrect.topRight());
//        painter->fillRect(trect.adjusted(+4, 0, +6, 0), QColor(Qt::yellow));
//    }

//    option.widget->style()->drawItemText(painter, option.rect.adjusted(+4, 0, 0, 0),
//                                         1, pal, true, text, QPalette::ButtonText);
    painter->restore();
}

// ------------------------- utility routines -----------------

// ------------------------- Testing routines ----------------
void InventoryDialog::testAddCat()
{
    for (int i=0; i<10; ++i) {
        QString id = Utility::rndString(12);
        id.replace(0, 1, id.at(0).toUpper());
        QModelIndex ind = m_inventoryModel->addCategory(id);
        ui->treeInventory->selectionModel()->clear();
        ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void InventoryDialog::testRemCat()
{
    qint32 count = m_inventoryModel->rowCount(QModelIndex());
    for (qint32 i=0; i<count; ++i) {
        QModelIndex ind = m_inventoryModel->index(i, 0);
        if (!ind.data(Constants::RoleNodeHasChildren).toBool()) {
            m_inventoryModel->deleteCategory(ind);
            --i;
            --count;
        }
    }
}

void InventoryDialog::testAddGrp()
{
    QModelIndex catind = ui->treeInventory->selectionModel()->selectedIndexes().first();
    for (int i=0; i<10; ++i) {
        QString id = Utility::rndString(12);
        id.replace(0, 1, id.at(0).toUpper());
        QModelIndex ind = m_inventoryModel->addGroup(catind, id);
        ui->treeInventory->selectionModel()->clear();
        ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
//        qDebug() << id;
    }
}

void InventoryDialog::testRemGrp()
{
    QModelIndex catind = ui->treeInventory->selectionModel()->selectedIndexes().first();
    qint32 count = m_inventoryModel->rowCount(catind);
    for (qint32 i=0; i<count; ++i) {
        QModelIndex ind = m_inventoryModel->index(i, 0, catind);
        if (!ind.data(Constants::RoleNodeHasChildren).toBool()) {
            m_inventoryModel->deleteGroup(ind);
            --i;
            --count;
        }
    }
}

QString Utility::rndString(qint32 len)
{
    static const QString charset =
    "0123456789"
//        "АБВГДЕЁЖЗИКЛМНОПРСТУФХЦЧшщЪЫЬЭЮЯ"
    "абвгдеёжзиклмнопрстуфхцчшщъыьэюя";
    QString out;
    while (len>0) {
        qint32 chr = qrand() % (charset.size() - 1);
        out.append(charset.at(chr));
        --len;
    }
    return out;
}
