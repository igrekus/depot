#include "reportmanager.h"
#include "ui_reportmanager.h"

ReportManager::ReportManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportManager)
{
    ui->setupUi(this);
}

ReportManager::~ReportManager()
{
    delete ui;
}

void ReportManager::filterGroupCombo_1(const qint32 catId)
{
    if (!m_filteredGroupModel_1->isEmpty()) {
        m_filteredGroupModel_1->clear();
    }

    if (catId == 0) {
        m_filteredGroupModel_1->m_mapData = m_dictModel->m_groupListModel->m_mapData;
        m_filteredGroupModel_1->m_strList = m_dictModel->m_groupListModel->m_strList;
        return;
    }

    HashDict tmpdict;
    for (const auto &it : m_dictModel->m_mapGroupToCategory.values(catId)) {
        QString str = m_dictModel->m_groupListModel->getData(it);
        tmpdict.id.insert(it, str);
        tmpdict.di.insert(str, it);
    }

    m_filteredGroupModel_1->initModel(tmpdict);
    m_filteredGroupModel_1->addItem(0, 0, QString("Все"));
}

void ReportManager::filterGroupCombo_2(const qint32 catId)
{
    if (!m_filteredGroupModel_2->isEmpty()) {
        m_filteredGroupModel_2->clear();
    }

    if (catId == 0) {
        m_filteredGroupModel_2->m_mapData = m_dictModel->m_groupListModel->m_mapData;
        m_filteredGroupModel_2->m_strList = m_dictModel->m_groupListModel->m_strList;
        return;
    }

    HashDict tmpdict;
    for (const auto &it : m_dictModel->m_mapGroupToCategory.values(catId)) {
        QString str = m_dictModel->m_groupListModel->getData(it);
        tmpdict.id.insert(it, str);
        tmpdict.di.insert(str, it);
    }

    m_filteredGroupModel_2->initModel(tmpdict);
    m_filteredGroupModel_2->addItem(0, 0, QString("Все"));
}

void ReportManager::initDialog()
{
    m_filteredGroupModel_1 = new MapModel(this);
    m_filteredGroupModel_2 = new MapModel(this);

    ui->comboProject->setModel(m_dictModel->m_projectListModel);
    ui->comboProject->setCurrentIndex(0);

    ui->comboCategory->setModel(m_dictModel->m_categoryListModel);
    ui->comboCategory->setCurrentIndex(0);

    ui->comboGroup->setModel(m_filteredGroupModel_1);
    ui->comboGroup->setCurrentIndex(0);

    ui->dateFrom->setDate(QDate::currentDate().addDays(-1));
    ui->dateUntil->setDate(QDate::currentDate());

    ui->comboProject_2->setModel(m_dictModel->m_projectListModel);
    ui->comboProject_2->setCurrentIndex(0);

    ui->comboCategory_2->setModel(m_dictModel->m_categoryListModel);
    ui->comboCategory_2->setCurrentIndex(0);

    ui->comboGroup_2->setModel(m_filteredGroupModel_2);
    ui->comboGroup_2->setCurrentIndex(0);

    ui->dateFrom_2->setDate(QDate::currentDate());
    ui->dateUntil_2->setDate(QDate::currentDate());
}

void ReportManager::changeEvent(QEvent *e)
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

void ReportManager::on_comboCategory_currentIndexChanged(int index)
{
    filterGroupCombo_1(ui->comboCategory->currentData(Constants::RoleNodeId).toInt());
    ui->comboGroup->setCurrentIndex(0);
//    emit ui->comboGroup->currentTextChanged("");
}

void ReportManager::on_comboCategory_2_currentIndexChanged(int index)
{
    filterGroupCombo_2(ui->comboCategory_2->currentData(Constants::RoleNodeId).toInt());
    ui->comboGroup_2->setCurrentIndex(0);
}

void ReportManager::on_comboGroup_currentTextChanged(const QString &arg1)
{
    if (ui->comboGroup->count() == 1) {
        ui->comboGroup->setEnabled(false);
    } else {
        ui->comboGroup->setEnabled(true);
    }
}

void ReportManager::on_comboGroup_2_currentTextChanged(const QString &arg1)
{
    if (ui->comboGroup_2->count() == 1) {
        ui->comboGroup_2->setEnabled(false);
    } else {
        ui->comboGroup_2->setEnabled(true);
    }
}

void ReportManager::on_btnShow_clicked()
{
    switch (ui->tabReport->currentIndex()) {
    case 0: {
        qDebug() << "stock stat request";
        break;
    }
    case 1: {
        qDebug() << "transact stat requst";
        break;
    }
    default:
        break;
    }
}
