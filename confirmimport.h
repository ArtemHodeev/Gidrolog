#ifndef CONFIRMIMPORT_H
#define CONFIRMIMPORT_H

#include <QDialog>
#include <itemmodel.h>
#include <tablemodel.h>
#include <confirmitemmodel.h>
#include <confirmwatertypemodel.h>
#include <confirmlocationmodel.h>
namespace Ui {
class ConfirmImport;
}

class ConfirmImport : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmImport(QWidget *parent = 0);
    ~ConfirmImport();
    void setParamModel(ConfirmItemModel *model);
    void setWaterModel(ConfirmWaterTypeModel *model);
    void setLocationModel(ConfirmLocationModel *model);
    bool isCanceled();

private slots:
    void on_pushButton_save_clicked();


    void on_pushButton_cancel_clicked();

private:
    Ui::ConfirmImport *ui;
    bool cancel;
    bool param_sign;
    bool water_sign;
    bool location_sign;
    ConfirmItemModel *param_model;
    ConfirmWaterTypeModel *water_model;
    ConfirmLocationModel *location_model;
public slots:
    void on_itemChanged_emited();
};

#endif // CONFIRMIMPORT_H
