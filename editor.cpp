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
#include <QVariant>
#include <comboboxdelegate.h>
#include <watertypecombobox.h>
#include <QMessageBox>
#include <QPushButton>
#include <itemtypecombobox.h>
#include <spinboxdelegate.h>

Editor::Editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    water_sign = false;
    location_sign = false;
    factor_sign = false;
    water_edited = false;
    location_edited = false;
    item_edited = false;
    factor_edited = false;
    item_model = new ItemModel();
    connect(item_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(on_item_changed()));
    item_type_sign = false;

    item_type_model = new ItemTypeModel();


    //Первичная установка модели данных раздела компоненты
    ui->stackedWidget->setCurrentIndex(0);
    item_model->setItems();
    setUi(0,item_model);
 //   item_model->setItems();
//    item_type_model->setItems();


//    setUi(0,item_model);
//    item_type_model->setItems();
//    setUi(0,item_type_model);

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
    {
//        Типы компонентов
        item_type_model->setItems();
        setUi(0,item_type_model);
        setUi(cur, item_type_model);
        ui->stackedWidget->setCurrentIndex(0);
        break;
    }
    case 1:
    {
//        Критерии

        if (factor_sign == false)
        {
            factor_model = new FactorModel();
            factor_model->setItems();

            factor_sign = true;
            ui->comboBox->addItems(factor_model->getWaterTypes());
        }
        QString corell = QVariant(factor_model->getCorell()).toString();
        QString lost = QVariant(factor_model->getLostCount()).toString();
        QString error = QVariant(factor_model->getErrorCount()).toString();

        ui->lineEdit_maxCorrel->setText(corell);
        ui->lineEdit_countOfLost->setText(lost);
        ui->lineEdit_maxErrorCount->setText(error);
        ui->comboBox->setCurrentText(factor_model->getAnaliticName());
       ui->comboBox->setEditable(true);

        ui->stackedWidget->setCurrentIndex(1);
        break;
    }
    case 2:
//        Объекты (Места взятия)
        if (location_sign == false)
        {
            location_model = new LocationModel();
            location_model->setItems();
            location_sign = true;
            connect(location_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(on_location_changed()));
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
            connect(water_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(on_water_changed()));
        }
        setUi(cur,water_model);
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case 4:
//            Компоненты
        if (item_type_sign == false)
        {
            item_model = new ItemModel();
            item_model->setItems();
            item_type_sign = true;
        }
        setUi(cur,item_model);
        ui->stackedWidget->setCurrentIndex(0);
        break;
    };
}
Editor::~Editor()
{
    delete ui;
    delete sel_model;
    delete item_model;
    if (factor_sign == true)
    {
        delete factor_model;
    }

//    delete item_model;
    if (item_type_sign == true)
    {
        delete item_type_model;
    }


    if (location_sign == true)
    {
        delete location_model;
    }
    if (water_sign == true)
    {
        delete water_model;
    }
    if (item_type_sign == true)
    {
        delete item_model;
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
            if (i->row() < current_model->rowCount(QModelIndex()) - 1)
            {
                sel_rows[ind] = i->row();
                ind ++;
            }
        }
        sel_rows[ind] = -1;

        // Удаление строк
        current_model->setItemsToDelete(sel_rows);
        switch(ui->listWidget_editorMenu->currentIndex().row())
        {
        case 0:
            item_edited = true;
            break;
        case 2:
            location_edited = true;
            break;
        case 3:
            water_edited = true;
            break;
        default:
            break;
        }

        delete sel_rows;
    }
}
void Editor::on_pushButton_exit_pressed()
{
    save();
}
void Editor::save()
{
    if (item_edited == true)
    {
        saveModel(item_model);
    }
    saveModel(item_type_model);

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
    if (item_type_sign == true)
    {
        saveModel(item_model);
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
    current_model = model;
    ui->tableView_itemInSystem->setModel(model);
    ui->tableView_itemInSystem->resizeColumnsToContents();



    sel_model = new QItemSelectionModel(model);

    ui->tableView_itemInSystem->setSelectionModel(sel_model);
    ui->tableView_itemInSystem->setSelectionMode(QAbstractItemView::ExtendedSelection);

    switch (index)
    {
    case 0:
    {
        ui->label_page->setText("Типы компонентов");
        SpinBoxDelegate *type_delegate = new SpinBoxDelegate();

        ui->tableView_itemInSystem->setItemDelegateForColumn(2, type_delegate);

        break;
    }
    case 2:
    {
        ui->label_page->setText("Объекты");
        break;
    }
    case 3:
        ui->label_page->setText("Типы водных масс");
        break;
    case 4:
    {
        ui->label_page->setText("Компоненты");

        ComboboxDelegate *item_type_delegate = new Itemtypecombobox();
        ui->tableView_itemInSystem->setItemDelegateForColumn(2, item_type_delegate);

        break;
    }
    default:
        break;
    }
}

bool Editor::maybeSave()
{
    QMessageBox msg;
    QAbstractButton *cancel = msg.addButton("Отменить",QMessageBox::RejectRole);
    QAbstractButton *ok = msg.addButton("Cохранить",QMessageBox::AcceptRole);

    msg.setText("Данные были изменены. Сохранить изменения?");
    msg.setIcon(QMessageBox::Question);
    msg.exec();

    return (msg.clickedButton() == ok) ? true : false;
}
void Editor::closeEvent(QCloseEvent *event)
{
    if (item_edited == true || location_edited == true || water_edited == true || factor_edited == true)
    {
        if (maybeSave() == true)
        {
            save();
        }
    }
}

void Editor::on_item_changed()
{
    item_edited = true;
}
void Editor::on_water_changed()
{
    water_edited = true;
}
void Editor::on_location_changed()
{
    location_edited = true;
}

void Editor::on_comboBox_activated(const QString &arg1)
{
    factor_model->setAnaliticId(arg1);
    factor_edited = true;
}

void Editor::on_lineEdit_maxCorrel_textEdited(const QString &arg1)
{
    factor_model->setCorell(arg1.toFloat());
    factor_edited = true;
}

void Editor::on_lineEdit_maxErrorCount_textEdited(const QString &arg1)
{
    factor_model->setErrorCount(arg1.toFloat());
    factor_edited = true;
}

void Editor::on_lineEdit_countOfLost_textEdited(const QString &arg1)
{
    factor_model->setLostCount(arg1.toFloat());
    factor_edited = true;
}
