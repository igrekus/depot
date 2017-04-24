#ifndef INVENTORYDATADIALOG_H
#define INVENTORYDATADIALOG_H

#include <QDialog>
#include <QDebug>
#include <QDataWidgetMapper>
#include <QMessageBox>

#include <inventorydatadialog.h>
#include <productitem.h>
#include <linkeddict.h>
#include <hashdict.h>
#include <mapmodel.h>
#include <constants.h>
#include <dictmodel.h>

namespace Ui {
class InventoryDataDialog;
}

class InventoryDataDialog : public QDialog
{
    Q_OBJECT

public:

    ProductItem m_data;
    ProductItem m_oldData;

    DictModel *m_dictModel;

    MapModel *m_filteredGroupModel;

    bool m_dataModified = false;

    explicit InventoryDataDialog(QWidget *parent = nullptr);
    ~InventoryDataDialog();

    InventoryDataDialog &setData         (const ProductItem &data){m_data      = data; return *this;}
    InventoryDataDialog &setDictModel    (      DictModel   *dict){m_dictModel = dict; return *this;}

    void filterGroupCombo(const qint32 catId);

    // init widgets with data
    void initWidgetsWithData();
    void initDialog();

    ProductItem getData();

protected:
    void changeEvent(QEvent *e);

private slots:

    ProductItem collectData();

    void on_comboCategory_currentIndexChanged(int index);
    void on_comboGroup_currentTextChanged(const QString &arg1);

    void on_btnOk_clicked();

private:
    Ui::InventoryDataDialog *ui;
};

#endif // INVENTORYDATADIALOG_H
