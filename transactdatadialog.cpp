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

void TransactDataDialog::resetWidgets()
{
    ui->editProductName->setText(QString(""));
    ui->editProductCode->setText("здесь будет код товара в базе");
    ui->dateTransact->setDate(QDate::currentDate());
    ui->comboStaff->setCurrentIndex(0);
    ui->comboProject->setCurrentText(0);
    ui->spinDiff->setValue(0);
    ui->editNote->setText(QString(""));
    ui->editSearch->setText(QString(""));
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
    ui->editSearch->setText(m_data.itemName);
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

    m_searchFilterModel = new ProjectRecursiveFilterProxyModel(this);
    m_searchFilterModel->setSourceModel(m_stockModel);
    m_searchFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_searchFilterModel->setFilterWildcard(m_data.itemName);
    m_searchFilterModel->setFilterProjectId(0);

    ui->treeStock->setModel(m_searchFilterModel);
    ui->treeStock->hideColumn(2);
//    ui->treeStock->hideColumn(3);
    ui->treeStock->hideColumn(4);
    ui->treeStock->hideColumn(6);
    ui->treeStock->hideColumn(7);
    ui->treeStock->setIndentation(0);
    ui->treeStock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeStock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->treeStock->setUniformRowHeights(true);
    ui->treeStock->header()->setDefaultAlignment(Qt::AlignCenter); // Qt::TextWordWrap
    ui->treeStock->header()->setStretchLastSection(true);
    ui->treeStock->setItemDelegate(new DelegateHighligtableTreeText(ui->treeStock));
    ui->treeStock->setRootIsDecorated(false);
    ui->treeStock->setStyleSheet(QString("QTreeView::branch { border-image: none; }"));
    ui->treeStock->header()->moveSection(3, 5);

    refreshView();

    updateWidgets();

    connect(ui->treeStock->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &treeStockSelectionChanged);
}

void TransactDataDialog::refreshView()
{
    qint32 trwidth = ui->treeStock->frameGeometry().width()-30;
    ui->treeStock->hide();
    ui->treeStock->setColumnWidth(0, trwidth*0.20);
    ui->treeStock->setColumnWidth(1, trwidth*0.50);
    ui->treeStock->setColumnWidth(3, trwidth*0.10);
    ui->treeStock->setColumnWidth(5, trwidth*0.20);
    ui->treeStock->show();
}

TransactItem TransactDataDialog::getData()
{
    return m_data;
}

TransactItem TransactDataDialog::collectData()
{
    return (TransactItem::TransactItemBuilder()
            .setId(m_data.itemId)
            .setName(m_data.itemName)
            .setDate(ui->dateTransact->date())
            .setDiff(ui->spinDiff->value())
            .setNote(ui->editNote->text())
            .setStock(m_data.itemStockRef)
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
    if (ui->editProductName->text().isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите позицию на складе, для которой нужно создать новый приход/расход.");
        return;
    }
    if (ui->comboProject->currentData(Constants::RoleNodeId).toInt() == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите тему, на которую следует записать текущий приход/расход.");
        return;
    }
//    if (ui->editNote->text().isEmpty()) {
//        QMessageBox::warning(this,
//                             "Ошибка!",
//                             "Введите комментарий, поясняющий информацию о текущем приходе/расходе.");
//        return;
//    }
    if (ui->spinDiff->value() == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Приход/расход не может быть равен нулю.");
        return;
    }

    m_data = collectData();
    accept();
}

void TransactDataDialog::on_editSearch_textChanged(const QString &arg1)
{
    m_searchFilterModel->setFilterWildcard(arg1);
    if (arg1.size() > 2) {
        ui->treeStock->expandAll();
    } else {
//        ui->treeStock->clearSelection();
        ui->treeStock->collapseAll();
    }
    m_searchFilterModel->invalidate();
}

void TransactDataDialog::on_treeStock_clicked(const QModelIndex &index)
{
//    qDebug() << "tree click";
//    on_treeStock_doubleClicked(index);
    if (index.data(Constants::RoleNodeType) == Constants::ItemItem) {
        QModelIndex sourceIndex = m_searchFilterModel->mapToSource(index);

        m_data = TransactItem::TransactItemBuilder().fromStockItem(m_stockModel->getStockItemByIndex(sourceIndex));
//        qDebug() << m_data;

        updateWidgets();
    }
}

void TransactDataDialog::on_treeStock_doubleClicked(const QModelIndex &index)
{
//    qDebug() << "tree doubleclick";
}

void TransactDataDialog::treeStockSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
//    qDebug() << "tree selection changed";
//    if (ui->treeStock->selectionModel()->hasSelection()) {
//        on_treeStock_doubleClicked(current);
//    }
//    else {
//        resetWidgets();
//    }
}
