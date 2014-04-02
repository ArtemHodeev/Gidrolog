#include "itemtablemodel.h"
#include <QSqlQuery>
#include <databaseaccessor.h>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

ItemTableModel::ItemTableModel(QObject *parent) :
    TableModel(parent)
{

}

ItemTableModel::~ItemTableModel()
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
QVariant ItemTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    QVariant res;
    if (index.row() < rCount - 1 )
    {
        switch(index.column())
        {
        case 0:
            res = QVariant(items.at(index.row())->getId());
            break;
        case 1:
            res = QVariant(items.at(index.row())->getName());
            break;
        case 2:
            res = QVariant(items.at(index.row())->getTypeId());
            break;
        case 3:
            res = QVariant(items.at(index.row())->getMinValue());
            break;
        case 4:
            res = QVariant(items.at(index.row())->getErrorLine());
            break;
        }
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
int ItemTableModel::rowCount(const QModelIndex &parent) const
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
int ItemTableModel::columnCount(const QModelIndex &parent) const
{
    return cCount;
}

//bool ItemTableModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    if (row < 0 || count < 0)
//    {
//        return false;
//    }

//    beginInsertRows(QModelIndex(), 2, 2);

//    rCount += 1;

//    endInsertRows();

//    return true;

//}

bool ItemTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent,row,row + count - 1);
    for (int i = 0; i < count; i ++)
    {
        items_to_remove.append(items.at(row + i));
    }
    endRemoveRows();

    removeItems();
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
Qt::ItemFlags ItemTableModel::flags(const QModelIndex &index) const
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
QVariant ItemTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res;
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
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
            res = QVariant("Тип");
            break;
        case 3:
            res = QVariant("Минимальное занчение");
            break;
        case 4:
            res = QVariant("Ошибка измерения");
            break;
        }
    }
    else if (role == Qt::DisplayRole && orientation == Qt::Vertical)
    {
        if (section < rCount-1)
        {
            res = QVariant(section+1);
        }
        else
        {
            res = QVariant("+");
        }
    }
    return res;
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
bool ItemTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !index.isValid())
    {
        return false;
    }

    if (index.row() < rCount - 1)
    {
        switch(index.column())
        {
        case 1:
            items.at(index.row())->setName(value != "" ? value.toString() : index.data().toString());
            break;
        case 2:
            items.at(index.row())->setTypeId(value != "" ? value.toUInt() : index.data().toUInt());
            break;
        case 3:
            items.at(index.row())->setMinValue(value != "" ? value.toDouble() : index.data().toDouble());
            break;
        case 4:
            items.at(index.row())->setErrorLine(value != "" ? value.toDouble() : index.data().toDouble());
            break;
        };

        // Добавление строки в список изменений в бд
        if (!items_to_save.contains(items.at(index.row())))
        {
            items_to_save.append(items.at(index.row()));
        }

        emit(dataChanged(index,index));
    }
    else
    {
        if (value != "")
        {
            Item *i = new Item();
            switch(index.column())
            {
            case 1:
                i->setName(value.toString());
                break;
            case 2:
                i->setTypeId(value.toUInt());
                break;
            case 3:
                i->setMinValue(value.toDouble());
                break;
            case 4:
                i->setErrorLine(value.toDouble());
                break;
            };

            items.append(i);
            insertRows(rCount,1);
        }

    }
    return true;
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
void ItemTableModel::setItems()
{
    QSqlQuery *query = new QSqlQuery("SELECT id,name, type_id, min_value, error_line FROM gydro.item ORDER BY id", DatabaseAccessor::getDb());

    rCount = query->size() + 1;
    cCount = query->record().count();
    while (query->next())
    {
        Item *item = new Item();
        item->setId(query->value("id").toInt());
        item->setName(query->value("name").toString());
        item->setTypeId(query->value("type_id").toUInt());
        item->setMinValue(query->value("min_value").toDouble());
        item->setErrorLine(query->value("error_line").toDouble());

        items.append(item);
//        delete item;
    }
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
void ItemTableModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    QString sql = QString("UPDATE gydro.item ");
    sql += ("SET name = :name, type_id = :type_id, min_value = :min_value, error_line = :error_line ");
    sql += ("WHERE id = :id");
    query->prepare(sql);
    qDebug()<<"Size of list_to_save: " <<items_to_save.size();
    while(!items_to_save.isEmpty())
    {
        query->bindValue(":id",items_to_save.first()->getId());
        query->bindValue(":name", items_to_save.first()->getName());
        query->bindValue(":type_id", items_to_save.first()->getTypeId());
        query->bindValue(":min_value", items_to_save.first()->getMinValue());
        query->bindValue(":error_line",items_to_save.first()->getErrorLine());

        query->exec();
        items_to_save.removeFirst();
    }
    qDebug()<<"Size of list_to_save at the end: " <<items_to_save.size();
    delete query;
}
void ItemTableModel::removeItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    query->prepare("DELETE FROM gydro.item WHERE id = :item_id");
    qDebug() << "Items_to_remove befor: "<< items_to_remove.size();
    while(!items_to_remove.empty())
    {
        query->bindValue(":item_id",items_to_remove.first()->getId());
        query->exec();
        qDebug()<<query->lastError().text();

        items_to_remove.removeFirst();
    }
    qDebug() << "Items_to_remove after: "<< items_to_remove.size();

    delete query;
}

void ItemTableModel::actionSave_clicked()
{
    insertRows(4, 1);
}
void ItemTableModel::on_removeButton_clicked()
{
    removeRows(3,1);
}

/*
 * F:
 *  void ItemTableModel::saveItems()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция срабатывает при нажатии на кнопку "Сохранить" в Редакторе
 */
void ItemTableModel::on_saveButton_clicked()
{
    saveItems();
}

void ItemTableModel::on_pushButton_exit_clicked()
{
    saveItems();
    removeItems();
}

