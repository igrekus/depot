#include "inventorydialog.h"
#include "ui_inventorydialog.h"

InventoryDialog::InventoryDialog(DataBaseManager *dbman, DictModel *dict, QWidget *parent) :
    QDialog(parent, Qt::WindowFlags() | Qt::Window),
    ui(new Ui::InventoryDialog)
{
    ui->setupUi(this);

    createActions();

    m_dbman = dbman;
    m_dictModel = dict;

    m_inventoryModel = new InventoryModel(m_dbman, this);

    ui->treeInventory->setModel(m_inventoryModel);
    ui->treeInventory->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeInventory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeInventory->setUniformRowHeights(false);
    ui->treeInventory->setAlternatingRowColors(true);
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

    actAddCategory = new QAction("Добавить категорию", this);
    connect(actAddCategory, &QAction::triggered, this, &InventoryDialog::procActAddCategory);
    actEditCategory = new QAction("Редактировать категорию", this);
    connect(actEditCategory, &QAction::triggered, this, &InventoryDialog::procActEditCategory);
    actDeleteCategory = new QAction("Удалить категорию", this);
    connect(actDeleteCategory, &QAction::triggered, this, &InventoryDialog::procActDeleteCategory);

    actAddGroup = new QAction("Добавить группу", this);
    connect(actAddGroup, &QAction::triggered, this, &InventoryDialog::procActAddGroup);
    actEditGroup = new QAction("Редактировать группу", this);
    connect(actEditGroup, &QAction::triggered, this, &InventoryDialog::procActEditGroup);
    actDeleteGroup = new QAction("Удалить группу", this);
    connect(actDeleteGroup, &QAction::triggered, this, &InventoryDialog::procActDeleteGroup);

    actAddInventory = new QAction("Добавить номенклатуру", this);
    connect(actAddInventory, &QAction::triggered, this, &InventoryDialog::procActAddInventory);
    actEditInventory = new QAction("Редактировать номеклатуру", this);
    connect(actEditInventory, &QAction::triggered, this, &InventoryDialog::procActEditInventory);
    actDeleteInventory = new QAction("Удалить номенклатуру", this);
    connect(actDeleteInventory, &QAction::triggered, this, &InventoryDialog::procActDeleteInventory);
}
void InventoryDialog::initDialog()
{
    m_inventoryModel->initModel();

    actRefreshView->trigger();
}

void InventoryDialog::procActRefreshView()
{
    qint32 trwidth = ui->treeInventory->frameGeometry().width()-30;
    ui->treeInventory->hide();
    ui->treeInventory->setColumnWidth(0, trwidth*0.20);
    ui->treeInventory->setColumnWidth(1, trwidth*0.05);
    ui->treeInventory->setColumnWidth(2, trwidth*0.35);
    ui->treeInventory->setColumnWidth(3, trwidth*0.05);
    ui->treeInventory->setColumnWidth(4, trwidth*0.05);
    ui->treeInventory->setColumnWidth(5, trwidth*0.30);
    ui->treeInventory->show();
}

void InventoryDialog::procActEdit()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    switch (index.data(ROLE_NODE_TYPE).toInt()) {
    case StockItem::ItemCategory:
        actEditCategory->trigger();
        break;
    case StockItem::ItemGroup:
        actEditGroup->trigger();
        break;
    case StockItem::ItemItem:
        actEditInventory->trigger();
        break;
    default:
        break;
    }
}

void InventoryDialog::procActDelete()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    switch (index.data(ROLE_NODE_TYPE).toInt()) {
    case StockItem::ItemCategory:
        actDeleteCategory->trigger();
        break;
    case StockItem::ItemGroup:
        actDeleteGroup->trigger();
        break;
    case StockItem::ItemItem:
        actDeleteInventory->trigger();
        break;
    default:
        break;
    }
}

