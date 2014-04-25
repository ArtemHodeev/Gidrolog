#include "confirmimport.h"
#include "ui_confirmimport.h"
#include <itemmodel.h>
#include <QTableView>

ConfirmImport::ConfirmImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmImport)
{
    ui->setupUi(this);
//    QTableView view;// = new QTableView(ui->toolBox);
//    view
//    QWidget *w = new QTableView();
//    ItemModel *model = new ItemModel();
//    model->setItems();
////    ui->toolBox->
//    w
////    view.setModel(model);
//    ui->toolBox->addItem(&view,QString("New tab"));
//    ui->tableView->setModel(model);

}

ConfirmImport::~ConfirmImport()
{
    delete ui;
}
