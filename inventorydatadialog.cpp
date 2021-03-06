#include "inventorydatadialog.h"
#include "ui_inventorydatadialog.h"

InventoryDataDialog::InventoryDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDataDialog)
{
    m_filteredGroupModel = new MapModel(this);

    ui->setupUi(this);
}

InventoryDataDialog::~InventoryDataDialog()
{
    delete ui;
}

void InventoryDataDialog::filterGroupCombo(const qint32 catId)
{
    if (!m_filteredGroupModel->isEmpty()) {
        m_filteredGroupModel->clear();
    }

    HashDict tmpdict;
    for (const auto &it : m_dictModel->m_mapGroupToCategory.values(catId)) {
        QString str = m_dictModel->m_groupListModel->getData(it);
        tmpdict.id.insert(it, str);
        tmpdict.di.insert(str, it);
    }

    m_filteredGroupModel->initModel(tmpdict);
}

void InventoryDataDialog::updateWidgets()
{
    ui->editFullname->setText(m_data.item.itemFullname);
    ui->editMiscTag->setText("");
    ui->editName->setText(m_data.item.itemName);
    ui->editSerialn->setText(m_data.item.itemSerialn);
    ui->editUnit->setText(m_data.item.itemUnit);

    ui->comboCategory->setCurrentText(m_dictModel->m_categoryListModel->getData(m_data.relation.parentCategory));
    ui->comboGroup->setCurrentText(m_dictModel->m_groupListModel->getData(m_data.relation.parentGroup));
}

void InventoryDataDialog::initDialog()
{
    if (m_data.item.itemId == 0) {
        setWindowTitle("Добавить номенклатуру:");
    } else {
        setWindowTitle("Изменить номенклатуру:");
    }

    ui->comboCategory->setModel(m_dictModel->m_categoryListModel);
    ui->comboGroup->setModel(m_filteredGroupModel);

    updateWidgets();

    ui->editName->setFocus();

    m_oldData = m_data;
}

InventoryDataDialog::DialogData InventoryDataDialog::getData()
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

InventoryDataDialog::DialogData InventoryDataDialog::collectData()
{
    DialogData tmpdata(ProductItem::ProductItemBuilder()
                       .setId(m_oldData.item.itemId)
                       .setName(ui->editName->text())
                       .setFullname(ui->editFullname->text())
                       .setSerialn(ui->editSerialn->text())
                       .setUnit(ui->editUnit->text())
                       .setMiscTag(ui->editMiscTag->text())
                       .build(),
                       ProductRelation::ProductRelationBuilder()
                       .setClass(m_oldData.relation.parentClass)
                       .setCategory(ui->comboCategory->currentData(Constants::RoleNodeId).toInt())
                       .setGroup(ui->comboGroup->currentData(Constants::RoleNodeId).toInt())
                       .build());
    return tmpdata;
}

void InventoryDataDialog::on_comboCategory_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    filterGroupCombo(ui->comboCategory->currentData(Constants::RoleNodeId).toInt());
    ui->comboGroup->setCurrentIndex(0);
//    emit ui->comboGroup->currentTextChanged("");
}

void InventoryDataDialog::on_comboGroup_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
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
                             "Поле наименования номенклатуры не должно быть пустым.");
        return;
    }
    if (ui->comboGroup->currentText().isEmpty() ||
        ui->comboGroup->currentData(Constants::RoleNodeId).toInt() == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите группу для добавления номенклатуры.");
        return;
    }
    if (ui->comboCategory->currentData(Constants::RoleNodeId) == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите существующую категорию.");
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
