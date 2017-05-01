#include "reportmanager.h"
#include "ui_reportmanager.h"

ReportManager::ReportManager(QWidget *parent) :
    QDialog(parent, Qt::WindowFlags() | Qt::Window),
    ui(new Ui::ReportManager)
{
    ui->setupUi(this);

    m_reportDir = QCoreApplication::applicationDirPath()+"/reports/";
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
    m_reportModel = new QSqlQueryModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);

    m_proxyModel->setSourceModel(m_reportModel);

    ui->tableView->setModel(m_proxyModel);
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
    ui->dateFrom_2->setDate(QDate::currentDate().addDays(-300));
    ui->dateUntil_2->setDate(QDate::currentDate());

    ui->dateFrom->hide();
    ui->lblFrom->hide();
    ui->dateUntil->hide();
    ui->lblUntil->hide();
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
    qint32 trwidth = ui->tableView->frameGeometry().width()-30;
    ui->tableView->hide();
    ui->tableView->setColumnWidth(1, trwidth*0.05);
    ui->tableView->setColumnWidth(2, trwidth*0.10);
    ui->tableView->setColumnWidth(3, trwidth*0.10);
    ui->tableView->setColumnWidth(4, trwidth*0.30);
    ui->tableView->setColumnWidth(5, trwidth*0.05);
    ui->tableView->setColumnWidth(6, trwidth*0.05);
    ui->tableView->setColumnWidth(7, trwidth*0.12);
    ui->tableView->setColumnWidth(8, trwidth*0.10);
    ui->tableView->setColumnWidth(9, trwidth*0.15);
    ui->tableView->show();
}

QString ReportManager::makeFileName()
{
    if (ui->tabReport->currentIndex() == 0) {
        return QString("Остаток на "+
                       QDate::currentDate().toString(Qt::ISODate)+" ("+
                       ui->comboProject->currentText()+")("+
                       ui->comboCategory->currentText()+")("+
                       ui->comboGroup->currentText()+").xlsx");
    } else {
        return QString("Приход-расход с "+
                       ui->dateFrom_2->date().toString(Qt::ISODate)+" по "+
                       ui->dateUntil_2->date().toString(Qt::ISODate)+" ("+
                       ui->comboProject_2->currentText()+")("+
                       ui->comboCategory_2->currentText()+")("+
                       ui->comboGroup_2->currentText()+").xlsx");
    }
}

void ReportManager::saveStockReport()
{
//    QString fname = m_reportDir+m_exportFileName;
    QString fname = "d:/"+m_exportFileName;
    QXlsx::Document xlsx;
    xlsx.write(1, 1, "Отчёт об остатках на складе на "+QDate::currentDate().toString(Qt::ISODate));
    xlsx.write(2, 1, "Тема: "+ui->comboProject->currentText());
    xlsx.write(3, 1, "Категория: "+ui->comboCategory->currentText());
    xlsx.write(4, 1, "Группа: "+ui->comboGroup->currentText());
    xlsx.write(5, 1, "Код");
    xlsx.write(5, 2, "Категория");
    xlsx.write(5, 3, "Группа");
    xlsx.write(5, 4, "Наименование");
    xlsx.write(5, 5, "Серийный №");
    xlsx.write(5, 6, "Остаток");
    xlsx.write(5, 7, "Тема");
    xlsx.write(5, 8, "Место хранения");
    for (qint32 i=0; i<m_reportModel->rowCount(); ++i) {
        xlsx.write(6+i, 1, m_reportModel->data(m_reportModel->index(i, 1)).toInt());
        xlsx.write(6+i, 2, m_reportModel->data(m_reportModel->index(i, 2)).toString());
        xlsx.write(6+i, 3, m_reportModel->data(m_reportModel->index(i, 3)).toString());
        xlsx.write(6+i, 4, m_reportModel->data(m_reportModel->index(i, 4)).toString());
        xlsx.write(6+i, 5, m_reportModel->data(m_reportModel->index(i, 5)).toString());
        xlsx.write(6+i, 6, m_reportModel->data(m_reportModel->index(i, 6)).toInt());
        xlsx.write(6+i, 7, m_reportModel->data(m_reportModel->index(i, 7)).toString());
        xlsx.write(6+i, 8, m_reportModel->data(m_reportModel->index(i, 8)).toString());
    }
    xlsx.saveAs(fname);
}

