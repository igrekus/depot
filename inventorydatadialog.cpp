#include "inventorydatadialog.h"
#include "ui_inventorydatadialog.h"

InventoryDataDialog::InventoryDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDataDialog)
{
    ui->setupUi(this);

    m_categoryListModel = new MapModel(this);
    m_groupListModel    = new MapModel(this);

    ui->comboCategory->setModel(m_categoryListModel);
    ui->comboGroup->setModel(m_groupListModel);
}

InventoryDataDialog::~InventoryDataDialog()
{
    delete ui;
}

void InventoryDataDialog::filterGroupCombo(const qint32 catId)
{
    if (!m_groupListModel->isEmpty()) {
        m_groupListModel->clear();
    }

    HashDict tmpdict;
    for (const auto &it : m_comboMap.values(catId)) {
        QString str = m_grpMap.id.value(it);
        tmpdict.id.insert(it, str);
        tmpdict.di.insert(str, it);
    }

    m_groupListModel->initModel(tmpdict);
}

void InventoryDataDialog::initDialog()
{
    if (m_data.itemId == 0) {
        setWindowTitle("Добавить номенклатуру:");
        m_newRecord = true;
    } else {
        setWindowTitle("Изменить номенклатуру:");
        m_newRecord = false;
    }

    m_categoryListModel->initModel(m_catMap);
    ui->comboCategory->setCurrentIndex(0);

    ui->editFullname->setText(m_data.itemFullname);
    ui->editMiscTag->setText("");
    ui->editName->setText(m_data.itemName);
    ui->editSerialn->setText(m_data.itemSerialn);
    ui->editUnit->setText(m_data.itemUnit);
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

void InventoryDataDialog::on_comboCategory_currentIndexChanged(int index)
{
    filterGroupCombo(ui->comboCategory->currentData(ROLE_NODE_ID).toInt());
    ui->comboGroup->setCurrentIndex(0);
    emit ui->comboGroup->currentTextChanged("");
}

void InventoryDataDialog::on_comboGroup_currentTextChanged(const QString &arg1)
{
    if (ui->comboGroup->count() == 0) {
        ui->comboGroup->setDisabled(true);
        ui->editFullname->setDisabled(true);
        ui->editName->setDisabled(true);
        ui->editSerialn->setDisabled(true);
        ui->editUnit->setDisabled(true);
    } else {
        ui->comboGroup->setDisabled(false);
        ui->editFullname->setDisabled(false);
        ui->editName->setDisabled(false);
        ui->editSerialn->setDisabled(false);
        ui->editUnit->setDisabled(false);
    }
}