void InventoryDialog::procActAddCategory()
{
    bool ok;
    QString newName = QInputDialog::getText(this, "Добавить категорию",
                                         "Введите название:", QLineEdit::Normal,
                                         QString(), &ok);
    if (ok & !newName.isEmpty()) {
        newName.replace(0, 1, newName.at(0).toUpper());
        QModelIndex ind = m_inventoryModel->addCategory(newName);
        ui->treeInventory->selectionModel()->clear();
        ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void InventoryDialog::procActEditCategory()
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
    }
}

void InventoryDialog::procActDeleteCategory()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    if (index.data(ROLE_NODE_HAS_CHILDREN).toBool()) {
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
    }
}

void InventoryDialog::procActAddGroup()
{
    QModelIndex cur = ui->treeInventory->selectionModel()->selectedIndexes().first();
    QModelIndex pindex = [cur]() -> QModelIndex {
        switch (cur.data(ROLE_NODE_TYPE).toInt()) {
        case StockItem::ItemCategory:
            return cur;
        case StockItem::ItemGroup:
            return cur.parent();
        case StockItem::ItemItem:
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
        ui->treeInventory->selectionModel()->clear();
        ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void InventoryDialog::procActEditGroup()
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
    }
}

void InventoryDialog::procActDeleteGroup()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    if (index.data(ROLE_NODE_HAS_CHILDREN).toBool()) {
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
    }
}

void InventoryDialog::procActAddInventory()
{
    QModelIndex cur = ui->treeInventory->selectionModel()->selectedIndexes().first();
    QModelIndex pindex = [cur]() -> QModelIndex {
        switch (cur.data(ROLE_NODE_TYPE).toInt()) {
        case StockItem::ItemGroup:
            return cur;
        case StockItem::ItemItem:
            return cur.parent();
    }
    }();

    ProductItem dummyProduct = ProductItem::ProductItemBuilder()
                               .setGroup(pindex.data(ROLE_NODE_ID).toInt())
                               .setCategory(pindex.parent().data(ROLE_NODE_ID).toInt())
                               .build();

    InventoryDataDialog dialog(this);

    dialog.setData(dummyProduct)
          .setCategoryMap(m_dictModel->m_mapCategory)
          .setGropMap(m_dictModel->m_mapGroup)
          .setComboLink(m_dictModel->m_mapGroupToCategory)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QModelIndex ind = m_inventoryModel->addInventory(pindex, dialog.getData());
    ui->treeInventory->selectionModel()->clear();
    ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
}

void InventoryDialog::procActEditInventory()
{
    QModelIndex cur = ui->treeInventory->selectionModel()->selectedIndexes().first();

    ProductItem oldProduct = m_inventoryModel->getProductItemByIndex(cur);

    InventoryDataDialog dialog(this);

    dialog.setData(oldProduct)
          .setCategoryMap(m_dictModel->m_mapCategory)
          .setGropMap(m_dictModel->m_mapGroup)
          .setComboLink(m_dictModel->m_mapGroupToCategory)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    m_inventoryModel->editInventory(cur, dialog.getData());
}

void InventoryDialog::procActDeleteInventory()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную группу?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res == QMessageBox::Yes) {
        m_inventoryModel->deleteInventory(index);
    }
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
    actAddCategory->trigger();
}

void InventoryDialog::on_btnAddGroup_clicked()
{
    if (!ui->treeInventory->selectionModel()->hasSelection()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите категорию для добавления группы.");
        return;
    }
    actAddGroup->trigger();
}

void InventoryDialog::on_btnAddInventory_clicked()
{
    if ((!ui->treeInventory->selectionModel()->hasSelection()) ||
         (ui->treeInventory->selectionModel()->selectedIndexes().first().data(ROLE_NODE_TYPE) == InventoryItem::ItemCategory)) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите группу для добавления номенклатуры.");
        return;
    }
    actAddInventory->trigger();
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
        if (!ind.data(ROLE_NODE_HAS_CHILDREN).toBool()) {
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
        if (!ind.data(ROLE_NODE_HAS_CHILDREN).toBool()) {
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

