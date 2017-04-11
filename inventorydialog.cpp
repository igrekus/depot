#include "inventorydialog.h"
#include "ui_inventorydialog.h"

InventoryDialog::InventoryDialog(DataBaseManager *dbman, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDialog)
{
    ui->setupUi(this);

    m_dbman = dbman;

    m_inventoryModel = new InventoryModel(m_dbman, this);

    ui->treeInventory->setModel(m_inventoryModel);
}

InventoryDialog::~InventoryDialog()
{
    delete ui;
}

void InventoryDialog::initDialog()
{
    m_inventoryModel->initModel();
}

void InventoryDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
//    actRefreshView->trigger();
}


void InventoryDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
