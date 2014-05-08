#ifndef CONFIRMCALCULATOR_H
#define CONFIRMCALCULATOR_H

#include <QDialog>
#include <confirmcalculatormodel.h>
#include <QItemSelectionModel>

namespace Ui {
class ConfirmCalculator;
}

class ConfirmCalculator : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmCalculator(QWidget *parent = 0);
    ~ConfirmCalculator();
    bool isCanceled();
    void setModel(ConfirmCalculatorModel *model);
private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_pressed();

private:
    bool cancel;
    ConfirmCalculatorModel *model;
    QItemSelectionModel *sel_model;
//    QVector<ItemInfo>
    Ui::ConfirmCalculator *ui;
};

#endif // CONFIRMCALCULATOR_H
