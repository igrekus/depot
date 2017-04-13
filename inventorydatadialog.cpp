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

    ui->editFullname->setText(m_data.itemFullname);
    ui->editMiscTag->setText("");
    ui->editName->setText(m_data.itemName);
    ui->editSerialn->setText(m_data.itemSerialn);
    ui->editUnit->setText(m_data.itemUnit);

    m_categoryListModel->initModel(m_catMap);
    ui->comboCategory->setCurrentIndex(0);

    ui->comboCategory->setCurrentText(m_categoryListModel->getData(m_data.itemCategoryRef));
    ui->comboGroup->setCurrentText(m_groupListModel->getData(m_data.itemGroupRef));

    m_oldData = m_data;
}

ProductItem InventoryDataDialog::getData()
{
    return m_data;
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

ProductItem InventoryDataDialog::collectData()
{
    return (ProductItem::ProductItemBuilder()
            .setId(m_data.itemId)
            .setName(ui->editName->text())
            .setFullname(ui->editFullname->text())
            .setSerialn(ui->editSerialn->text())
            .setUnit(ui->editUnit->text())
            .setMiscTag(ui->editMiscTag->text())
            .setGroup(ui->comboGroup->currentData(ROLE_NODE_ID).toInt())
            .setCategory(ui->comboCategory->currentData(ROLE_NODE_ID).toInt())
            .build());
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

void InventoryDataDialog::on_btnOk_clicked()
{
    if (ui->editName->text().isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Поле наименования категории не должно быть пустым!");
        return;
    }
    m_data = collectData();

    if (!(m_data == m_oldData)) {
        m_dataModified = true;
        accept();
    } else {
        m_dataModified = false;
        reject();
    }
}
