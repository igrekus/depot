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
    m_filteredGroupModel_1->addItemAtPosition(0, 0, QString("Все"));
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
    m_filteredGroupModel_2->addItemAtPosition(0, 0, QString("Все"));
}

void ReportManager::initDialog()
{
    m_filteredGroupModel_1 = new MapModel(this);
    m_filteredGroupModel_2 = new MapModel(this);
    m_reportModel = new QSqlQueryModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);

    m_proxyModel->setSourceModel(m_reportModel);
//    m_proxyModel->setSortLocaleAware(true);

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
           .setFromDate (ui->dateFrom->date())     // not used
           .setUntilDate(ui->dateUntil->date())    // not used
           .setSearch   (ui->editSearch_1->text())
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
           .setSearch   (ui->editSearch_2->text())
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
    ui->tableView->setColumnWidth(1, trwidth*0.10);
    ui->tableView->setColumnWidth(2, trwidth*0.10);
    ui->tableView->setColumnWidth(3, trwidth*0.10);
    ui->tableView->setColumnWidth(4, trwidth*0.25);
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
                       ui->comboGroup->currentText()+")("+
                       ui->editSearch_1->text()+").xlsx");
    } else {
        return QString("Приход-расход с "+
                       ui->dateFrom_2->date().toString(Qt::ISODate)+" по "+
                       ui->dateUntil_2->date().toString(Qt::ISODate)+" ("+
                       ui->comboProject_2->currentText()+")("+
                       ui->comboCategory_2->currentText()+")("+
                       ui->comboGroup_2->currentText()+")("+
                       ui->editSearch_2->text()+").xlsx");
    }
}

void ReportManager::xlsxWriteDocumentHeader(QXlsx::Document &doc,
                                    const QXlsx::CellReference &topleft,
                                    const QVector<qint32> columnWidths,
                                    const QString &caption)
{
    {qint32 i=0;
    for (const auto &it : columnWidths) {
        doc.setColumnWidth(topleft.column()+i,  it);
        ++i;
    }}

    {QXlsx::Format fmt;
    fmt.setBorderColor(QColor(Qt::black));
    fmt.setBorderStyle(QXlsx::Format::BorderThin);
    fmt.setPatternBackgroundColor(QColor(QRgb(hdrCaptionBg)));
    doc.write(topleft, caption, fmt);
    doc.mergeCells(QXlsx::CellRange(topleft.row(), topleft.column(), topleft.row(), topleft.column()+2), fmt);}

    {QXlsx::Format fmt;
    fmt.setLeftBorderColor(QColor(Qt::black));
    fmt.setLeftBorderStyle(QXlsx::Format::BorderThin);
    fmt.setRightBorderColor(QColor(Qt::black));
    fmt.setRightBorderStyle(QXlsx::Format::BorderThin);
    doc.write(topleft.row()+1, topleft.column(), "Тема: "+ui->comboProject->currentText(), fmt);
    doc.mergeCells(QXlsx::CellRange(topleft.row()+1, topleft.column(), topleft.row()+1, topleft.column()+2), fmt);
    doc.write(topleft.row()+2, topleft.column(), "Категория: "+ui->comboCategory->currentText(), fmt);
    doc.mergeCells(QXlsx::CellRange(topleft.row()+2, topleft.column(), topleft.row()+2, topleft.column()+2), fmt);
    doc.write(topleft.row()+3, topleft.column(), "Группа: "+ui->comboGroup->currentText(), fmt);
    doc.mergeCells(QXlsx::CellRange(topleft.row()+3, topleft.column(), topleft.row()+3, topleft.column()+2), fmt);
    doc.write(topleft.row()+4, topleft.column(),"Ключевое слово в наименовании: "+ui->editSearch_1->text(), fmt);
    doc.mergeCells(QXlsx::CellRange(topleft.row()+4, topleft.column(), topleft.row()+4, topleft.column()+2), fmt);}

    {QXlsx::Format fmt;
    fmt.setBorderColor(QColor(Qt::black));
    fmt.setBorderStyle(QXlsx::Format::BorderThin);
    fmt.setPatternBackgroundColor(QColor(QRgb(hdrDateBg)));
    doc.write(topleft.row()+5, topleft.column(),"Отчёт составлен: "+QDate::currentDate().toString(Qt::ISODate), fmt);
    doc.mergeCells(QXlsx::CellRange(topleft.row()+5, topleft.column(), topleft.row()+5, topleft.column()+2), fmt);}
}

