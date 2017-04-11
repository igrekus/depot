#include "inventorydatadialog.h"
#include "ui_inventorydatadialog.h"

InventoryDataDialog::InventoryDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDataDialog)
{
    ui->setupUi(this);

}

InventoryDataDialog::~InventoryDataDialog()
{
    delete ui;
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
