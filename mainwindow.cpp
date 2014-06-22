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
#include <QCloseEvent>
#include <time.h>

#include <QMainWindow>
#include <QTabWidget>
#include "editornew.h"
#include <QMessageBox>

#include <starter.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//     QListWidgetItem *create_item = new QListWidgetItem()
    QListWidgetItem *create_item = new QListWidgetItem("<Введите название>");//,ui->create_set);
    QListWidgetItem *available_item = 0;
//    QListWidgetItem *create_item
//    QListWidget *lwg = new QListWidget();
//    lwg->setIconSize(QSize(48,48));
//    create_item->setText("<Введите название>");



    create_item->setIcon(QPixmap(":/resources/new.png"));
    create_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                                Qt::ItemIsEditable);
//    lwg->insertItem(0,lwg);
    ui->create_set->addItem(create_item);

    ui->stackedWidget->setCurrentIndex(1);
    QMap<QString, unsigned int> *local_map;
    map = new QMap<QString, unsigned int>();
    QMap<QString, unsigned int>::iterator iter;
    Starter sample_sets;
    local_map = sample_sets.getSampleSets();

    for (iter = local_map->begin(); iter != local_map->end(); iter ++)
    {
        map->insert(iter.key(),iter.value());
        available_item =  new QListWidgetItem(iter.key(),ui->all_sets);
        available_item->setIcon(QPixmap(":/resources/available.png"));

//        ui->all_sets->addItem(iter.key());
    }

    del = false;
////    for ()

}

MainWindow::~MainWindow()
{
    if(del == true)
    {
        delete sel_model;
        delete model;
        delete ui;
    }

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

//    Editor *dlg = new Editor();
//    dlg->exec();
//    delete dlg;
//    model->resetModel();
     EditorNew *window = new EditorNew();
     window->setWindowTitle(QString::fromUtf8("Редактор знаний"));
     window->resize(680, 559);
     window->show();

//    Editor *dlg = new Editor();
//    dlg->exec();
//    delete dlg;
    // если модель уже имеется(т.е. не на главной странице), то reset
    if (ui->stackedWidget->currentIndex() == 0)
    {
        model->resetModel();
    }
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
    clock_t start;
    clock_t end;
    start = clock();
    model->saveItems();
    end = clock() - start;
    qDebug()<< "saving: "<<end / CLOCKS_PER_SEC;
    model->updateItems();
    end = clock() - end;
    qDebug()<< "updating: "<<end / CLOCKS_PER_SEC;
    model->removeItems();
    end = clock() - end;
    qDebug()<< "deleting: "<<end / CLOCKS_PER_SEC;
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
        DialogTriangles *dlg = new DialogTriangles();
        dlg->setFixedSize(1350, 700);
        dlg->setSamples(samples);
        dlg->setParam(confirm->getCount(),confirm->getSelectedItems());
        dlg->setPlurList();
        if (dlg->list_size)
            dlg->show();
        else
            QMessageBox::about(
                      this, tr("Отсутствуют варианты"),
                      tr("<h2>Для данного числа компонентов</h2>"
                         "<p>не найдено ни одного набора, соответствующего условиям."));

//        TestSolve *solve = new TestSolve();
//        solve->setSamples(samples);
//        solve->setParam(confirm->getCount(),confirm->getSelectedItems());
//        solve->setPlurList();
//        solve->exec();

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



void MainWindow::on_all_sets_itemClicked(QListWidgetItem *item)
{
    del = true;
    ui->stackedWidget->setCurrentIndex(0);
    Names::sample_set_id = map->value(item->data(Qt::DisplayRole).toString());

    //создание модели
    model = new SampleModel();

    // Создание обработчика действий по нажатию на кнопку "Сохранить"
    QObject::connect(model,SIGNAL(actionSave()), this,SLOT(on_actionSave_triggered()));

    // наполнение модели данными
    model->setItems();
    model->setHeaders();

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


void MainWindow::on_create_set_itemChanged(QListWidgetItem *item)
{
    Starter starter;
    unsigned int set_id = starter.saveSampleSet(item->data(Qt::DisplayRole).toString());
    Names::sample_set_id = set_id;
    on_all_sets_itemClicked(item);
}
