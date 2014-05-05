#include "confirmcalculator.h"
#include "ui_confirmcalculator.h"

ConfirmCalculator::ConfirmCalculator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmCalculator)
{
    ui->setupUi(this);

    cancel = false;
    model = new ConfirmCalculatorModel();
//    model->setItems();
}

ConfirmCalculator::~ConfirmCalculator()
{
    delete ui;
}
void ConfirmCalculator::setModel(ConfirmCalculatorModel *model)
{
    this->model = model;
    ui->tableView->setModel(this->model);
}
void ConfirmCalculator::on_pushButton_cancel_clicked()
{
    cancel = true;
}
bool ConfirmCalculator::isCanceled()
{
    return cancel;
}
