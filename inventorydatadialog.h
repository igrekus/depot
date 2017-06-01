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
    struct DialogData {
        ProductItem item;
        ProductRelation relation;

        DialogData() = default;
        DialogData(const DialogData &copy) = default;

        DialogData(const ProductItem &item, const ProductRelation &rel):
            item(item),
            relation(rel)
        {}

        DialogData &operator=(const DialogData &right) {
            if (this != &right) {
                item     = right.item;
                relation = right.relation;
            }
            return *this;
        }

        bool operator==(const DialogData &right) const {
            return (item     == right.item &&
                    relation == right.relation);
        }

        friend QDebug operator<<(QDebug dbg, const DialogData &right) {
            dbg.nospace() << "InvDialogData("
                          << "product item:" << right.item
                          << " relation :"   << right.relation
                          << ")" ;
            return dbg.maybeSpace();
        }
    };

    DialogData m_data;
    DialogData m_oldData;

    DictModel *m_dictModel;

    MapModel *m_filteredGroupModel;

    bool m_dataModified = false;

    explicit InventoryDataDialog(QWidget *parent = nullptr);
    ~InventoryDataDialog();

    InventoryDataDialog &setDictModel(      DictModel  *dict) {m_dictModel = dict;  return *this;}
    InventoryDataDialog &setData     (const DialogData &data) {m_data      = data;  return *this;}

    void filterGroupCombo(const qint32 catId);

    // init widgets with data
    void updateWidgets();
    void initDialog();

    DialogData getData();

protected:
    void changeEvent(QEvent *e);

private slots:

    DialogData collectData();

    void on_comboCategory_currentIndexChanged(int index);
    void on_comboGroup_currentTextChanged(const QString &arg1);

    void on_btnOk_clicked();

private:
    Ui::InventoryDataDialog *ui;
};

#endif // INVENTORYDATADIALOG_H