void ReportManager::xlsxWriteTableHeader(QXlsx::Document &doc,
                                         const QXlsx::CellReference &topleft,
                                         QSortFilterProxyModel *model)
{
    QXlsx::Format fmt;
    fmt.setBorderColor(QColor(Qt::black));
    fmt.setBorderStyle(QXlsx::Format::BorderThin);
    fmt.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    fmt.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    fmt.setFontBold(true);
    fmt.setPatternBackgroundColor(QColor(QRgb(hdrCaptionBg)));
    for (qint32 i=1; i<model->columnCount(); ++i) {
        doc.write(topleft.row(), topleft.column()+i-1, model->headerData(i, Qt::Horizontal).toString(), fmt);
    }
}

void ReportManager::xlsxWriteTable(QXlsx::Document &doc,
                                   const QXlsx::CellReference &topleft,
                                   QSortFilterProxyModel *model)
{
    {QXlsx::Format fmt;
    fmt.setLeftBorderColor(QColor(Qt::black));
    fmt.setLeftBorderStyle(QXlsx::Format::BorderThin);
    fmt.setRightBorderColor(QColor(Qt::black));
    fmt.setRightBorderStyle(QXlsx::Format::BorderThin);
    fmt.setBottomBorderColor(QColor(Qt::lightGray));
    fmt.setBottomBorderStyle(QXlsx::Format::BorderThin);
    fmt.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    fmt.setTextWarp(true);
    QXlsx::Format strFmt(fmt);
    strFmt.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    // TODO: fix this if needed moe reports (new class - tablewriter?)
    if (m_reportType == ReportStock) {
        QXlsx::Format numFmt(fmt);
        numFmt.setHorizontalAlignment(QXlsx::Format::AlignRight);
        for (qint32 i=0; i<model->rowCount(); ++i) {
            doc.write(topleft.row()+1+i, topleft.column()+0, model->data(model->index(i, 1)).toInt()   , numFmt);
            doc.write(topleft.row()+1+i, topleft.column()+1, model->data(model->index(i, 2)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+2, model->data(model->index(i, 3)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+3, model->data(model->index(i, 4)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+4, model->data(model->index(i, 5)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+5, model->data(model->index(i, 6)).toInt()   , numFmt);
            doc.write(topleft.row()+1+i, topleft.column()+6, model->data(model->index(i, 7)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+7, model->data(model->index(i, 8)).toString(), strFmt);
        }
    }
    if (m_reportType == ReportTransact) {
        QXlsx::Format goodNumFmt(fmt);
        goodNumFmt.setFontColor(QColor(QRgb(Constants::ColorReceipt)));
        goodNumFmt.setPatternBackgroundColor(QColor(QRgb(Constants::ColorReceiptBackground)));
        QXlsx::Format badNumFmt(fmt);
        badNumFmt.setFontColor(QColor(QRgb(Constants::ColorExpense)));
        badNumFmt.setPatternBackgroundColor(QColor(QRgb(Constants::ColorExpenseBackground)));
        for (qint32 i=0; i<model->rowCount(); ++i) {
            doc.write(topleft.row()+1+i, topleft.column()+0, model->data(model->index(i, 1)).toDate().toString(Qt::ISODate), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+1, model->data(model->index(i, 2)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+2, model->data(model->index(i, 3)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+3, model->data(model->index(i, 4)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+4, model->data(model->index(i, 5)).toString(), strFmt);
            qint32 qty = model->data(model->index(i, 6)).toInt();
            doc.write(topleft.row()+1+i, topleft.column()+5, qty, qty>=0?goodNumFmt:badNumFmt);
            doc.write(topleft.row()+1+i, topleft.column()+6, model->data(model->index(i, 7)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+7, model->data(model->index(i, 8)).toString(), strFmt);
            doc.write(topleft.row()+1+i, topleft.column()+8, model->data(model->index(i, 9)).toString(), strFmt);
        }
    }}

    QXlsx::Format fmt;
    fmt.setBorderColor(QColor(Qt::black));
    fmt.setBorderStyle(QXlsx::Format::BorderThin);
    fmt.setPatternBackgroundColor(QColor(QRgb(hdrCaptionBg)));
    fmt.setFontBold(true);
    fmt.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    fmt.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    doc.mergeCells(QXlsx::CellRange(topleft.row()+1+model->rowCount(),
                                    topleft.column(),
                                    topleft.row()+1+model->rowCount(),
                                    topleft.column()+3), fmt);
    doc.mergeCells(QXlsx::CellRange(topleft.row()+1+model->rowCount(),
                                    topleft.column()+6,
                                    topleft.row()+1+model->rowCount(),
                                    topleft.column()+6+(model->columnCount()-1-1-6)), fmt);
    doc.write(topleft.row()+1+model->rowCount(), topleft.column()+4, "ИТОГО:", fmt);
    fmt.setHorizontalAlignment(QXlsx::Format::AlignRight);
    QXlsx::CellRange sumRange(topleft.row()+1,                             topleft.column()+5,
                              topleft.row()+1+model->rowCount()-1, topleft.column()+5);
    doc.write(topleft.row()+1+model->rowCount(), topleft.column()+5, "=SUM("+sumRange.toString()+")", fmt);
}

void ReportManager::saveStockReport()
{
//    QString fname = m_reportDir+m_exportFileName;
    QString fname = "d:/"+m_exportFileName;

    QXlsx::Document xlsx;

    // doc position on the sheet
    QXlsx::CellReference hdrTopLeft("A1");

    // write dco header
    {QVector<qint32> widths = {5, 25, 30, 45, 15, 8, 15, 10};
    QString caption("Отчёт об остатках на складе на "+QDate::currentDate().toString(Qt::ISODate));
    xlsxWriteDocumentHeader(xlsx, hdrTopLeft, widths, caption);}

    // write table header
    QXlsx::CellReference tblTopLeft(hdrTopLeft.row()+7, hdrTopLeft.column());
    xlsxWriteTableHeader(xlsx, tblTopLeft, m_proxyModel);

    // write table
    xlsxWriteTable(xlsx, tblTopLeft, m_proxyModel);

    xlsx.saveAs(fname);
}

void ReportManager::saveTransactReport()
{
    //    QString fname = m_reportDir+m_exportFileName;
        QString fname = "d:/"+m_exportFileName;

        QXlsx::Document xlsx;

        // doc position on the sheet
        QXlsx::CellReference hdrTopLeft("A1");

        // write doc header
        {QVector<qint32> widths = {12, 25, 30, 40, 15, 8, 17, 10, 15};
        QString caption("Отчёт о приходе/расходе на период с "+
                        ui->dateFrom_2->date().toString(Qt::ISODate)+" по "+
                        ui->dateUntil_2->date().toString(Qt::ISODate));
        xlsxWriteDocumentHeader(xlsx, hdrTopLeft, widths, caption);}

        // write table header
        QXlsx::CellReference tblTopLeft(hdrTopLeft.row()+7, hdrTopLeft.column());
        xlsxWriteTableHeader(xlsx, tblTopLeft, m_proxyModel);

        // write table
        xlsxWriteTable(xlsx, tblTopLeft, m_proxyModel);

        xlsx.saveAs(fname);

//        QXlsx::Format fmt;
//        fmt.setLeftBorderColor(QColor(Qt::black));
//        fmt.setLeftBorderStyle(QXlsx::Format::BorderThin);
//        fmt.setRightBorderColor(QColor(Qt::black));
//        fmt.setRightBorderStyle(QXlsx::Format::BorderThin);
//        fmt.setBottomBorderColor(QColor(Qt::lightGray));
//        fmt.setBottomBorderStyle(QXlsx::Format::BorderThin);
//        fmt.setVerticalAlignment(QXlsx::Format::AlignVCenter);
//        fmt.setHorizontalAlignment(QXlsx::Format::AlignLeft);
//        fmt.setTextWarp(true);

//        QXlsx::Format goodFmt(fmt);
//        goodFmt.setFontColor(QColor(QRgb(Constants::ColorReceipt)));
//        goodFmt.setPatternBackgroundColor(QColor(QRgb(Constants::ColorReceiptBackground)));
//        QXlsx::Format badFmt(fmt);
//        badFmt.setFontColor(QColor(QRgb(Constants::ColorExpense)));
//        badFmt.setPatternBackgroundColor(QColor(QRgb(Constants::ColorExpenseBackground)));
//        QXlsx::Format strFmt(fmt);
//        strFmt.setHorizontalAlignment(QXlsx::Format::AlignLeft);

//        for (qint32 i=0; i<m_reportModel->rowCount(); ++i) {
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+0, m_reportModel->data(m_reportModel->index(i, 1)).toDate().toString(Qt::ISODate), strFmt);
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+1, m_reportModel->data(m_reportModel->index(i, 2)).toString(), strFmt);
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+2, m_reportModel->data(m_reportModel->index(i, 3)).toString(), strFmt);
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+3, m_reportModel->data(m_reportModel->index(i, 4)).toString(), strFmt);
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+4, m_reportModel->data(m_reportModel->index(i, 5)).toString(), strFmt);
//            qint32 qty = m_reportModel->data(m_reportModel->index(i, 6)).toInt();
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+5, qty, qty>=0?goodFmt:badFmt);
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+6, m_reportModel->data(m_reportModel->index(i, 7)).toString(), strFmt);
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+7, m_reportModel->data(m_reportModel->index(i, 8)).toString(), strFmt);
//            xlsx.write(tblTopLeft.row()+1+i, tblTopLeft.column()+8, m_reportModel->data(m_reportModel->index(i, 9)).toString(), strFmt);
//        }
//        fmt = QXlsx::Format();
//        fmt.setBorderColor(QColor(Qt::black));
//        fmt.setBorderStyle(QXlsx::Format::BorderThin);
//        fmt.setPatternBackgroundColor(QColor(QRgb(0xffDDDDDD)));
//        fmt.setFontBold(true);
//        fmt.setHorizontalAlignment(QXlsx::Format::AlignLeft);
//        fmt.setVerticalAlignment(QXlsx::Format::AlignVCenter);
//        xlsx.mergeCells(QXlsx::CellRange(tblTopLeft.row()+1+m_reportModel->rowCount(),
//                                         tblTopLeft.column(),
//                                         tblTopLeft.row()+1+m_reportModel->rowCount(),
//                                         tblTopLeft.column()+3), fmt);
//        xlsx.mergeCells(QXlsx::CellRange(tblTopLeft.row()+1+m_reportModel->rowCount(),
//                                         tblTopLeft.column()+6,
//                                         tblTopLeft.row()+1+m_reportModel->rowCount(),
//                                         tblTopLeft.column()+8), fmt);
//        xlsx.write(tblTopLeft.row()+1+m_reportModel->rowCount(), tblTopLeft.column()+4, "ИТОГО:", fmt);
//        fmt.setHorizontalAlignment(QXlsx::Format::AlignRight);
//        QXlsx::CellRange sumRange(tblTopLeft.row()+1,                             tblTopLeft.column()+5,
//                                  tblTopLeft.row()+1+m_reportModel->rowCount()-1, tblTopLeft.column()+5);
//        xlsx.write(tblTopLeft.row()+1+m_reportModel->rowCount(), tblTopLeft.column()+5, "=SUM("+sumRange.toString()+")", fmt);

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
        m_reportModel->setHeaderData(8, Qt::Horizontal, "Место");
        ui->tableView->hideColumn(0);
        resizeStockTable();
        m_reportType = ReportStock;
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
        m_reportModel->setHeaderData(6, Qt::Horizontal, "+/-");
        m_reportModel->setHeaderData(7, Qt::Horizontal, "Тема");
        m_reportModel->setHeaderData(8, Qt::Horizontal, "Сотрудник");
        m_reportModel->setHeaderData(9, Qt::Horizontal, "Примечание");
        ui->tableView->hideColumn(0);
        resizeTransactTable();
        m_reportType = ReportTransact;
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
    if (m_reportType == ReportStock) {
        saveStockReport();
        QMessageBox::information(this,
                                 "Информация",
                                 "Отчёт экспортирован успешно.");
    }
    if (m_reportType == ReportTransact) {
        saveTransactReport();
        QMessageBox::information(this,
                                 "Информация",
                                 "Отчёт экспортирован успешно.");
    }
}
