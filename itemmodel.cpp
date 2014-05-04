#include "itemmodel.h"
#include <QSqlQuery>
#include <databaseaccessor.h>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

ItemModel::ItemModel(QObject *parent) :
    TableModel(parent)
{

}

ItemModel::~ItemModel()
{
}

/*
 * F:
 *  QVariant ItemModel::data(const QModelIndex &index, int role) const
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
QVariant ItemModel::data(const QModelIndex &index, int role) const
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
 *  int ItemModel::rowCount(const QModelIndex &parent) const
 * I:
 *  &parent - ссылка объект роделя модели, по умолчанию ссылка на NULL
 * O:
 *  Число строк в таблице модели
 * D:
 *
 */
//int ItemModel::rowCount(const QModelIndex &parent) const
//{
//    return rCount;
//}

/*
 * F:
 *  int ItemModel::columnCount(const QModelIndex &parent) const
 * I:
 *  &parent - ссылка объект роделя модели, по умолчанию ссылка на NULL
 * O:
 *  Число столбцов в таблице модели
 * D:
 *
 */
//int ItemModel::columnCount(const QModelIndex &parent) const
//{
//    return cCount;
//}



//bool ItemModel::removeRows(int row, int count, const QModelIndex &parent)
//{
//    beginRemoveRows(parent,row,row + count - 1);
//    for (int i = 0; i < count; i ++)
//    {
//        items_to_remove.append(items.at(row + i));
//    }
//    endRemoveRows();

//    removeItems();
//}

/*
 * F:
 *  Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const
 * I:
 * &index - ссылка на элемент модели
 * O:
 * Возвращает флаг доступности элемента для редактирования
 * D:
 * Если для текущего элемента модели редактируются данные из ячейки с столбца 0,
 *  то возвращается флаг запрета на редактирование;
 *  в остальных случаях возвращается флаг разрешения на редактирование
 */
Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

/*
 * F:
 *  QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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
QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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
 *  bool ItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
bool ItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !index.isValid())
    {
        return false;
    }

    if (value == QVariant(""))
    {
        return true;
    }

    Item *i;
    if (index.row() < rCount - 1)
    {
        i = items.at(index.row());
    }
    else
    {
        i = new Item();
    }

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

    i->setPosition(index.row());
    items.insert(index.row(),i);

    if (index.row() < rCount - 1)
    {
        if (items_to_update.contains(i) != true)
        {
            items_to_update.append(i);
        }
    }
    else
    {
        items_to_save.append(i);
        insertRows(rCount,1);
    }

    return true;
}

/*
 * F:
 *  void ItemModel::setItems()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция определяет количество строк и столцов будующей таблицы,
 *  согдалсно количеству строк и столбцов в таблице БД. выбираются все данные таблицы item
 *  из БД и формируется список элементов items
 */

void ItemModel::setItems()
{
    QSqlQuery *query = new QSqlQuery("SELECT id,name, type_id, min_value, error_line FROM item ORDER BY id", DatabaseAccessor::getDb());

    rCount = query->size() + 1;
    cCount = query->record().count();
    int pos = 0;
    while (query->next())
    {
        Item *item = new Item();
        item->setId(query->value("id").toInt());
        item->setName(query->value("name").toString());
        item->setTypeId(query->value("type_id").toUInt());
        item->setMinValue(query->value("min_value").toDouble());
        item->setErrorLine(query->value("error_line").toDouble());
        item->setPosition(pos);
        pos ++;
        items.append(item);
//        delete item;
    }
    delete query;
}

/*
 * F:
 *  void ItemModel::saveItems()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция перезаписывает в БД все объекты из списка items_to_save и опустошает его
 */
void ItemModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    QString sql = QString("INSERT INTO item (name, type_id, min_value, error_line)");
    sql += ("VALUES(:name,:type_id,:min_value,:error_line)");
    query->prepare(sql);

    while(!items_to_save.isEmpty())
    {
        query->bindValue(":name", items_to_save.first()->getName());
        query->bindValue(":type_id", items_to_save.first()->getTypeId());
        query->bindValue(":min_value", items_to_save.first()->getMinValue());
        query->bindValue(":error_line",items_to_save.first()->getErrorLine());
        query->exec();
        qDebug()<<"Error on save item: "<< query->lastError().text();

        items_to_save.removeFirst();
    }
    delete query;
}
void ItemModel::updateItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    QString sql = QString("UPDATE item ");
    sql += ("SET name = :name, type_id = :type_id, min_value = :min_value, error_line = :error_line ");
    sql += ("WHERE id = :id");
    query->prepare(sql);
    unsigned int i_id = 0;

    while(!items_to_update.isEmpty())
    {
        // Определение id параметра пробы
        // Если параметр пробы был уже БД и система знает его id
        if (items_to_update.first()->getId() > 0)
        {
            i_id = items_to_update.first()->getId();
        }
        //Если параметр пробы, был недавно добавлен и его редктировали
        else
        {
            QSqlQuery *q = new QSqlQuery(DatabaseAccessor::getDb());
            q->prepare("SELECT id FROM item");
            q->exec();
            q->seek(items_to_update.first()->getPosition());
            i_id = q->value("id").toUInt();
        }

        query->bindValue(":id", i_id);
        query->bindValue(":name", items_to_update.first()->getName());
        query->bindValue(":type_id", items_to_update.first()->getTypeId());
        query->bindValue(":min_value", items_to_update.first()->getMinValue());
        query->bindValue(":error_line",items_to_update.first()->getErrorLine());
        query->exec();

        items_to_update.removeFirst();
    }
    qDebug()<<"Size of list_to_save at the end: " <<items_to_update.size();
    delete query;
}
void ItemModel::removeItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    query->prepare("DELETE FROM item WHERE id = :item_id");

    while(!items_to_delete.empty())
    {
        query->bindValue(":item_id",items_to_delete.first()->getId());
        query->exec();
        qDebug()<<query->lastError().text();

        items_to_delete.removeFirst();
    }

    delete query;
}
void ItemModel::setItemsToDelete(int *mass)
{
    int count = 0;
    unsigned int first = mass[count];
    unsigned int index = -1;

    while(mass[count] != -1)
    {
        index= findItemInPosition(mass[count]);
        if (index != -1)
        {
            items_to_delete.append(items[index]);
            int update_index = items_to_update.indexOf(items[index]);
            int save_index = items_to_save.indexOf(items[index]);

            // Если удаляема проба есть в списках на добавление новых проб или обновление,
            // то ее сразу удалить из этих списков.
            if (update_index >= 0)
            {
                items_to_update.removeAt(update_index);
            }
            if (save_index >= 0)
            {
                items_to_save.removeAt(save_index);
            }

            items.remove(index);
        }

        count ++;
    }
//    qDebug()<<"Items_to_delete size: "<<items_to_delete.size();
    removeRows(first,count);
}

int ItemModel::findItemInPosition(unsigned int pos)
{
    int i = -1;
    while (items[++i]->getPosition() != pos && i < items.size());

    return (items[i]->getPosition() == pos) ? i : -1;
}

