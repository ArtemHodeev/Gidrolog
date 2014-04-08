#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <editor.h>
#include <QWidget>
#include <tablemodel.h>
#include <itemtablemodel.h>
#include <samplemodel.h>
#include <QDebug>
#include <QItemSelectionModel>
#include <QItemSelection>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new SampleModel();
    QObject::connect(this,SIGNAL(actionSave()), model,SLOT(on_actionSave_triggered()));

    model->setItems();
    model->setHeaders();

    ui->tableView->setModel(model);

    sel_model = new QItemSelectionModel(model);
//    model-

    ui->tableView->setSelectionModel(sel_model);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
//    QModelIndex top_left;
//    QModelIndex bottom_right;
//    QItemSelection row_selection;

//    top_left = model->index(0,0);
//    bottom_right = model->index(4,0);

//    row_selection.select(top_left,bottom_right);

//    sel_model->select(row_selection,QItemSelectionModel::Select | QItemSelectionModel::Rows);
//    connect(sel_model,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(changeSelection(QModelIndex,QModelIndex)));
//    ui->tableView->;
//    qDebug()<<"Is Selected: "<<sel_model->isRowSelected(2,QModelIndex());

}

MainWindow::~MainWindow()
{
    delete sel_model;
    delete model;
    delete ui;
}
/*
 * I: --
 * O: --
 * D: Вызывает открытие окна редактора знаний
 */
void MainWindow::on_action_editorTool_triggered()
{
    Editor *dlg = new Editor();
    dlg->exec();
    delete dlg;
}
void MainWindow::on_actionSave_triggered()
{
    qDebug() << "Slot MainWindow::on_actionSave_clicked()";
    emit(actionSave());
}

void MainWindow::keyPressEvent(QKeyEvent *key_event)
{
    if (key_event->matches(QKeySequence::Delete) && sel_model->selectedRows().size() > 0)
    {
        QModelIndexList list = sel_model->selectedRows();
        QModelIndexList::iterator i;

        int *sample_num = new int[sel_model->selectedRows().size() + 1];

        int ind = 0;
        for (i = list.begin(); i < list.end(); i ++)
        {
             sample_num[ind] = i->row();
             ind ++;
        }
        sample_num[ind] = -1;

        model->setItemsToDelete(sample_num);
    }
}
