#include "editornew.h"
#include "ui_editornew.h"
#include <QTabWidget>
#include <QTableView>
#include <QDebug>
#include <databaseaccessor.h>
#include <tablemodel.h>
#include <QSqlError>
#include <itemmodel.h>
#include <itemtypemodel.h>
#include <watertypemodel.h>
#include <locationmodel.h>
#include <QVariant>
#include <comboboxdelegate.h>
#include <watertypecombobox.h>
#include <QMessageBox>
#include <QPushButton>
#include <itemtypecombobox.h>
#include <spinboxdelegate.h>
#include <doubleboxdelegate.h>
#include <QBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

EditorNew::EditorNew(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorNew)
{
    ui->setupUi(this);

    save_btn = new QPushButton("Сохранить");
    connect( save_btn, SIGNAL( clicked() ),  this, SLOT( on_save_button_pressed() ) );

    tabs = new QTabWidget(ui->centralwidget);
    connect( tabs, SIGNAL(tabBarClicked(int)),  this, SLOT( tabBarClickedSlot(int)) );

    item_table = new QTableView();
    item_type_table = new QTableView();
    water_type_table = new QTableView();
    location_table = new QTableView();
    criteries = new QWidget();

    analitic_type_label = new QLabel("Анализируемый тип водной массы");
    correl_label = new QLabel("Максимальный процент корреляции");
    max_error_count_label = new QLabel("Максимальный процент ошибки измерения");
    count_of_lost_label = new QLabel("Максимальный процент пропусков данных");

    analitic_type_t = new QComboBox();
    correl_t = new QLineEdit();
    max_error_t = new QLineEdit();
    count_of_lost_t = new QLineEdit();

    tabs->addTab(item_table,"Компоненты");
    tabs->addTab(item_type_table,"Типы компонентов");
    tabs->addTab(water_type_table, "Типы водных масс");
    tabs->addTab(location_table, "Объекты");
    tabs->addTab(criteries, "Критерии");

    QBoxLayout *layout_editor = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *layout_button = new QBoxLayout(QBoxLayout::RightToLeft);
    QBoxLayout *layout_criteries = new QBoxLayout(QBoxLayout::TopToBottom);

    //layout_button->addStretch(1);
    layout_button->addWidget(save_btn, 2);
    layout_button->addStretch(3);

    layout_editor->addWidget(tabs, 1);
    layout_editor->addLayout(layout_button);

    ui->centralwidget->setLayout(layout_editor);
    //setCentralWidget(layout_editor);

    layout_criteries->addWidget(analitic_type_label);
    layout_criteries->addWidget(analitic_type_t);
    layout_criteries->addWidget(correl_label);
    layout_criteries->addWidget(correl_t);
    layout_criteries->addWidget(max_error_count_label);
    layout_criteries->addWidget(max_error_t);
    layout_criteries->addWidget(count_of_lost_label);
    layout_criteries->addWidget(count_of_lost_t);
    criteries->setLayout(layout_criteries);

    item_model = new ItemModel();
    item_model->setItems();
    item_table->setModel(item_model);
    ComboboxDelegate *item_type_delegate = new Itemtypecombobox();
    item_table->setItemDelegateForColumn(2, item_type_delegate);
    CheckBoxDelegate *disp_delegate = new CheckBoxDelegate();
    item_table->setItemDelegateForColumn(5,disp_delegate);

    item_type_model = new ItemTypeModel();
    item_type_model->setItems();
    item_type_table->setModel(item_type_model);
    SpinBoxDelegate *type_delegate = new SpinBoxDelegate();
    item_type_table->setItemDelegateForColumn(2, type_delegate);

    water_model = new WaterTypeModel();
    water_model->setItems();
    water_type_table->setModel(water_model);

    location_model = new LocationModel();
    location_model->setItems();
    location_table->setModel(location_model);
    connect(location_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(on_location_changed()));

    factor_model = new FactorModel();
    factor_model->setItems();

    QString corell = QVariant(factor_model->getCorell()).toString();
    QString lost = QVariant(factor_model->getLostCount()).toString();
    QString error = QVariant(factor_model->getErrorCount()).toString();

    analitic_type_t->addItems(factor_model->getWaterTypes());
    analitic_type_t->setCurrentText(factor_model->getAnaliticName());
    analitic_type_t->setEditable(true);
    correl_t->setText(corell);
    count_of_lost_t->setText(lost);
    max_error_t->setText(error);

    water_sign = false;
    location_sign = false;
    factor_sign = false;
    item_type_sign = false;

    item_edited = false;
    water_edited = false;
    location_edited = false;
    item_type_edited = false;
    factor_edited = false;

    item_sign = true;
    current_model = item_model;
    sel_model = new QItemSelectionModel(item_model);
    item_table->setSelectionModel(sel_model);
    item_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(item_model,SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),this,SLOT(on_item_changed(const QModelIndex&, const QModelIndex&)));

}
void EditorNew::tabBarClickedSlot(int index)
{
    switch(index)
    {
    case 0:
    {
        //            Компоненты
       if (item_sign == false)
       {
           item_sign = true;    
       }
       connect(item_model,SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),this,SLOT(on_item_changed(const QModelIndex&, const QModelIndex&)));
       current_model = item_model;
       sel_model = new QItemSelectionModel(item_model);
       item_table->setSelectionModel(sel_model);
       item_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
       break;
    }
    case 1:
    {
//        Типы компонентов
        if (item_type_sign == false)
        {
            item_type_sign = true;
        }
        connect(item_type_model,SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),this,SLOT(on_item_type_changed(const QModelIndex&, const QModelIndex&)));
        current_model = item_type_model;
        sel_model = new QItemSelectionModel(item_type_model);
        item_type_table->setSelectionModel(sel_model);
        item_type_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
        break;
    }
    case 2:
    {
//        Типы водных масс
       if (water_sign == false)
       {
           water_sign = true;
       }
       connect(water_model,SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),this,SLOT(on_water_changed(const QModelIndex&, const QModelIndex&)));
       current_model = water_model;
       sel_model = new QItemSelectionModel(water_model);
       water_type_table->setSelectionModel(sel_model);
       water_type_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
       break;
    }
    case 3:
    {
//        Объекты (Места взятия)
         if (location_sign == false)
         {
             location_sign = true;
         }
         connect(location_model,SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),this,SLOT(on_location_changed(const QModelIndex&, const QModelIndex&)));
         current_model = location_model;
         sel_model = new QItemSelectionModel(location_model);
         location_table->setSelectionModel(sel_model);
         location_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
         break;
    }
    case 4:
    {
//        Критерии
       if (factor_sign == false)
       {
            factor_sign = true;
       }
       connect(analitic_type_t,SIGNAL(currentTextChanged(QString)), this,SLOT(on_comboBox_activated(const QString &)));
       connect(correl_t,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_maxCorrel_textEdited(const QString &)));
       connect(max_error_t,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_maxErrorCount_textEdited(const QString &)));
       connect(count_of_lost_t,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_countOfLost_textEdited(const QString &)));
       break;
    }
    };
}
EditorNew::~EditorNew()
{
    delete ui;
    delete sel_model;
    delete item_model;
    delete factor_model;
    delete location_model;
    delete water_model;
    delete item_type_model;
}
/*
 * F:
 *  void Editor::keyPressEvent(QKeyEvent *key_event)
 * I:
 *  QKeyEvent *key_event - событие - нажатая кнопка
 * O:
 *  --
 * D:
 *  Функция реагирует на нажатие клавиш клавиатуры. если нажата клавиша delete и есть выделенные строки, то выделенные строки удалются.
*/
void EditorNew::keyPressEvent(QKeyEvent *key_event)
{
    if (key_event->matches(QKeySequence::Delete) && sel_model->selectedRows().size() > 0)
    {
        QModelIndexList list; // список выделенных строк
        QModelIndexList::iterator i; // итератор по выделенным строкам
        int ind = 0;
        int *sel_rows; // массив, элементы которого номера выделенных строк

        sel_rows = new int[sel_model->selectedRows().size() + 1];
        list = sel_model->selectedRows();

        // Определяются номеров строк для удаления
        for (i = list.begin(); i < list.end(); i ++)
        {
            if (i->row() < current_model->rowCount(QModelIndex()) - 1)
            {
                sel_rows[ind] = i->row();
                ind ++;
            }
        }
        sel_rows[ind] = -1;

        // Удаление строк
        current_model->setItemsToDelete(sel_rows);
        switch(tabs->currentIndex())
        {
        case 0:
            item_edited = true;
            break;
        case 2:
            item_type_edited = true;
            break;
        case 3:
            water_edited = true;
            break;
        case 4:
            location_edited = true;
            break;
        default:
            break;
        }
        delete sel_rows;
    }
}
void EditorNew::on_save_button_pressed()
{
    save();
    item_sign = false;
    water_sign = false;
    location_sign = false;
    factor_sign = false;
    item_type_sign = false;

    item_edited = false;
    water_edited = false;
    location_edited = false;
    item_type_edited = false;
    factor_edited = false;

    int pos = tabs->currentIndex();
    tabs->setTabEnabled(pos, true);
}
void EditorNew::save()
{
    if (item_edited == true)
    {
        saveModel(item_model);
    }
    if (location_edited == true)
    {
        saveModel(location_model);
    }
    if (water_edited == true)
    {
        saveModel(water_model);
    }
    if (factor_edited == true)
    {
        factor_model->saveItems();
    }
    if (item_type_edited == true)
    {
        saveModel(item_type_model);
    }
}
void EditorNew::saveModel(TableModel *model)
{
    model->saveItems();
    model->updateItems();
    model->removeItems();
}
bool EditorNew::maybeSave()
{
    QMessageBox msg;
    QAbstractButton *ok = msg.addButton("Cохранить",QMessageBox::AcceptRole);
    QAbstractButton *cancel = msg.addButton("Отменить",QMessageBox::RejectRole);

    msg.setText("Данные были изменены. Сохранить изменения?");
    msg.setIcon(QMessageBox::Question);
    msg.exec();

    return (msg.clickedButton() == ok) ? true : false;
}
void EditorNew::closeEvent(QCloseEvent *event)
{
    if (item_edited == true || location_edited == true || water_edited == true || factor_edited == true || item_type_edited == true)
    {
        if (maybeSave() == true)
        {
            save();
        }
    }
}
void EditorNew::on_item_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    item_edited = true;
    qDebug() << "on_item_changed";
}
void EditorNew::on_water_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    water_edited = true;
    qDebug() << "on_water_changed";
}
void EditorNew::on_location_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    location_edited = true;
    qDebug() << "on_location_changed";
}
void EditorNew::on_item_type_changed(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    item_type_edited = true;
    qDebug() << "on_item_type_changed";
}
void EditorNew::on_comboBox_activated(const QString &arg1)
{
    factor_model->setAnaliticId(arg1);
    factor_edited = true;
}
void EditorNew::on_lineEdit_maxCorrel_textEdited(const QString &arg1)
{
    factor_model->setCorell(arg1.toFloat());
    factor_edited = true;
}
void EditorNew::on_lineEdit_maxErrorCount_textEdited(const QString &arg1)
{
    factor_model->setErrorCount(arg1.toFloat());
    factor_edited = true;
}
void EditorNew::on_lineEdit_countOfLost_textEdited(const QString &arg1)
{
    factor_model->setLostCount(arg1.toFloat());
    factor_edited = true;
}
