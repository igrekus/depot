#ifndef INVENTORYDATADIALOG_H
#define INVENTORYDATADIALOG_H

#include <QDialog>
#include <QDebug>

#include <inventorydatadialog.h>
#include <productitem.h>
#include <linkeddict.h>

namespace Ui {
class InventoryDataDialog;
}

class InventoryDataDialog : public QDialog
{
    Q_OBJECT

public:

    ProductItem m_data;
//    ProductItem m_backupData;

    LinkedDict m_comboLink;

    bool m_dataChanged = false;

    // TODO: вместо конструктора - сцпленные методы для получения:
    // hashdict для комбо категорий
    // hashdict для комбо групп
    // hashdict для связки между комбо категорий и групп

    explicit InventoryDataDialog(const ProductItem &item,
                                 const LinkedDict  &link,
                                 QWidget *parent = nullptr);
    ~InventoryDataDialog();

    // init widgets with data
    void initDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InventoryDataDialog *ui;
};

#endif // INVENTORYDATADIALOG_H
