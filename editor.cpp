#include "editor.h"
#include "ui_editor.h"
#include <QDebug>
#include <databaseaccessor.h>
#include <tablemodel.h>
#include <QSqlError>
#include <itemmodel.h>
#include <itemtypemodel.h>
#include <watertypemodel.h>
#include <locationmodel.h>

Editor::Editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    water_sign = false;
    location_sign = false;
    item_model = new ItemModel();

    //Первичная установка моделеи данных раздела компоненты
    ui->stackedWidget->setCurrentIndex(0);
    item_model->setItems();
    setUi(0,item_model);

}
/*
 * F:
 *  void Editor::on_listWidget_editorMenu_clicked()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция является слотом и реагирует на кликание мышкой на списке разделов слева.
 *  В зависимости от выбранного раздела, устанавливается новая модель данных. Старая удаляется.
*/
void Editor::on_listWidget_editorMenu_clicked()
{
    int cur = ui->listWidget_editorMenu->currentIndex().row(); // индекс текущего раздела

    // Выбор модели данных
    switch(cur)
    {
    case 0:
//        Компоненты
        setUi(cur,item_model);
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case 1:
//        Критерии
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case 2:
//        Объекты (Места взятия)
        if (location_sign == false)
        {
            location_model = new LocationModel();
            location_model->setItems();
            location_sign = true;
        }
        setUi(cur,location_model);
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case 3:
//        Типы водных масс
        if (water_sign == false)
        {
            water_model = new WaterTypeModel();
            water_model->setItems();
            water_sign = true;
        }
//        current_model = water_model;
        setUi(cur,water_model);
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case 4:
//        Типы элементов
//        model = new ItemTypeModel();
        break;
    };// switch(cur)
}
Editor::~Editor()
{
    delete ui;
    delete sel_model;
    delete item_model;

    if (location_sign == true)
    {
        delete location_model;
    }
    if (water_sign == true)
    {
        delete water_model;
    }
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
void Editor::keyPressEvent(QKeyEvent *key_event)
{
    if (key_event->matches(QKeySequence::Delete) && sel_model->selectedRows().size() > 0)
    {
        QModelIndexList list; // список выделенных строк
        QModelIndexList::iterator i; // итератор по выделенным строкам
        int ind = 0;
        int *sel_rows; // массив, элементы которого номера выделенных строк

        sel_rows = new int[sel_model->selectedRows().size() + 1];
        list= sel_model->selectedRows();

        // Определяются номеров строк для удаления
        for (i = list.begin(); i < list.end(); i ++)
        {
             sel_rows[ind] = i->row();
             ind ++;
        }
        sel_rows[ind] = -1;

        // Удаление строк
        current_model->setItemsToDelete(sel_rows);

        delete sel_rows;
    }
}
void Editor::on_pushButton_exit_pressed()
{
    saveModel(item_model);

    if (location_sign == true)
    {
        saveModel(location_model);
    }
    if (water_sign == true)
    {
        saveModel(water_model);
    }
}
void Editor::saveModel(TableModel *model)
{
    model->saveItems();
    model->updateItems();
    model->removeItems();

}
void Editor::setUi(int index, TableModel *model)
{
    switch (index)
    {
    case 0:
        ui->label_page->setText("Компоненты");
        break;
    case 2:
        ui->label_page->setText("Объекты");
        break;
    case 3:
        ui->label_page->setText("Типы водных масс");
        break;
    case 4:
        ui->label_page->setText("Типы компонентов");
        break;
    default:
        break;
    }

    current_model = model;
    ui->tableView_itemInSystem->setModel(model);
    sel_model = new QItemSelectionModel(model);

    ui->tableView_itemInSystem->setSelectionModel(sel_model);
    ui->tableView_itemInSystem->setSelectionMode(QAbstractItemView::ExtendedSelection);
}
