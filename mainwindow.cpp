#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <editor.h>
#include <QWidget>
#include <tablemodel.h>
#include <itemmodel.h>
#include <samplemodel.h>
#include <QDebug>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QFileDialog>
#include <confirmimport.h>
#include <importer.h>
#include <comboboxdelegate.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new SampleModel();
    QObject::connect(this,SIGNAL(actionSave()), model,SLOT(on_actionSave_triggered()));

    model->setItems();
    model->setHeaders();
    model->setWaterTypes();
    model->setLocation();

    ui->tableView->setModel(model);

    ComboboxDelegate *water_delegeate = new ComboboxDelegate() ;
    ComboboxDelegate *location_delegate = new ComboboxDelegate();

    location_delegate->setItems(model->getLocations());
    water_delegeate->setItems(model->getWaterTypes());

    ui->tableView->setItemDelegateForColumn(1,location_delegate);
    ui->tableView->setItemDelegateForColumn(3,water_delegeate);
    sel_model = new QItemSelectionModel(model);

    ui->tableView->setSelectionModel(sel_model);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

MainWindow::~MainWindow()
{
    delete sel_model;
    delete model;
    delete ui;
}

/*
 * F:
 *  void MainWindow::on_action_editorTool_triggered()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Вызывает открытие окна редактора знаний
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
    model->saveItems();
    model->updateItems();
    model->removeItems();
//    emit(actionSave());
}

/*
 * F:
 *  void MainWindow::keyPressEvent(QKeyEvent *key_event)
 * I:
 *  *key_event - указатель на событие связанное с нажатием клавиши клавиатуры
 * O:
 *  --
 * D:
 *  Если была нажата клашиша delete и есть выделенные строки таблицы, то выделенные строки удаляются
 */
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
        qDebug()<<"In mainwindow: "<<sample_num;

        model->setItemsToDelete(sample_num);
        delete sample_num;
    }
}

void MainWindow::on_action_importExcel_triggered()
{
    QString url = QFileDialog::getOpenFileName();
    if (url == "")
    {
        return;
    }

    Importer doc(url);

    doc.setParams(model->getParams());
    doc.setWaterTypes(model->getWaterTypes());
    doc.setLocations(model->getLocations());

    QVector<Sample*> sam = doc.import();

    model->resetModel(sam);
//    model->setHeaders();
//    model->setSamples(sam);
//    emit(model->modelReset());

}

void MainWindow::on_actionOpen_triggered()
{
    QDialog *dlg = new ConfirmImport();
    dlg->exec();
    delete dlg;
}
