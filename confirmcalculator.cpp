#include "confirmcalculator.h"
#include "ui_confirmcalculator.h"
#include <itemtypecombobox.h>
ConfirmCalculator::ConfirmCalculator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmCalculator)
{
    ui->setupUi(this);
    ui->tableView->setWordWrap(true);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    cancel = false;
    model = new ConfirmCalculatorModel();

}

ConfirmCalculator::~ConfirmCalculator()
{
    delete ui;
}
void ConfirmCalculator::setModel(ConfirmCalculatorModel *model)
{
    this->model = model;
    ui->tableView->setModel(this->model);
    sel_model = new QItemSelectionModel(model);
    ui->tableView->setSelectionModel(sel_model);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}
void ConfirmCalculator::on_pushButton_cancel_clicked()
{
    cancel = true;
}
bool ConfirmCalculator::isCanceled()
{
    return cancel;
}

void ConfirmCalculator::on_pushButton_ok_pressed()
{
    QModelIndexList sel_columns = sel_model->selectedRows(0);

    if (sel_columns.isEmpty() != true)
    {
        int *rows = new int[sel_columns.size() + 1];
        int ind  = 0;
        QModelIndexList::iterator i;
        for (i = sel_columns.begin(); i != sel_columns.end(); i ++)
        {
            rows[ind] = i->row();
            ind ++;
        }
        rows[ind] = -1;
        model->setItemsToDelete(rows);
        delete rows;
//        model->backup();
        model->removeItems();
    }

}
