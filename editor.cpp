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
    model->setItems();

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


void Editor::keyPressEvent(QKeyEvent *key_event)
{
    if (key_event->matches(QKeySequence::Delete) && sel_model->selectedRows().size() > 0)
    {
        QModelIndexList list = sel_model->selectedRows();
        QModelIndexList::iterator i;

        unsigned int *sel_rows = new unsigned int[sel_model->selectedRows().size() + 1];
        int ind = 0;
        for (i = list.begin(); i < list.end(); i ++)
        {

             sel_rows[ind] = i->row();
             ind ++;
        }
        sel_rows[ind] = -1;

        model->setItemsToDelete(sel_rows);
        delete sel_rows;
    }
}
void Editor::on_pushButton_exit_pressed()
{
    model->saveItems();
    model->updateItems();
    model->removeItems();
}
