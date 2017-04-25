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

void TransactDataDialog::updateWidgets()
{
    ui->editProductName->setText(m_data.itemName);
    ui->editProductCode->setText("здесь будет код товара в базе");
    ui->dateTransact->setDate(m_data.itemDate);
    ui->comboStaff->setCurrentText(m_dictModel->m_staffListModel->getData(m_data.itemStaffRef));
    ui->comboProject->setCurrentText(m_dictModel->m_projectListModel->getData(m_data.itemProjectRef));
    ui->spinDiff->setValue(m_data.itemDiff);
    ui->editNote->setText(m_data.itemNote);
}

void TransactDataDialog::initDialog()
{
    if (m_data.itemId == 0) {
        setWindowTitle("Новая запись о приходе/расходе");
    } else {
        setWindowTitle("Редактировать запись о приходе/расходе");
    }

    ui->comboProject->setModel(m_dictModel->m_projectListModel);
    ui->comboStaff->setModel(m_dictModel->m_staffListModel);

    updateWidgets();

    m_oldData = m_data;
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
