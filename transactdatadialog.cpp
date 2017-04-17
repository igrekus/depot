#include "transactdatadialog.h"
#include "ui_transactdatadialog.h"

TransactDataDialog::TransactDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactDataDialog)
{
    ui->setupUi(this);
}

TransactDataDialog::~TransactDataDialog()
{
    delete ui;
}

void TransactDataDialog::changeEvent(QEvent *e)
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
