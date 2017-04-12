#include "inventorydatadialog.h"
#include "ui_inventorydatadialog.h"

InventoryDataDialog::InventoryDataDialog(const ProductItem &item,
                                         const LinkedDict  &link,
                                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDataDialog)
{
    ui->setupUi(this);

    m_data = item;
    m_comboLink = link;
}

InventoryDataDialog::~InventoryDataDialog()
{
    delete ui;
}

void InventoryDataDialog::initDialog()
{

}

void InventoryDataDialog::changeEvent(QEvent *e)
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
