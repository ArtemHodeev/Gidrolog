#ifndef CONFIRMIMPORT_H
#define CONFIRMIMPORT_H

#include <QDialog>
#include <itemmodel.h>
#include <tablemodel.h>
#include <confirmitemmodel.h>

namespace Ui {
class ConfirmImport;
}

class ConfirmImport : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmImport(QWidget *parent = 0);
    ~ConfirmImport();
    void setParamModel(TableModel *model);
    void setWaterModel(TableModel *model);
    void setModels();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ConfirmImport *ui;
    bool param_sign;
    bool water_sign;
    TableModel *param_model;
    TableModel *water_model;



};

#endif // CONFIRMIMPORT_H
