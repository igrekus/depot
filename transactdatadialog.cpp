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

//    QStringList projects(m_dictModel->m_projectListModel->m_strList);
//    comProject = new QCompleter(projects, this);
//    comProject->setCaseSensitivity(Qt::CaseInsensitive);
//    comProject->setCompletionMode(QCompleter::PopupCompletion);

//    QStringList staff(m_dictModel->m_staffListModel->m_strList);
//    comStaff   = new QCompleter(staff, this);
//    comStaff->setCaseSensitivity(Qt::CaseInsensitive);
//    comStaff->setCompletionMode(QCompleter::PopupCompletion);

    ui->comboProject->setModel(m_dictModel->m_projectListModel);
//    ui->comboProject->setEditable(true);
//    ui->comboProject->setInsertPolicy(QComboBox::NoInsert);
//    ui->comboProject->setCompleter(comProject);

    ui->comboStaff->setModel(m_dictModel->m_staffListModel);
//    ui->comboStaff->setEditable(true);
//    ui->comboStaff->setInsertPolicy(QComboBox::NoInsert);
//    ui->comboStaff->setCompleter(comStaff);

    ui->lblProductCode->setVisible(false);
    ui->editProductCode->setVisible(false);

    updateWidgets();

    m_oldData = m_data;
}

TransactItem TransactDataDialog::getData()
{
    return m_data;
}

TransactItem TransactDataDialog::collectData()
{
    return (TransactItem::TransactItemBuilder()
            .setId(m_oldData.itemId)
            .setName(m_oldData.itemName)
            .setDate(ui->dateTransact->date())
            .setDiff(ui->spinDiff->value())
            .setNote(ui->editNote->text())
            .setStock(m_oldData.itemStockRef)
            .setStaff(ui->comboStaff->currentData(Constants::RoleNodeId).toInt())
            .setProject(ui->comboProject->currentData(Constants::RoleNodeId).toInt())
            .build());
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

void TransactDataDialog::on_spinDiff_valueChanged(int arg1)
{
//    if (arg1 > 0) {
//        ui->editNote->setText("Приход");
//    }
//    else {
//        ui->editNote->clear();
//    }
}

void TransactDataDialog::on_btnOk_clicked()
{
    if (ui->comboProject->currentData(Constants::RoleNodeId).toInt() == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите тему, на которую следует записать текущий приход/расход.");
        return;
    }
    if (ui->editNote->text().isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Введите комментарий, поясняющий информацию о текущем приходе/расходе.");
        return;
    }
    if (ui->spinDiff->value() == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Приход/расход не может быть равен нулю.");
        return;
    }

    m_data = collectData();
    accept();
}
