#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QDialog>
#include <QDebug>
#include <QDate>

#include <databasemanager.h>
#include <dictmodel.h>
#include <mapmodel.h>

namespace Ui {
class ReportManager;
}

class ReportManager : public QDialog
{
    Q_OBJECT

public:

    DataBaseManager *m_dbman;

    DictModel *m_dictModel;

    MapModel *m_filteredGroupModel_1;
    MapModel *m_filteredGroupModel_2;

    explicit ReportManager(QWidget *parent = 0);
    ~ReportManager();

    ReportManager &setDbManager(DataBaseManager *dbman) {m_dbman     = dbman; return *this;}
    ReportManager &setDictModel(DictModel       *dict)  {m_dictModel = dict;  return *this;}

    void filterGroupCombo_1(const qint32 catId);
    void filterGroupCombo_2(const qint32 catId);

    void initDialog();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_comboCategory_currentIndexChanged(int index);

    void on_comboCategory_2_currentIndexChanged(int index);

    void on_comboGroup_currentTextChanged(const QString &arg1);

    void on_comboGroup_2_currentTextChanged(const QString &arg1);

    void on_btnShow_clicked();

private:
    Ui::ReportManager *ui;
};

#endif // REPORTMANAGER_H
