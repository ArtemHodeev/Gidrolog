#include "itemtypemodel.h"
#include <databaseaccessor.h>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

ItemTypeModel::ItemTypeModel(QObject *parent):
    TableModel(parent)
{
}
ItemTypeModel::~ItemTypeModel()
{
}

/*
 * F:
 *  QVariant ItemTableModel::data(const QModelIndex &index, int role) const
 * I:
 *  index - индекс элемента(текущего) в моделе
 *  role - теукщфа роль
 * O:
 *  Возвращает строку данных в ячейку модели
 * D:
 *  Если текущая роль для элемента модели Qt::DisplayRole (отображать на экране) и индекс элемента модели корректен,
 *  то функция для каждой ячейки модели(таблицы) берет данные
 *  элемента списка items в позиции равной номеру строки и отображает данные
 *  его полей в соответствующую ячейку таблицы
 */
QVariant ItemTypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    QVariant res;
    if (index.row() < rCount - 1)
    {
        switch(index.column())
        {
        case 0:
            res = items.at(index.row())->getId();
            break;
        case 1:
            res = items.at(index.row())->getName();
            break;
        case 2:
            res = items.at(index.row())->getPriority();
            break;
        };
    }
    else
    {
        res = QVariant("");
    }

    return res;
}

/*
 * F:
 *  int ItemTableModel::rowCount(const QModelIndex &parent) const
 * I:
 *  &parent - ссылка объект роделя модели, по умолчанию ссылка на NULL
 * O:
 *  Число строк в таблице модели
 * D:
 *
 */
int ItemTypeModel::rowCount(const QModelIndex &parent) const
{
    return rCount;
}

/*
 * F:
 *  int ItemTableModel::columnCount(const QModelIndex &parent) const
 * I:
 *  &parent - ссылка объект роделя модели, по умолчанию ссылка на NULL
 * O:
 *  Число столбцов в таблице модели
 * D:
 *
 */
int ItemTypeModel::columnCount(const QModelIndex &parent) const
{
    return cCount;
}

/*
 * F:
 *  bool ItemTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
 * I:
 *  &index - индекс текущего элемента модели
 *  &value - данные, которые заносятся в ячейку таблицы
 *  role - роль текущего эелемента модели
 * O:
 *  true - данные внесены в таблицу
 *  false - ошибка; данные не занесены в таблицу
 * D:
 *  если для текущего элемента модели разрешено редактирование и индекс элемента корректен,
 *  то функци заносит в текущую ячейку таблицы значение value, если value != "",
 *  иначе в ячейку заносится прежнее значение. Элемент списка items заносится в список элементов
 *  items_to_save, которые будут сохранены в БД. Если для одной строки таблицы редактируются
 *  несколько полей, то в список items_to_save элемент списка items с индексом равным номеру
 *  строки занесется один раз
 */
bool ItemTypeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole || !index.isValid())
    {
        return false;
    }
    ItemType *i = new ItemType();

    if (value == QVariant(""))
    {
        return true;
    }

    switch(index.column())
    {
    case 1:
        i->setName(value.toString());
        break;
    case 2:
        i->setPriority(value.toUInt());
        items.at(index.row())->setPriority(value != "" ?  : index.data().toUInt());
        break;
    }

    if (index.row() < rCount - 1)
    {
        items.insert(index.row(), i);
    }
    else
    {
        items.append(i);
    }
//    if (index.row() < rCount - 1)
//    {
//        switch(index.column())
//        {
//        case 1:
//            items.at(index.row())->setName(value != "" ? value.toString() : index.data().toString());
//            break;
//        case 2:
//            items.at(index.row())->setPriority(value != "" ? value.toUInt() : index.data().toUInt());
//            break;
//        }
//        if (!items_to_save.contains(items.at(index.row())))
//        {
//            items_to_save.append(items.at(index.row()));
//        }
//    }
//    else
//    {
//        switch(index.column())
//        {
//        case 1:
//            items.at(index.row())->setName(value != "" ? value.toString() : index.data().toString());
//            break;
//        case 2:
//            items.at(index.row())->setPriority(value != "" ? value.toUInt() : index.data().toUInt());
//            break;
//        }
//    }
//    emit(dataChanged(index,index));
    return true;
}

/*
 * F:
 *  Qt::ItemFlags ItemTableModel::flags(const QModelIndex &index) const
 * I:
 * &index - ссылка на элемент модели
 * O:
 * Возвращает флаг доступности элемента для редактирования
 * D:
 * Если для текущего элемента модели редактируются данные из ячейки с столбца 0,
 *  то возвращается флаг запрета на редактирование;
 *  в остальных случаях возвращается флаг разрешения на редактирование
 */
Qt::ItemFlags ItemTypeModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

/*
 * F:
 *  QVariant ItemTableModel::headerData(int section, Qt::Orientation orientation, int role) const
 * I:
 *  int section - номер столбца
 *  Qt::Orientation orientation - расположение заголовка: вертикальное или горизонтальное
 *  int role - роль текущего заголовка: видим не видим на экране
 * O:
 *  Строка - заголовок для столбца
 * D:
 *  Функция задает названия столбцов согласно названиям полей класса Item,
 *  названием строки является порядковый номер строки в моделе
 */
QVariant ItemTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    QVariant res;
    if (orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            res = QVariant("Id");
            break;
        case 1:
            res = QVariant("Название");
            break;
        case 2:
            res = QVariant("Приоритет");
            break;
        }
    }
    else if (orientation == Qt::Vertical)
    {
        res = QVariant(section + 1);
    }
    return res;
}

/*
 * F:
 *  void ItemTableModel::setItems()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция определяет количество строк и столцов будующей таблицы,
 *  согдалсно количеству строк и столбцов в таблице БД. выбираются все данные таблицы item
 *  из БД и формируется список элементов items
 */
void ItemTypeModel::setItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    query->prepare("SELECT id, name, priority FROM item_type");
    query->exec();

    //Определение размеров таблицы rCount, cCount (см. tablemodel.h)
    rCount = query->size() + 1 ;
    cCount = query->record().count();

    while(query->next())
    {
        ItemType *item = new ItemType();

        item->setId(query->value("id").toUInt());
        item->setName(query->value("name").toString());
        item->setPriority(query->value("priority").toUInt());

        items.append(item);
//        delete item;
    }
    qDebug()<< "Items list size: "<<items.size();
    delete query;
}

/*
 * F:
 *  void ItemTableModel::saveItems()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция перезаписывает в БД все объекты из списка items_to_save и опустошает его
 */
void ItemTypeModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    query->prepare("UPDATE item_type SET name = :name, priority = :priority WHERE id = :id");
    while (!items_to_save.isEmpty())
    {
        query->bindValue(":id", items_to_save.first()->getId());
        query->bindValue(":name", items_to_save.first()->getName());
        query->bindValue(":priority", items_to_save.first()->getPriority());
        query->exec();

        items_to_save.removeFirst();
    }
}

void ItemTypeModel::updateItems()
{}
void ItemTypeModel::removeItems()
{}
