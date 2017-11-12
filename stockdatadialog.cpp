#include "stockdatadialog.h"
#include "ui_stockdatadialog.h"

StockDataDialog::StockDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StockDataDialog)
{
    ui->setupUi(this);
}

StockDataDialog::~StockDataDialog()
{
    delete ui;
}

void StockDataDialog::updateWidgetsWithStock(const StockItem &tmpstock)
{
    ui->editSearchProduct->setText(tmpstock.itemName);
    ui->editProductName->setText(tmpstock.itemName);
    ui->editProductId->setText(QString::number(tmpstock.itemProduct));
    ui->comboLocation->setCurrentText(m_dictModel->m_locationListModel->getData(tmpstock.itemLocation));
    ui->comboProject->setCurrentText(m_dictModel->m_projectListModel->getData(tmpstock.itemProject));
    ui->spinAmount->setValue(tmpstock.itemAmount.getAsDouble());
}

void StockDataDialog::updateWidgetsWithProduct(const qint32 prodId)
{
    QString prodname = m_dictModel->m_productListModel->getName(prodId);

    ui->editSearchProduct->setText(prodname);
    ui->editProductName->setText(prodname);
    ui->editProductId->setText(QString::number(prodId));
    ui->comboLocation->setCurrentIndex(0);
    ui->comboProject->setCurrentIndex(0);
    ui->spinAmount->setValue(0);
}

void StockDataDialog::initDialog()
{
    // TODO: m_data and m_oldData init logic fault?
    if (m_data.itemId == 0) {
        setWindowTitle("Добавить позицию хранения:");
    } else {
        setWindowTitle("Изменить позицию хранения:");
    }

    m_searchFilterModel = new QSortFilterProxyModel(this);
    m_searchFilterModel->setSourceModel(m_dictModel->m_productListModel);
    m_searchFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_searchFilterModel->setFilterWildcard(m_oldData.itemName);

    ui->listProduct->setModel(m_searchFilterModel);
    ui->comboLocation->setModel(m_dictModel->m_locationListModel);
    ui->comboProject->setModel(m_dictModel->m_projectListModel);

    m_oldData = m_dbman->getStockItemByProductId(m_data.itemProduct);

//    qDebug() << "dlg old data:" << m_oldData;

    if (m_data.itemId == 0) {
        updateWidgetsWithProduct(m_data.itemProduct);
    } else {
        updateWidgetsWithStock(m_data);
    }
}

StockItem StockDataDialog::getData()
{
    return m_data;
}

void StockDataDialog::changeEvent(QEvent *e)
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

StockItem StockDataDialog::collectData()
{
    return (StockItem::StockItemBuilder()
            .setId         (m_data.itemId)
            .setName       (ui->editProductName->text())
            .setType       (Constants::ItemItem)
            .setLevel      (Constants::Level_2)
            .setAmount     (ui->spinAmount->value())
            .setSerialn    (m_data.itemSerialn)
            .setProject    (ui->comboProject->currentData(Constants::RoleNodeId).toInt())
            .setLocation   (ui->comboLocation->currentData(Constants::RoleNodeId).toInt())
            .setProduct    (ui->editProductId->text().toInt())
            .setUnit       (m_oldData.itemUnit)
            .build());
}

void StockDataDialog::on_editSearchProduct_textChanged(const QString &arg1)
{
    m_searchFilterModel->setFilterWildcard(arg1);
}

void StockDataDialog::on_listProduct_doubleClicked(const QModelIndex &index)
{
    m_data = m_dbman->getStockItemByProductId(index.data(Constants::RoleNodeId).toInt());

    if (m_data.itemId == 0) {
        updateWidgetsWithProduct(index.data(Constants::RoleNodeId).toInt());
    } else {
        updateWidgetsWithStock(m_data);
    }

    m_searchFilterModel->setFilterWildcard(QString(""));
    ui->listProduct->scrollTo(index);
}

void StockDataDialog::on_listProduct_clicked(const QModelIndex &index)
{
    on_listProduct_doubleClicked(index);
}

void StockDataDialog::on_btnOk_clicked()
{
    if (ui->editProductName->text().isEmpty() ||
        ui->editProductId->text() == QString("0")) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите продукт для регистрации на складе.");
        return;
    }
    if (ui->comboProject->currentData(Constants::RoleNodeId).toInt() == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите тему, на которую будет зарегистрирована данная позиция.");
        return;
    }
    if (ui->comboLocation->currentData(Constants::RoleNodeId).toInt() == 0) {
        QMessageBox::warning(this,
                             "Ошибка!",
                             "Выберите место хранения, на котором будет зарегистрирована данная позиция.");
        return;
    }

    m_data = collectData();
    accept();
}

