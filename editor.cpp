#include "editor.h"
#include "ui_editor.h"
#include <QDebug>
#include <databaseaccessor.h>
#include <tablemodel.h>
#include <QSqlError>
#include <itemtablemodel.h>
#include <itemtypemodel.h>

Editor::Editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    model = new ItemTableModel();
//    ui->pushButton_itemAdd->si
    model->setItems();
    connect(this,SIGNAL(save_clicked()),model,SLOT(on_saveButton_clicked()));
    connect(this,SIGNAL(remove_clicked()),model,SLOT(on_removeButton_clicked()));
    connect(this,SIGNAL(exit_action()),model,SLOT(on_pushButton_exit_clicked()));
    ui->tableView_itemInSystem->setModel(model);
}

void Editor::on_listWidget_editorMenu_clicked()
{
    int cur = ui->listWidget_editorMenu->currentIndex().row();
    ui->stackedWidget->setCurrentIndex(cur);

    switch(cur)
    {
    case 0:
        model = new ItemTableModel();
        model->setItems();
        ui->tableView_itemInSystem->setModel(model);
        break;
//    case 1:
//        sql = "SELECT id, name, value FROM gydro.factor";

//        model->setQuery(sql,DatabaseAccessor::getDb());
//        ui->tableView_itemInSystem->setModel(model);
    case 2:
        model = new ItemTypeModel();
        model->setItems();

        ui->tableView_locationInSystem->setModel(model);

        break;
    case 3:

        break;
    case 4:

        break;
    };// switch(cur)
}
Editor::~Editor()
{
    delete ui;
    delete model;
}

void Editor::on_pushButton_itemAdd_clicked()
{
    emit(save_clicked());
}

void Editor::on_pushButton_exit_clicked()
{
    emit(exit_action());
}

void Editor::on_pushButton_itemRemove_clicked()
{
    emit(remove_clicked());
}
