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

void StockDataDialog::initDialog()
{
    if (m_data.itemId == 0) {
        setWindowTitle("Добавить позицию хранения:");
    } else {
        setWindowTitle("Изменить позицию хранения:");
    }

    m_productListModel = new IdStringModel(this);
    m_searchFilterModel = new QSortFilterProxyModel(this);

    m_productListModel->setData(m_dbman->getIdProductList100());
    m_searchFilterModel->setSourceModel(m_productListModel);
    m_searchFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_searchFilterModel->setFilterWildcard(m_data.itemName);

    ui->listProduct->setModel(m_searchFilterModel);
    ui->comboLocation->setModel(m_dictModel->m_locationListModel);
    ui->comboProject->setModel(m_dictModel->m_projectListModel);

    ui->editSearchProduct->setText(m_data.itemName);
    ui->editProductName->setText(m_data.itemName);
    ui->editProductId->setText(QString::number(m_data.itemId));
    ui->comboLocation->setCurrentText(m_dictModel->m_locationListModel->getData(m_data.itemLocation));
    ui->comboProject->setCurrentText(m_dictModel->m_projectListModel->getData(m_data.itemProject));

    m_oldData = m_data;
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

void StockDataDialog::on_editSearchProduct_textChanged(const QString &arg1)
{
    m_searchFilterModel->setFilterWildcard(arg1);
}


void StockDataDialog::on_listProduct_doubleClicked(const QModelIndex &index)
{
    ui->editSearchProduct->setText(index.data(Qt::DisplayRole).toString());
    ui->editProductName->setText(index.data(Qt::DisplayRole).toString());
    ui->editProductId->setText(index.data(Constants::RoleNodeId).toString());
}
