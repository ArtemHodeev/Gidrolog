#include "confirmimport.h"
#include "ui_confirmimport.h"
#include <itemtypecombobox.h>

ConfirmImport::ConfirmImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmImport)
{
    ui->setupUi(this);

    // По умолчанию все таблицы для моделей не видимы на экране
    ui->params_widget->setHidden(true);
    ui->location_widget->setHidden(true);
    ui->water_widget->setHidden(true);

    cancel = false;
    param_sign = false;
    water_sign = false;
    location_sign = false;
}
/*
 *F:
 *   void ConfirmImport::setParamModel(ConfirmItemModel *model)
 * I:
 *  ConfirmItemModel *model - модель, которую следует отобразить на экране
 * O:
 *  --
 * D:
 *  Функция устанавливает модель отображения для неизвестных компонентов проб.
 *  Функция изначально делает кнопку "Сохранить", неактивно, т.к. не заполено обязательное поле "Тип компонента"
 *  Функция связывает сигнал при изменении в моделе данных поля "Тип компонента", и обработчика данного события
 *  Функция делает видимой на экране таблицу с компонентами
*/
void ConfirmImport::setParamModel(ConfirmItemModel *model)
{
    // Установка модели
    param_model = new ConfirmItemModel();
    Itemtypecombobox *combobox = new Itemtypecombobox();
    param_model = model;

    // Установка "слушателя" на изменение данных поля "Тип компонента"
    connect(param_model,SIGNAL(on_itemChanged()),this,SLOT(on_itemChanged_emited()));

    // Отображение модели на экране
    ui->pushButton_save->setEnabled(false);
    ui->params_widget->setHidden(false);
    ui->params_tableView->setModel(model);
    ui->params_tableView->setItemDelegateForColumn(2, combobox);
    param_sign = true;
}

/*
 *F:
 *   void ConfirmImport::setWaterModel(ConfirmWaterTypeModel *model)
 * I:
 *  ConfirmWaterTypeModel *model - модель, которую следует отобразить на экране
 * O:
 *  --
 * D:
 *  Функция устанавливает модель отображения для неизвестных типов водных масс.
 *  Функция делает видимой на экране таблицу с типами водных масс
 */
void ConfirmImport::setWaterModel(ConfirmWaterTypeModel *model)
{
    // Установка модели
    water_model = new ConfirmWaterTypeModel();
    water_model = model;

    // Отображение модели на экране
    ui->water_widget->setHidden(false);
    ui->water_tableView->setModel(water_model);
    water_sign = true;
}

/*
 *F:
 *   void ConfirmImport::etLocationModel(ConfirmLocationModel *model)
 * I:
 *  ConfirmLocationModel *model - модель, которую следует отобразить на экране
 * O:
 *  --
 * D:
 *  Функция устанавливает модель отображения для неизвестных мест взяти проб.
 *  Функция делает видимой на экране таблицу с местами взятия
 */
void ConfirmImport::setLocationModel(ConfirmLocationModel *model)
{
    location_model = new ConfirmLocationModel();
    location_model = model;
    ui->location_widget->setHidden(false);
    ui->location_tableView->setModel(location_model);

    location_sign = true;
}

/*
 *F:
 *   ConfirmImport::~ConfirmImport()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция - деструктор - удаляет существующие модели.
 */
ConfirmImport::~ConfirmImport()
{
    if (param_sign ==true)
    {
        delete param_model;
    }
    if (water_sign == true)
    {
        delete water_model;
    }
    if(location_sign == true)
    {
        delete location_model;
    }
    delete ui;
}

/*
 *F:
 *   void ConfirmImport::on_pushButton_save_clicked()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция вызывает функцию сохранения данных для всех созданных моделей.
 */
void ConfirmImport::on_pushButton_save_clicked()
{
    if (param_sign ==true)
    {
        param_model->saveItems();
    }
    if (water_sign == true)
    {
        water_model->saveItems();
    }
    if (location_sign == true)
    {
        location_model->saveItems();
    }
}

/*
 *F:
 *   void ConfirmImport::on_itemChanged_emited()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция проверяет: есть ли не заполненое поле "Тип компонента" в таблице компонентов
 *  Если есть, то делает кнопку "Сохранить" неактивной, если нет, то деает кнопку "Сохранить" активной
 */
void ConfirmImport::on_itemChanged_emited()
{
    bool enable = !param_model->hasEmptyType();
    ui->pushButton_save->setEnabled(enable);
}

/*
 *F:
 *   void ConfirmImport::on_pushButton_cancel_clicked()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция слотом для кнопки "Отменить", вызывается при нажатии на данныю кнопку и устанавливает флаг cancel
 *  в true
 */
void ConfirmImport::on_pushButton_cancel_clicked()
{
    cancel = true;
}

/*
 *F:
 *   ConfirmImport::isCanceled()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция значение флага cancel
 */
bool ConfirmImport::isCanceled()
{
    return cancel;
}
