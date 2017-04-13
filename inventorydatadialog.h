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

namespace Ui {
class InventoryDataDialog;
}

class InventoryDataDialog : public QDialog
{
    Q_OBJECT

public:

    ProductItem m_data;
    ProductItem m_oldData;
    HashDict m_catMap;
    HashDict m_grpMap;
    IdMap m_comboMap;

    MapModel *m_categoryListModel;
    MapModel *m_groupListModel;

    bool m_dataModified = false;
    bool m_newRecord = false;

    explicit InventoryDataDialog(QWidget *parent = nullptr);
    ~InventoryDataDialog();

    InventoryDataDialog &setData       (const ProductItem &data)   {m_data     = data;   return *this;}
    InventoryDataDialog &setCategoryMap(const HashDict    &catmap) {m_catMap   = catmap; return *this;}
    InventoryDataDialog &setGropMap    (const HashDict    &grpmap) {m_grpMap   = grpmap; return *this;}
    InventoryDataDialog &setComboLink  (const IdMap       &link)   {m_comboMap = link;   return *this;}

    void filterGroupCombo(const qint32 catId);

    // init widgets with data
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
