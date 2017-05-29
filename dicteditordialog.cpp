#include "dicteditordialog.h"
#include "ui_dicteditordialog.h"

DictEditorDialog::DictEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DictEditorDialog)
{
    ui->setupUi(this);
}

DictEditorDialog::~DictEditorDialog()
{
    delete ui;
}

void DictEditorDialog::initDialog()
{
    m_dictListModel = new QStringListModel(dictList, this);

    ui->comboDict->setModel(m_dictListModel);
    ui->comboDict->setInsertPolicy(QComboBox::NoInsert);
    ui->comboDict->setCurrentIndex(0);
}

void DictEditorDialog::loadDict(MapModel *dict)
{
    // TODO: load from db if needed
    ui->listDict->setModel(dict);
    m_currentListModel = dict;
}

void DictEditorDialog::addRecord(MapModel *dict, const qint32 tableId, const QString &newname)
{
    qint32 newId = m_dbman->insertDictRecord(tableList.at(tableId), newname);

    dict->addItem(newId, newname);
}

void DictEditorDialog::editRecord(MapModel *dict, const qint32 tableId, const qint32 recId, const QString &newname)
{
    m_dbman->updateDictRecord(tableList.at(tableId), recId, newname);

    dict->editItem(recId, newname);
}

void DictEditorDialog::deleteRecord(MapModel *dict, const qint32 tableId, const qint32 recId)
{
    m_dbman->deleteDictRecord(tableList.at(tableId), recId);

    dict->removeItem(recId);
}

void DictEditorDialog::on_comboDict_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        loadDict(m_dictModel->m_locationListModel);
        m_dictTableId = index;
        break;
    case 1:
        loadDict(m_dictModel->m_staffListModel);
        m_dictTableId = index;
        break;
    case 2:
        loadDict(m_dictModel->m_projectListModel);
        m_dictTableId = index;
        break;
    }
}

void DictEditorDialog::on_btnAdd_clicked()
{
     bool ok;
     QString newName = QInputDialog::getText(this, "Добавить запись",
                                          "Введите название:", QLineEdit::Normal,
                                          QString(), &ok);
     if (ok & !newName.isEmpty()) {
         newName.replace(0, 1, newName.at(0).toUpper());
         addRecord(m_currentListModel, m_dictTableId, newName);
     }
}

void DictEditorDialog::on_btnEdit_clicked()
{
    if (!ui->listDict->selectionModel()->hasSelection()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите запись для редактирования.");
        return;
    }
    bool ok;
    QString newName = QInputDialog::getText(this, "Добавить запись",
                                         "Введите название:", QLineEdit::Normal,
                                         ui->listDict->currentIndex().data(Qt::DisplayRole).toString(), &ok);
    if (ok & !newName.isEmpty()) {
        newName.replace(0, 1, newName.at(0).toUpper());
        editRecord(m_currentListModel, m_dictTableId,
                   ui->listDict->currentIndex().data(Constants::RoleNodeId).toInt(),
                   newName);
    }
}

void DictEditorDialog::on_btnDelete_clicked()
{
    // TODO: FK checks
    if (!ui->listDict->selectionModel()->hasSelection()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите запись для удаления.");
        return;
    }
    qint32 res = QMessageBox::question(this,
                                       "Внимание!",
                                       "Вы действительно хотите удалить выбранную запись о приходе/расходе?",
                                       QMessageBox::Yes,
                                       QMessageBox::No | QMessageBox::Default);
    if (res == QMessageBox::Yes) {
        deleteRecord(m_currentListModel, m_dictTableId,
                     ui->listDict->currentIndex().data(Constants::RoleNodeId).toInt());
    }
}