void ReportManager::saveTransactReport()
{
    //    QString fname = m_reportDir+m_exportFileName;
        QString fname = "d:/"+m_exportFileName;
        QXlsx::Document xlsx;
        xlsx.write(1, 1, "Отчёт о приходе/расходе на период с "+
                         ui->dateFrom_2->date().toString(Qt::ISODate)+" по "+
                         ui->dateUntil_2->date().toString(Qt::ISODate));
        xlsx.write(2, 1, "Тема: "+ui->comboProject->currentText());
        xlsx.write(3, 1, "Категория: "+ui->comboCategory->currentText());
        xlsx.write(4, 1, "Группа: "+ui->comboGroup->currentText());
        xlsx.write(5, 1, "Дата");
        xlsx.write(5, 2, "Категория");
        xlsx.write(5, 3, "Группа");
        xlsx.write(5, 4, "Наименование");
        xlsx.write(5, 5, "Серийный №");
        xlsx.write(5, 6, "Приход/Расход");
        xlsx.write(5, 7, "Тема");
        xlsx.write(5, 8, "Сотрудник");
        xlsx.write(5, 9, "Примечание");
        QXlsx::Format fmtgood;
        fmtgood.setFontColor(QColor(QRgb(0xff006100)));
        fmtgood.setPatternBackgroundColor(QColor(QRgb(Constants::ColorReceiptBackground)));
        fmtgood.setFontBold(true);
        QXlsx::Format fmtbad;
        fmtbad.setFontColor(QColor(QRgb(0xff9c0006)));
        fmtbad.setPatternBackgroundColor(QColor(QRgb(Constants::ColorExpenseBackground)));
        fmtbad.setFontBold(true);
        for (qint32 i=0; i<m_reportModel->rowCount(); ++i) {
            xlsx.write(6+i, 1, m_reportModel->data(m_reportModel->index(i, 1)).toDate().toString(Qt::ISODate));
            xlsx.write(6+i, 2, m_reportModel->data(m_reportModel->index(i, 2)).toString());
            xlsx.write(6+i, 3, m_reportModel->data(m_reportModel->index(i, 3)).toString());
            xlsx.write(6+i, 4, m_reportModel->data(m_reportModel->index(i, 4)).toString());
            xlsx.write(6+i, 5, m_reportModel->data(m_reportModel->index(i, 5)).toString());
            qint32 qty = m_reportModel->data(m_reportModel->index(i, 6)).toInt();
            xlsx.write(6+i, 6, qty, qty>=0?fmtgood:fmtbad);
            xlsx.write(6+i, 7, m_reportModel->data(m_reportModel->index(i, 7)).toString());
            xlsx.write(6+i, 8, m_reportModel->data(m_reportModel->index(i, 8)).toString());
            xlsx.write(6+i, 9, m_reportModel->data(m_reportModel->index(i, 9)).toString());
        }
        xlsx.saveAs(fname);
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
    Q_UNUSED(index)
    filterGroupCombo_1(ui->comboCategory->currentData(Constants::RoleNodeId).toInt());
    ui->comboGroup->setCurrentIndex(0);
//    emit ui->comboGroup->currentTextChanged("");
}

void ReportManager::on_comboCategory_2_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    filterGroupCombo_2(ui->comboCategory_2->currentData(Constants::RoleNodeId).toInt());
    ui->comboGroup_2->setCurrentIndex(0);
}

void ReportManager::on_comboGroup_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    if (ui->comboGroup->count() == 1) {
        ui->comboGroup->setEnabled(false);
    } else {
        ui->comboGroup->setEnabled(true);
    }
}

void ReportManager::on_comboGroup_2_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
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
        m_reportModel->setQuery(m_dbman->getStockStats(collectStockRequestData()));
        m_reportModel->setHeaderData(0, Qt::Horizontal, "Склад");
        m_reportModel->setHeaderData(1, Qt::Horizontal, "Код");
        m_reportModel->setHeaderData(2, Qt::Horizontal, "Категория");
        m_reportModel->setHeaderData(3, Qt::Horizontal, "Группа");
        m_reportModel->setHeaderData(4, Qt::Horizontal, "Наименование");
        m_reportModel->setHeaderData(5, Qt::Horizontal, "Сер. №");
        m_reportModel->setHeaderData(6, Qt::Horizontal, "Остаток");
        m_reportModel->setHeaderData(7, Qt::Horizontal, "Тема");
        m_reportModel->setHeaderData(8, Qt::Horizontal, "Место хран.");
        ui->tableView->hideColumn(0);
        resizeStockTable();
        break;
    }
    case 1: {
        m_reportModel->setQuery(m_dbman->getTransactStats(collectTransactRequestData()));
        m_reportModel->setHeaderData(0, Qt::Horizontal, "Транзакця №");
        m_reportModel->setHeaderData(1, Qt::Horizontal, "Дата");
        m_reportModel->setHeaderData(2, Qt::Horizontal, "Категория");
        m_reportModel->setHeaderData(3, Qt::Horizontal, "Группа");
        m_reportModel->setHeaderData(4, Qt::Horizontal, "Наименование");
        m_reportModel->setHeaderData(5, Qt::Horizontal, "Сер. №");
        m_reportModel->setHeaderData(6, Qt::Horizontal, "Кол-во");
        m_reportModel->setHeaderData(7, Qt::Horizontal, "Тема");
        m_reportModel->setHeaderData(8, Qt::Horizontal, "Сотрудник");
        m_reportModel->setHeaderData(9, Qt::Horizontal, "Примечание");
        ui->tableView->hideColumn(0);
        resizeTransactTable();
        break;
    }
    default:
        break;
    }
    m_exportFileName = makeFileName();
}

void ReportManager::on_btnExport_clicked()
{
    if (m_reportModel->rowCount() == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Нет данных для экспорта в Excel.");
        return;
    }
    if (ui->tabReport->currentIndex() == 0) {
        saveStockReport();
    } else {
        saveTransactReport();
    }
}
