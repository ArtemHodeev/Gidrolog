#include "editor.h"
#include "ui_editor.h"
#include <QDebug>
#include <databaseaccessor.h>
#include <tablemodel.h>
#include <QSqlError>
#include <itemmodel.h>
#include <itemtypemodel.h>
#include <watertypemodel.h>

Editor::Editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    model = new ItemModel();
//    ui->pushButton_itemAdd->si
    model->setItems();
//    connect(this,SIGNAL(save_clicked()),model,SLOT(on_saveButton_clicked()));
//    connect(this,SIGNAL(remove_clicked()),model,SLOT(on_removeButton_clicked()));
//    connect(this,SIGNAL(exit_action()),model,SLOT(on_pushButton_exit_clicked()));

    ui->tableView_itemInSystem->setModel(model);
    sel_model = new QItemSelectionModel(model);

    ui->tableView_itemInSystem->setSelectionModel(sel_model);
    ui->tableView_itemInSystem->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void Editor::on_listWidget_editorMenu_clicked()
{
    int cur = ui->listWidget_editorMenu->currentIndex().row();

    switch(cur)
    {
    case 0:
        model = new ItemModel();
        break;
    case 1:
//        Критерии
    case 2:
//        Объекты (Места взятия)
    case 3:
        model = new WaterTypeModel();
        break;
    case 4:
        model = new ItemTypeModel();
        break;
    };// switch(cur)

    model->setItems();
    ui->tableView_itemInSystem->setModel(model);
    sel_model = new QItemSelectionModel(model);

    ui->tableView_itemInSystem->setSelectionModel(sel_model);
    ui->tableView_itemInSystem->setSelectionMode(QAbstractItemView::ExtendedSelection);

}
Editor::~Editor()
{
    delete ui;
    delete sel_model;
    delete model;
}

//void Editor::on_pushButton_itemAdd_clicked()
//{
//    emit(save_clicked());
//}

//void Editor::on_pushButton_exit_clicked()
//{
////    emit(exit_action());
//}

//void Editor::on_pushButton_itemRemove_clicked()
//{
//    emit(remove_clicked());
//}
void Editor::keyPressEvent(QKeyEvent *key_event)
{
    qDebug()<<"Event";
    qDebug()<<"Sel_rows count: "<<sel_model->selectedRows().size();
    if (key_event->matches(QKeySequence::Delete) && sel_model->selectedRows().size() > 0)
    {
        qDebug()<<"Event: delete";
        QModelIndexList list = sel_model->selectedRows();
        QModelIndexList::iterator i;

        int *sel_rows = new int[sel_model->selectedRows().size() + 1];
        int ind = 0;
        for (i = list.begin(); i < list.end(); i ++)
        {

             sel_rows[ind] = i->row();
             qDebug()<<"row num: "<<sel_rows[ind];
             ind ++;
        }
        sel_rows[ind] = -1;
        qDebug()<<"In mainwindow: "<<sel_rows;

        model->setItemsToDelete(sel_rows);
//        delete sel_rows;
    }
}
void Editor::on_pushButton_exit_pressed()
{
    model->saveItems();
    model->updateItems();
    model->removeItems();
}
