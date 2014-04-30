#include "confirmimport.h"
#include "ui_confirmimport.h"
#include <itemmodel.h>
#include <QTableView>
#include <confirmitemmodel.h>
#include <confirmwatertypemodel.h>
//#include <watertypemodel.h>
#include <QDebug>

ConfirmImport::ConfirmImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmImport)
{
    ui->setupUi(this);
    ui->params_widget->setHidden(true);
    ui->location_widget->setHidden(true);
    ui->water_widget->setHidden(true);
    param_sign = false;
    water_sign = false;
}
void ConfirmImport::setParamModel(TableModel *model)
{
//    param_model = new ConfirmItem();
    param_model = model;
    ui->params_widget->setHidden(false);
    ui->params_tableView->setModel(model);
    param_sign = true;
}
void ConfirmImport::setWaterModel(TableModel *model)
{
    water_model = new ConfirmWaterTypeModel();
    water_model = model;
    ui->water_widget->setHidden(false);
    ui->water_tableView->setModel(water_model);
    water_sign = true;
//    ui->params_tableView->setModel();

}
void ConfirmImport::setModels()
{
//    if (param_model)
//    {
//        ui->params_widget->setHidden(false);
//        ui->params_tableView->setModel(param_model);
//    }
//    if (water_model)
//    {
//        ui->water_widget->setHidden(false);
//        ui->water_tableView->setModel(water_model);
//    }
//    ui->params_widget->sh
}
ConfirmImport::~ConfirmImport()
{
    if (param_sign ==true)
    {
        delete param_model;
    }
    if (water_sign == true)
    {
        delete water_model;
    }

    delete ui;
}

void ConfirmImport::on_pushButton_clicked()
{
    if (param_sign ==true)
    {
        param_model->saveItems();
    }
    if (water_sign == true)
    {
        water_model->saveItems();
    }
}
