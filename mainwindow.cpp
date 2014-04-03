#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <editor.h>
#include <QWidget>
#include <tablemodel.h>
#include <itemtablemodel.h>
#include <samplemodel.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SampleModel *model = new SampleModel();
//    QObject::connect(this,SIGNAL(actionSave()), model,SLOT(actionSave_clicked()));

    model->setItems();
    model->setHeaders();

    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
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
