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

    //Первичная установка моделеи данных раздела компоненты
    ui->stackedWidget->setCurrentIndex(0);
    model = new ItemModel();
    model->setItems();

    ui->tableView_itemInSystem->setModel(model);
    sel_model = new QItemSelectionModel(model);

    ui->tableView_itemInSystem->setSelectionModel(sel_model);
    ui->tableView_itemInSystem->setSelectionMode(QAbstractItemView::ExtendedSelection);
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
        model = new ItemModel();
        break;
    case 1:
//        Критерии
    case 2:
//        Объекты (Места взятия)
        model = new LocationModel();
        break;
    case 3:
//        Типы водных масс
        model = new WaterTypeModel();
        break;
    case 4:
//        Типы элементов
        model = new ItemTypeModel();
        break;
    };// switch(cur)

    // Наполенение модели данными данных
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
        unsigned int *sel_rows; // массив, элементы которого номера выделенных строк

        sel_rows = new unsigned int[sel_model->selectedRows().size() + 1];
        list= sel_model->selectedRows();

        // Определяются номеров строк для удаления
        for (i = list.begin(); i < list.end(); i ++)
        {
             sel_rows[ind] = i->row();
             ind ++;
        }
        sel_rows[ind] = -1;

        // Удаление строк
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
