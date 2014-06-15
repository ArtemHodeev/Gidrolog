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
#include <watertypecombobox.h>
#include <locationcombobox.h>
#include <names.h>
#include <calculator.h>
#include <confirmcalculatormodel.h>
#include <confirmcalculator.h>
#include <confirmsolver.h>
#include <solverpca.h>
#include <connectdb.h>
#include "dialogtriangles.h"
#include <testsolve.h>
#include <spinboxdelegate.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //создание модели
    model = new SampleModel();

    // Создание обработчика действий по нажатию на кнопку "Сохранить"
    QObject::connect(model,SIGNAL(actionSave()), this,SLOT(on_actionSave_triggered()));

    // наполнение модели данными
    model->setItems();
    model->setHeaders();
//    model->setWaterTypes();
//    model->setLocation();
//    model->setParams();
    //отображение модели
    ui->tableView->setModel(model);

    //Создание делегатов combobox для ввода типов водных масс и мест взятия
    ComboboxDelegate *water_delegeate = new WaterTypeCombobox();
    ComboboxDelegate *location_delegate = new LocationCombobox();

    ui->tableView->setItemDelegateForColumn(1,location_delegate);
    ui->tableView->setItemDelegateForColumn(3,water_delegeate);

    //Установление модели для выделения элементов
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
    model->resetModel();
}

/*
 * F:
 *  void MainWindow::on_actionSave_triggered()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  сохранение изменений модели в БД
 */
void MainWindow::on_actionSave_triggered()
{
    model->saveItems();
    model->updateItems();
    model->removeItems();
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
        QModelIndexList list; // список выделенных строк
        QModelIndexList::iterator i; // итератор по списку выделенных строк
        int ind = 0;
        int *sample_num; // массив, элементы которого номера выделенных строк

        list = sel_model->selectedRows();
        sample_num = new int[sel_model->selectedRows().size() + 1];

        //Определение номеров выделенных строк
        for (i = list.begin(); i < list.end(); i ++)
        {
            if (i->row() < model->rowCount(QModelIndex()) - 1)
            {
                sample_num[ind] = i->row();
                ind ++;
            }
        }
        sample_num[ind] = -1;

        model->setItemsToDelete(sample_num);
        delete sample_num;
    }
}

/*
 * F:
 *  void MainWindow::on_action_importExcel_triggered()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция вызывает проводник для выбора файла, из которго предполагается импорт проб. Если файл был выбран,
 *  то функция запускает создает объект типа Importer, который извлекает пробы из файла. После импорта проб
 *  модель устанавливается заново с новыми данными.
 */
void MainWindow::on_action_importExcel_triggered()
{
    QString url = QFileDialog::getOpenFileName();
    if (url == "")
    {
        return;
    }

    // Подготовка для импорта проб
    Importer doc(url);

    // Импорт проб
    QVector<Sample*> sam = doc.import();

    //Если есть импортированные пробы, то переустановить модель
    if (sam.size() != 0)
    {
        model->resetModel(sam);
    }
}

void MainWindow::on_action_prepare_triggered()
{
    Calculator calc;
    ConfirmCalculatorModel *calc_model = new ConfirmCalculatorModel();
    ConfirmCalculator *calc_dlg = new ConfirmCalculator();
    QDialog *dlg ;
    QVector<ItemInfo* >list;

    calc.setItems(model->getSample());
    list = calc.getInfo();

    calc_model->setItems(list);
    calc_dlg->setModel(calc_model);
    dlg = calc_dlg;

    if (dlg->exec() == QDialog::Accepted)
    {
        model->resetModel(calc.getItems());
    }

    delete dlg;
    delete calc_model;
}

void MainWindow::on_action_3_triggered()
{
    QDialog *dlg = new ConnectDB();
    dlg->exec();
    delete dlg;
}

void MainWindow::on_action_calcilate_triggered()
{
    ConfirmSolver *confirm = new ConfirmSolver();
    QDialog *dlg = confirm;

    if (dlg->exec() == QDialog::Accepted)
    {
//        SolverPCA solver;
        QHash<QString, unsigned int>::iterator iter;
        QVector<unsigned int> mass;
        QVector<Sample*> samples;

        samples = model->getSample();

        for (iter = Names::params->begin(); iter != Names::params->end(); iter ++)
        {
            mass.append(iter.value());
        }

        TestSolve *solve = new TestSolve();
        solve->setSamples(samples);
        solve->setParam(confirm->getCount(),confirm->getSelectedItems());
        solve->setPlurList();
        solve->exec();
//        solver.setItems(samples);
//        solver.standart();
//        solver.makePlurals(confirm->getCount(),confirm->getSelectedItems(), mass);
//        solver.setAnaliticSamples(analitic_samples);

    }
    delete dlg;
}

void MainWindow::on_action_plot_triggered()
{
    DialogTriangles *dlg = new DialogTriangles();
    dlg->setFixedSize(800, 559);
    dlg->show();
}
