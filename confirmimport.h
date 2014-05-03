#ifndef CONFIRMIMPORT_H
#define CONFIRMIMPORT_H

#include <QDialog>
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
    bool cancel; // флаг: была ли надата кнопка отмены
    bool param_sign; // флаг: существует ли модель компонентов
    bool water_sign; // флаг: существует ли модель типов водных масс
    bool location_sign; // флаг: существует ли модель мест взятия
    ConfirmItemModel *param_model; // Модель компонентов
    ConfirmWaterTypeModel *water_model; // модель типов водных масс
    ConfirmLocationModel *location_model; // модель мест взятия
public slots:
    void on_itemChanged_emited();
};

#endif // CONFIRMIMPORT_H
