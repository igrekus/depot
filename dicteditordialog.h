#ifndef DICTEDITORDIALOG_H
#define DICTEDITORDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

#include <databasemanager.h>
#include <dictmodel.h>
#include <QStringListModel>
#include <mapmodel.h>

namespace Ui {
class DictEditorDialog;
}

class DictEditorDialog : public QDialog
{
    Q_OBJECT

public:
    QStringList dictList = {"Место хранения", "Сотрудники", "Тема"};
//    QStringList tableList = {"location", "staff", "project_tag"};
    qint32 m_dictTableId = 0;

    DataBaseManager *m_dbman;
    DictModel *m_dictModel;
    MapModel *m_currentListModel;

    QStringListModel *m_dictListModel;

    explicit DictEditorDialog(QWidget *parent = nullptr);
    ~DictEditorDialog();

    DictEditorDialog &setDbManager(DataBaseManager *dbman){m_dbman = dbman; return *this;}
    DictEditorDialog &setDictModel(DictModel *dm){m_dictModel = dm; return *this;}

    void initDialog();
    void loadDict(MapModel *dict);

    void addRecord(MapModel *dict, const qint32 tableId, const QString &newname);
    void editRecord(MapModel *dict, const qint32 tableId, const qint32 recId, const QString &newname);
    void deleteRecord(MapModel *dict, const qint32 tableId, const qint32 recId);

private slots:

    void on_comboDict_currentIndexChanged(int index);

    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();

    void on_listDict_doubleClicked(const QModelIndex &index);

private:
    Ui::DictEditorDialog *ui;
};

#endif // DICTEDITORDIALOG_H
