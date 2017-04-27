#include "reportmanager.h"
#include "ui_reportmanager.h"

ReportManager::ReportManager(QWidget *parent) :
    QDialog(parent, Qt::WindowFlags() | Qt::Window),
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
    reportModel = new QSqlQueryModel(this);
    proxyModel = new QSortFilterProxyModel(this);

    proxyModel->setSourceModel(reportModel);

    ui->tableView->setModel(proxyModel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->verticalHeader()->setDefaultSectionSize(14);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->horizontalHeader()->setFixedHeight(20);

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

//    ui->dateFrom_2->setDate(QDate::currentDate().addDays(-1));
//    ui->dateUntil_2->setDate(QDate::currentDate());
    ui->dateFrom_2->setDate(QDate::currentDate().addDays(-1000));
    ui->dateUntil_2->setDate(QDate::currentDate());
}

ReportRequest ReportManager::collectStockRequestData()
{
    return ReportRequest::ReportRequestBuilder()
           .setProject  (ui->comboProject->currentData(Constants::RoleNodeId).toInt())
           .setCategory (ui->comboCategory->currentData(Constants::RoleNodeId).toInt())
           .setGroup    (ui->comboGroup->currentData(Constants::RoleNodeId).toInt())
           .setFromDate (ui->dateFrom->date())
           .setUntilDate(ui->dateUntil->date())
           .build();
}

ReportRequest ReportManager::collectTransactRequestData()
{
    qint32 flag = 0;
    if (!ui->checkExpense->isChecked() && ui->checkReceipt->isChecked()) {
        flag = 1;
    }
    if (ui->checkExpense->isChecked() && !ui->checkReceipt->isChecked()) {
        flag = 2;
    }
    return ReportRequest::ReportRequestBuilder()
           .setProject  (ui->comboProject_2->currentData(Constants::RoleNodeId).toInt())
           .setCategory (ui->comboCategory_2->currentData(Constants::RoleNodeId).toInt())
           .setGroup    (ui->comboGroup_2->currentData(Constants::RoleNodeId).toInt())
           .setFromDate (ui->dateFrom_2->date())
           .setUntilDate(ui->dateUntil_2->date())
           .setFlag     (flag)
           .build();
}

void ReportManager::resizeStockTable()
{
    qint32 trwidth = ui->tableView->frameGeometry().width()-18;
    ui->tableView->hide();
    ui->tableView->setColumnWidth(1, trwidth*0.05);
    ui->tableView->setColumnWidth(2, trwidth*0.15);
    ui->tableView->setColumnWidth(3, trwidth*0.15);
    ui->tableView->setColumnWidth(4, trwidth*0.38);
    ui->tableView->setColumnWidth(5, trwidth*0.05);
    ui->tableView->setColumnWidth(6, trwidth*0.05);
    ui->tableView->setColumnWidth(7, trwidth*0.12);
    ui->tableView->setColumnWidth(8, trwidth*0.05);
    ui->tableView->show();
}

void ReportManager::resizeTransactTable()
{
//    qint32 trwidth = ui->tableView->frameGeometry().width()-30;
//    ui->tableView->hide();
//    ui->tableView->setColumnWidth(0, trwidth*0.20);
//    ui->tableView->setColumnWidth(1, trwidth*0.05);
//    ui->tableView->setColumnWidth(2, trwidth*0.35);
//    ui->tableView->setColumnWidth(3, trwidth*0.05);
//    ui->tableView->setColumnWidth(4, trwidth*0.05);
//    ui->tableView->setColumnWidth(5, trwidth*0.30);
//    ui->tableView->show();
}

void ReportManager::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if (ui->tabReport->currentIndex() == 0) {
        resizeStockTable();
    } else {
        resizeTransactTable();
    }
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
        reportModel->setQuery(m_dbman->getStockStats(collectStockRequestData()));
        reportModel->setHeaderData(0, Qt::Horizontal, "Склад");
        reportModel->setHeaderData(1, Qt::Horizontal, "Код");
        reportModel->setHeaderData(2, Qt::Horizontal, "Категория");
        reportModel->setHeaderData(3, Qt::Horizontal, "Группа");
        reportModel->setHeaderData(4, Qt::Horizontal, "Наименование");
        reportModel->setHeaderData(5, Qt::Horizontal, "Сер.№");
        reportModel->setHeaderData(6, Qt::Horizontal, "Остаток");
        reportModel->setHeaderData(7, Qt::Horizontal, "Тема");
        reportModel->setHeaderData(8, Qt::Horizontal, "Место хран.");
        ui->tableView->hideColumn(0);
        resizeStockTable();
        break;
    }
    case 1: {
        reportModel->setQuery(m_dbman->getTransactStats(collectTransactRequestData()));
        ui->tableView->hideColumn(0);
        break;
    }
    default:
        break;
    }

}
