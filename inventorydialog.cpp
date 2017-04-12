#include "inventorydialog.h"
#include "ui_inventorydialog.h"

InventoryDialog::InventoryDialog(DataBaseManager *dbman, QWidget *parent) :
    QDialog(parent, Qt::WindowFlags() | Qt::Window),
    ui(new Ui::InventoryDialog)
{
    ui->setupUi(this);

    createActions();

    m_dbman = dbman;

    m_inventoryModel = new InventoryModel(m_dbman, this);

    ui->treeInventory->setModel(m_inventoryModel);
    ui->treeInventory->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeInventory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeInventory->setUniformRowHeights(false);
    ui->treeInventory->setAlternatingRowColors(true);

    actRefreshView->trigger();
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
    qDebug() << "edit press";
}

void InventoryDialog::procActDelete()
{
    qDebug() << "delete press";
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
    qDebug() << "edit cat";
}

void InventoryDialog::procActDeleteCategory()
{
    qDebug() << "del cat";
}

void InventoryDialog::procActAddGroup()
{
    qDebug() << "add grp";
}

void InventoryDialog::procActEditGroup()
{
    qDebug() << "edit grp";
}

void InventoryDialog::procActDeleteGroup()
{
    qDebug() << "del grp";
}

void InventoryDialog::procActAddInventory()
{
    qDebug() << "add inv";
}

void InventoryDialog::procActEditInventory()
{
    qDebug() << "edit inv";
}

void InventoryDialog::procActDeleteInventory()
{
    qDebug() << "del inv";
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
    actAddGroup->trigger();
}

void InventoryDialog::on_btnAddInventory_clicked()
{
    actAddInventory->trigger();
}

void InventoryDialog::on_btnEdit_clicked()
{
    actEdit->trigger();
}

void InventoryDialog::on_btnDelete_clicked()
{
    actDelete->trigger();
}
