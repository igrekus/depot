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

    m_searchProxyModel = new RecursiveFilterProxyModel(this);
    m_searchProxyModel->setSourceModel(m_inventoryModel);
    m_searchProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_searchProxyModel->setFilterWildcard("");
    m_searchProxyModel->setFilterRole(Constants::RoleSearchString);

    ui->treeInventory->setModel(m_searchProxyModel);
    ui->treeInventory->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeInventory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeInventory->setUniformRowHeights(false);
//    ui->treeInventory->setAlternatingRowColors(true);
    ui->treeInventory->setItemDelegate(new DelegateHighligtableTreeText(ui->treeInventory));
    ui->treeInventory->header()->setStretchLastSection(true);
    ui->treeInventory->hideColumn(1); // hide "code" column

    showMaximized();

    actRefreshView->trigger();
}

void InventoryDialog::procActRefreshView()
{
    qint32 trwidth = ui->treeInventory->frameGeometry().width()-30;
    if (trwidth < 200) {
        trwidth = ui->grpInventory->frameGeometry().width()-50;
    }
    ui->treeInventory->hide();
    ui->treeInventory->setColumnWidth(0, trwidth*0.15);
//    ui->treeInventory->setColumnWidth(1, trwidth*0.05); // -5, hidden code column
    ui->treeInventory->setColumnWidth(2, trwidth*0.55);
    ui->treeInventory->setColumnWidth(3, trwidth*0.05);
    ui->treeInventory->setColumnWidth(4, trwidth*0.10);   // +5
    ui->treeInventory->setColumnWidth(5, trwidth*0.15);
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
    case Constants::ItemClass:
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
    case Constants::ItemClass:
    default:
        break;
    }
}

void InventoryDialog::procActCategoryAdd()
{
    QModelIndexList path;
    getPathToRoot(ui->treeInventory->selectionModel()->selectedIndexes().first(), path);

    QModelIndex parentIndex = m_searchProxyModel->mapToSource(path.last());

    bool ok;
    QString newName = QInputDialog::getText(this, "Добавить категорию",
                                            "Введите название:", QLineEdit::Normal,
                                            QString(), &ok);
    if (ok & !newName.isEmpty()) {
        newName.replace(0, 1, newName.at(0).toUpper());

        QModelIndex index = m_inventoryModel->addCategory(parentIndex, newName);

        m_dictModel->updateCategoryList();

        ui->treeInventory->selectionModel()->clear();
        ui->treeInventory->selectionModel()->setCurrentIndex(m_searchProxyModel->mapFromSource(index),
                                                             QItemSelectionModel::Select | QItemSelectionModel::Rows);
        treeUpdated = true;
    }
}

void InventoryDialog::procActCategoryEdit()
{
    QModelIndex index = m_searchProxyModel->mapToSource(ui->treeInventory->selectionModel()->selectedIndexes().
                                                        first());
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
    QModelIndex sourceIndex = m_searchProxyModel->mapToSource(index);

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
        m_inventoryModel->deleteCategory(sourceIndex);

        m_dictModel->updateCategoryList();

        treeUpdated = true;
    }
}

void InventoryDialog::procActGroupAdd()
{
    QModelIndexList path;
    getPathToRoot(ui->treeInventory->selectionModel()->selectedIndexes().first(), path);

    QModelIndex parentIndex = m_searchProxyModel->mapToSource(path.end()[-1-1]);

    bool ok;
    QString newName = QInputDialog::getText(this, "Добавить группу",
                                         "Введите название:", QLineEdit::Normal,
                                         QString(), &ok);

    if (ok & !newName.isEmpty()) {
        newName.replace(0, 1, newName.at(0).toUpper());
        QModelIndex index = m_inventoryModel->addGroup(parentIndex, newName);

        m_dictModel->updateGroupList();
        m_dictModel->updateMapGroupToCategory();

        ui->treeInventory->selectionModel()->clear();
        ui->treeInventory->selectionModel()->setCurrentIndex(m_searchProxyModel->mapFromSource(index), QItemSelectionModel::Select | QItemSelectionModel::Rows);
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
        m_inventoryModel->editGroup(m_searchProxyModel->mapToSource(index), newName);

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
                             "Сначала удалите всю номенклатуру.");
        return;
    }
    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную группу?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res == QMessageBox::Yes) {
        m_inventoryModel->deleteGroup(m_searchProxyModel->mapToSource(index));

        m_dictModel->updateGroupList();
        m_dictModel->updateMapGroupToCategory();

        treeUpdated = true;
    }
}

