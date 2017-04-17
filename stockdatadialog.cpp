#include "stockdatadialog.h"
#include "ui_stockdatadialog.h"

StockDataDialog::StockDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StockDataDialog)
{
    ui->setupUi(this);
}

StockDataDialog::~StockDataDialog()
{
    delete ui;
}

void StockDataDialog::changeEvent(QEvent *e)
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
