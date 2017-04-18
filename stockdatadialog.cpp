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

void StockDataDialog::initDialog()
{
    if (m_data.itemId == 0) {
        setWindowTitle("Добавить позицию хранения:");
    } else {
        setWindowTitle("Изменить позицию хранения:");
    }

    ui->comboLocation->setModel(m_dictModel->m_locationListModel);
    ui->comboProject->setModel(m_dictModel->m_projectListModel);

    qDebug() << m_data;
//    ui->listProduct->setModel();
//    ui->editFullname->setText(m_data.itemFullname);
//    ui->editMiscTag->setText("");
//    ui->editName->setText(m_data.itemName);
//    ui->editSerialn->setText(m_data.itemSerialn);
//    ui->editUnit->setText(m_data.itemUnit);

////    ui->comboCategory->setCurrentIndex(0);

//    ui->comboCategory->setCurrentText(m_dictModel->m_categoryListModel->getData(m_data.itemCategoryRef));
//    ui->comboGroup->setCurrentText(m_dictModel->m_groupListModel->getData(m_data.itemGroupRef));

    m_oldData = m_data;
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