void InventoryDialog::procActInventoryAdd()
{
    // TODO: merge with copy method
    QModelIndexList path;
    if (ui->treeInventory->selectionModel()->hasSelection()) {
        getPathToRoot(ui->treeInventory->selectionModel()->selectedIndexes().first(), path);
    }

//    qDebug() << "path: " << path.size();

    InventoryDataDialog::DialogData dummyData;

    if (path.size() > 2) {
        dummyData = InventoryDataDialog::DialogData(ProductItem::ProductItemBuilder().build(),
                                                    ProductRelation::ProductRelationBuilder()
                                                    .setClass   (path.end()[-1-0].data(Constants::RoleNodeId).toInt())
                                                    .setCategory(path.end()[-1-1].data(Constants::RoleNodeId).toInt())
                                                    .setGroup   (path.end()[-1-2].data(Constants::RoleNodeId).toInt())
                                                    .build()); // TODO: extract to a method
    } else {
        dummyData = InventoryDataDialog::DialogData(ProductItem::ProductItemBuilder().build(),
                                                    ProductRelation::ProductRelationBuilder()
                                                    .build()); // TODO: extract to a method
    }

    InventoryDataDialog dialog(this);

    dialog.setDictModel(m_dictModel)
          .setData(dummyData)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    InventoryDataDialog::DialogData recievedData = dialog.getData();

    QModelIndex parentIndex = m_inventoryModel->getParentIndexByRelation(recievedData.relation);

    QModelIndex index = m_inventoryModel->addInventory(parentIndex, recievedData.relation, recievedData.item);

    m_dictModel->updateProductList();

    QModelIndex indexToSelect = m_searchProxyModel->mapFromSource(index);
    QModelIndexList ePath;
    getPathToRoot(indexToSelect, ePath);
    expandTreePath(ePath);
    ui->treeInventory->selectionModel()->clear();
    ui->treeInventory->selectionModel()->setCurrentIndex(m_searchProxyModel->mapFromSource(index), QItemSelectionModel::Select | QItemSelectionModel::Rows);

    treeUpdated = true;
}

void InventoryDialog::procActInventoryCopy()
{
    QModelIndex proxyIndex = ui->treeInventory->selectionModel()->selectedIndexes().first();

    QModelIndex sourceIndex = m_searchProxyModel->mapToSource(proxyIndex);
    QModelIndex parentIndex = sourceIndex.parent();

    QModelIndexList path;
    getPathToRoot(sourceIndex, path);

    ProductItem copyProduct = m_inventoryModel->getProductItemByIndex(sourceIndex);
    copyProduct.itemId = 0;

    InventoryDataDialog::DialogData dummyData(copyProduct,
                                              ProductRelation::ProductRelationBuilder()
                                              .setClass   (path.end()[-1-0].data(Constants::RoleNodeId).toInt())
                                              .setCategory(path.end()[-1-1].data(Constants::RoleNodeId).toInt())
                                              .setGroup   (path.end()[-1-2].data(Constants::RoleNodeId).toInt())
                                              .build());

    InventoryDataDialog dialog(this);

    dialog.setDictModel(m_dictModel)
          .setData(dummyData)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    InventoryDataDialog::DialogData recievedData(dialog.getData());

    QModelIndex index = m_inventoryModel->addInventory(parentIndex, recievedData.relation, recievedData.item);

    m_dictModel->updateProductList();

    QModelIndex indexToSelect = m_searchProxyModel->mapFromSource(index);
    QModelIndexList ePath;
    getPathToRoot(indexToSelect, ePath);
    expandTreePath(ePath);
    ui->treeInventory->selectionModel()->clear();
    ui->treeInventory->selectionModel()->setCurrentIndex(m_searchProxyModel->mapFromSource(index), QItemSelectionModel::Select | QItemSelectionModel::Rows);

    treeUpdated = true;
}

void InventoryDialog::procActInventoryEdit()
{
    QModelIndex proxyIndex = ui->treeInventory->selectionModel()->selectedIndexes().first();

    QModelIndex sourceIndex = m_searchProxyModel->mapToSource(proxyIndex);
//    QModelIndex parentIndex = sourceIndex.parent();

    QModelIndexList path;
    getPathToRoot(sourceIndex, path);

    ProductItem editProduct = m_inventoryModel->getProductItemByIndex(sourceIndex);

    InventoryDataDialog::DialogData editData(editProduct,
                                             ProductRelation::ProductRelationBuilder()
                                             .setClass   (path.end()[-1-0].data(Constants::RoleNodeId).toInt())
                                             .setCategory(path.end()[-1-1].data(Constants::RoleNodeId).toInt())
                                             .setGroup   (path.end()[-1-2].data(Constants::RoleNodeId).toInt())
                                             .build());

    InventoryDataDialog dialog(this);

    dialog.setDictModel(m_dictModel)
          .setData(editData)
          .initDialog();

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    InventoryDataDialog::DialogData recievedData(dialog.getData());

    QModelIndex index = m_inventoryModel->editInventory(sourceIndex, recievedData.relation, recievedData.item);

    m_dictModel->updateProductList();

    // TODO: extract to selectNode method
    QModelIndex indexToSelect = m_searchProxyModel->mapFromSource(index);
    QModelIndexList ePath;
    getPathToRoot(indexToSelect, ePath);
    expandTreePath(ePath);
    ui->treeInventory->selectionModel()->clear();
    ui->treeInventory->selectionModel()->setCurrentIndex(m_searchProxyModel->mapFromSource(index), QItemSelectionModel::Select | QItemSelectionModel::Rows);

    treeUpdated = true;
}

