#include "itemmodel.h"
#include <QSqlQuery>
#include <databaseaccessor.h>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <names.h>


ItemModel::ItemModel(QObject *parent) :
    TableModel(parent)
{
     item_types = new QHash<QString, unsigned int>();
     setItemType();
}

ItemModel::~ItemModel()
{
    delete item_types;
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
    if (!index.isValid() )
    {
        return QVariant();
    }
    if (role == Qt::EditRole)
    {
        return index.data();
    }
    else if(role == Qt::DisplayRole)
    {
        QVariant res = QVariant("");
        unsigned int item_type_id;
        QString item_type_name = "";

        if (index.row() < rCount - 1 )
        {
            switch(index.column())
            {
            case 0:
            {
                res = QVariant(items.at(index.row())->getId());
                break;
            }
            case 1:
            {
                res = QVariant(items.at(index.row())->getName());
                break;
            }
            case 2:
            {
                item_type_id = items.at(index.row())->getTypeId();
                item_type_name = item_types->key(item_type_id);
                res = QVariant(item_type_name);
                //res = QVariant(items.at(index.row())->getTypeId());
                break;
            }
            case 3:
            {
                res = QVariant(items.at(index.row())->getMinValue());
                break;
            }
            case 4:
            {
                res = QVariant(items.at(index.row())->getErrorLine());
                break;
            }
            case 5:
            {
                res = QVariant(items.at(index.row())->getDisplay());
                break;
            }
            default :
                break;
    //        case 5:
    //        {
    ////            if (role == Qt::CheckStateRole)
    ////            {
    ////                return Qt::Checked;
    ////            }
    //            res = QVariant(items.at(index.row())->getDisplay());
    //            break;
    //        }
            }
        }
        return res;
    }
    else
    {
         return QVariant();
    }
}

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
    else if(index.column() == 5)
    {
        return Qt::ItemIsEditable | Qt::ItemIsUserCheckable | QAbstractTableModel::flags(index);
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
            res = QVariant("Минимальное значение");
            break;
        case 4:
            res = QVariant("Ошибка измерения");
            break;
        case 5:
            res = QVariant("Отображать компоненты?");
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
    if (role != Qt::EditRole || !index.isValid() || value == QVariant(""))
    {
        return false;
    }

    int row = index.row();

    Item *i;
    bool sign = false;

    i = (row < rCount -1) ? items[index.row()] : new Item();

    switch(index.column())
    {
    case 1:
    {
        i->setName(value.toString());
        break;
    }
    case 2:
    {
        unsigned int item_type_id = item_types->value(value.toString());
        i->setTypeId(item_type_id);
        sign = true;
        //i->setTypeId(value.toUInt());
        break;
    }
    case 3:
    {
        i->setMinValue(value.toDouble());
        break;
    }
    case 4:
    {
        i->setErrorLine(value.toDouble());
        break;
    }
    case 5:
    {
        i->setDisplay(value.toBool());
        break;
    }
    };

    i->setPosition(row);

    if (row < rCount - 1)
    {
        items[row] = i;
        if (items_to_update.contains(i) != true)
        {
            items_to_update.append(i);
        }
    }
    else
    {
//        i->setDisplay(true);
        items.append(i);
        items_to_save.append(i);
        insertRows(rCount,1);
    }
    emit(dataChanged(index, index));
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
    QSqlQuery *query = new QSqlQuery("SELECT id,name, type_id, min_value, error_line, display FROM item ORDER BY id", DatabaseAccessor::getDb());

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
        item->setDisplay(query->value("display").toBool());
        item->setPosition(pos);
        pos ++;
        items.append(item);
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

    QString sql = QString("INSERT INTO item (name, type_id, min_value, error_line, display)");
    sql += ("VALUES(:name,:type_id,:min_value,:error_line, :display)");
    query->prepare(sql);

    while(!items_to_save.isEmpty())
    {
        query->bindValue(":name", items_to_save.first()->getName());
        query->bindValue(":type_id", items_to_save.first()->getTypeId());
        query->bindValue(":min_value", items_to_save.first()->getMinValue());
        query->bindValue(":error_line",items_to_save.first()->getErrorLine());
        query->bindValue(":display", items_to_save.first()->getDisplay());
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
    sql += ("SET name = :name, type_id = :type_id, min_value = :min_value, error_line = :error_line, display = :display ");
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
        //Если параметр пробы, был недавно добавлен и его редактировали
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
        query->bindValue(":display", items_to_update.first()->getDisplay());
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
        index = findItemInPosition(mass[count]);
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
    for (int i=0; i < items.size(); i++)
    {
        items[i]->setPosition(i);
    }
    if (first >= 0)
        removeRows(first+1,count);
}

int ItemModel::findItemInPosition(int pos)
{
    int i = 0;
    while (i < items.size() && items[i]->getPosition() != pos)
    {
        i ++;
    }

    return (i<items.size()) ? i : -1;
}

void ItemModel::setItemType()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    QString name = "";
    unsigned int id;
    sql = "SELECT id, name FROM item_type";
    query->exec(sql);

    while(query->next())
    {
        id = query->value("id").toUInt();
        name = query->value("name").toString();
        Names::item_types->insert(name, id);
        item_types->insert(name, id);
    }

    delete query;
}
