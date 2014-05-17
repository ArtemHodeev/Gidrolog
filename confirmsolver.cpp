#include "confirmsolver.h"
#include "ui_confirmsolver.h"
#include <names.h>

ConfirmSolver::ConfirmSolver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmSolver)
{
    ui->setupUi(this);
    ui->radioButton_full->setChecked(true);
    ui->listWidget->setEnabled(false);
    ui->spinBox->setMaximum(Names::params->size());
    QHash<QString, unsigned int>::iterator iterator;

    for (iterator = Names::params->begin(); iterator != Names::params->end(); iterator ++)
    {
        ui->listWidget->addItem(iterator.key());
    }
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
