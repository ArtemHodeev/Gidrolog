#include "confirmsolver.h"
#include "ui_confirmsolver.h"
#include <names.h>

ConfirmSolver::ConfirmSolver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmSolver)
{
    ui->setupUi(this);
    ui->radioButton_selected->setChecked(true);
    ui->listWidget->setEnabled(true);
    ui->spinBox->setMaximum(Names::params->size());
    QHash<QString, unsigned int>::iterator iterator;

    for (iterator = Names::params->begin(); iterator != Names::params->end(); iterator ++)
    {
        ui->listWidget->addItem(iterator.key());
    }
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
}

ConfirmSolver::~ConfirmSolver()
{
    delete ui;
}

void ConfirmSolver::on_pushButton_ok_pressed()
{
    count = ui->spinBox->text().toInt();
}

int ConfirmSolver::getCount() const
{
    return count;
}

void ConfirmSolver::on_radioButton_selected_clicked()
{
    ui->listWidget->setEnabled(true);
}

void ConfirmSolver::on_radioButton_full_clicked()
{

    ui->listWidget->setEnabled(false);
    selected_items.clear();
    ui->listWidget->selectedItems().clear();
    ui->listWidget->itemSelectionChanged();

}

void ConfirmSolver::on_listWidget_clicked(const QModelIndex &index)
{
    unsigned int item_id = Names::params->value(index.data().toString());
    int pos = selected_items.indexOf(item_id);

    if (pos == -1)
    {
        selected_items.append(item_id);
    }
    else
    {
        selected_items.removeAt(pos);
    }
    if (selected_items.size() > ui->spinBox->text().toInt())
    {
        ui->pushButton_ok->setEnabled(false);
    }
    else
    {
        ui->pushButton_ok->setEnabled(true);
    }
}

void ConfirmSolver::on_spinBox_valueChanged(int arg1)
{
    if (selected_items.isEmpty() == false && arg1 > selected_items.size())
    {
        ui->pushButton_ok->setEnabled(false);
    }
    else
    {
        ui->pushButton_ok->setEnabled(true);
    }
}
QVector<unsigned int> ConfirmSolver::getSelectedItems()
{
    return selected_items;
}