void InventoryDialog::procActInventoryDelete()
{
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную номенклатуру?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res == QMessageBox::Yes) {
        m_inventoryModel->deleteInventory(m_searchProxyModel->mapToSource(index));

        m_dictModel->updateProductList();

        treeUpdated = true;
    }
}

void InventoryDialog::procActRegisterStock()
{
    // FIXME: ХАК: работает в обход модели, переписать на работу со стокмоделью
    QModelIndex proxyIndex = ui->treeInventory->selectionModel()->selectedIndexes().first();
    QModelIndex sourceIndex = m_searchProxyModel->mapToSource(proxyIndex);

    StockItem newStockItem = StockItem::StockItemBuilder()
                             .setName(sourceIndex.sibling(sourceIndex.row(), 2).data(Qt::DisplayRole).toString())
                             .setProduct(sourceIndex.data(Constants::RoleNodeId).toInt())
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
    if (newStockItem.itemAmount < 0)
        newStockItem.itemAmount = 0;

    // TODO: check stock for dupes
    qint32 newStockId = m_dbman->insertStock(newStockItem);

    if (newStockItem.itemAmount > 0) {
        TransactItem newTransact(TransactItem::TransactItemBuilder()
                                 .setDate(QDate::currentDate())
                                 .setDiff(newStockItem.itemAmount)
                                 .setNote(QString("Поступление"))
                                 .setStock(newStockId)
                                 .setStaff(22)         // TODO: FIX: add default staff
                                 .setProject(newStockItem.itemProject)
                                 .build());
        m_dbman->insertTransact(newTransact);
    }

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
    if (!ui->treeInventory->selectionModel()->hasSelection()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите раздел для добавления категории.");
        return;
    }
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
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    if (index.data(Constants::RoleNodeType).toInt() == Constants::ItemClass) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите категорию для добавления группы.");
        return;
    }
    actGroupAdd->trigger();
}

void InventoryDialog::on_btnAddInventory_clicked()
{
//    if (!ui->treeInventory->selectionModel()->hasSelection()) {
//        QMessageBox::warning(this,
//                             "Ошибка!",
//                             "Выберите группу для добавления номенклатуры.");
//        return;
//    }

//    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
//    if ((index.data(Constants::RoleNodeType).toInt() == Constants::ItemClass) ||
//        (index.data(Constants::RoleNodeType).toInt() == Constants::ItemCategory)) {
//        QMessageBox::warning(this,
//                             "Ошибка!",
//                             "Выберите группу для добавления номенклатуры.");
//        return;
//    }
    actInventoryAdd->trigger();
}

void InventoryDialog::on_btnCopy_clicked()
{
    if (!ui->treeInventory->selectionModel()->hasSelection() ||
        (ui->treeInventory->selectionModel()->selectedIndexes().first().data(Constants::RoleNodeType).toInt() != Constants::ItemItem)) {
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
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    if (index.data(Constants::RoleNodeType).toInt() == Constants::ItemClass) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Нельзя редактировать раздел.");
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
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    if (index.data(Constants::RoleNodeType).toInt() == Constants::ItemClass) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Нельзя удалить раздел.");
        return;
    }
    actDelete->trigger();
}

void InventoryDialog::on_btnRegisterStock_clicked()
{
    if (!ui->treeInventory->selectionModel()->hasSelection()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите номенклатуру для регистрации на складе.");
        return;
    }
    QModelIndex index = ui->treeInventory->selectionModel()->selectedIndexes().first();
    if (index.data(Constants::RoleNodeType) != Constants::ItemItem) {
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
//    qDebug() << m_inventoryModel->getInventoryItemByIndex(m_searchProxyModel->mapToSource(index));
}

void InventoryDialog::on_editSearch_textChanged(const QString &arg1)
{
    if (arg1.size() == 0)
        ui->treeInventory->collapseAll();
    m_searchProxyModel->setFilterWildcard(arg1);
    if (arg1.size() > 2)
        ui->treeInventory->expandAll();
}

// ------------------------- utility routines -----------------
void InventoryDialog::getPathToRoot(const QModelIndex &node, QModelIndexList &path)
{
    // TODO: exstract to separate TreeHelper class
    if (!node.isValid())
        return;
    path.append(node);
    getPathToRoot(node.parent(), path);
}

void InventoryDialog::expandTreePath(const QModelIndexList &path)
{
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        ui->treeInventory->expand(*it);
    }
}

// ------------------------- Testing routines ----------------
void InventoryDialog::testAddCat()
{
//    for (int i=0; i<10; ++i) {
//        QString id = Utility::rndString(12);
//        id.replace(0, 1, id.at(0).toUpper());
//        QModelIndex ind = m_inventoryModel->addCategory(id);
//        ui->treeInventory->selectionModel()->clear();
//        ui->treeInventory->selectionModel()->setCurrentIndex(ind, QItemSelectionModel::Select | QItemSelectionModel::Rows);
//    }
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

