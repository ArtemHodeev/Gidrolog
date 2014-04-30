#include "confirmimport.h"
#include "ui_confirmimport.h"
#include <itemmodel.h>
#include <QTableView>

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
    ui->pushButton_save->setEnabled(false);
    param_sign = false;
    water_sign = false;
    location_sign = false;
}
void ConfirmImport::setParamModel(ConfirmItemModel *model)
{
//    param_model = new ConfirmItem();
    param_model = new ConfirmItemModel();
    param_model = model;
    connect(param_model,SIGNAL(on_itemChanged()),this,SLOT(on_itemChanged_emited()));

    ui->params_widget->setHidden(false);
    ui->params_tableView->setModel(model);
//    if (location_model->hasEmptyType());
    param_sign = true;
}
void ConfirmImport::setWaterModel(ConfirmWaterTypeModel *model)
{
    water_model = new ConfirmWaterTypeModel();
    water_model = model;
    ui->water_widget->setHidden(false);
    ui->water_tableView->setModel(water_model);
    water_sign = true;
//    ui->params_tableView->setModel();

}
void ConfirmImport::setLocationModel(ConfirmLocationModel *model)
{
    location_model = new ConfirmLocationModel();
    location_model = model;
    ui->location_widget->setHidden(false);
    ui->location_tableView->setModel(location_model);

    location_sign = true;
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
    if(location_sign == true)
    {
        delete location_model;
    }

    delete ui;
}

void ConfirmImport::on_pushButton_save_clicked()
{
    if (param_sign ==true)
    {
        param_model->saveItems();
    }
    if (water_sign == true)
    {
        water_model->saveItems();
    }
    if (location_sign == true)
    {
        location_model->saveItems();
    }

}

void ConfirmImport::on_itemChanged_emited()
{
//    if ( == true)
//    {
    bool enable = !param_model->hasEmptyType();
        ui->pushButton_save->setEnabled(enable);
//    }
//    else
}
