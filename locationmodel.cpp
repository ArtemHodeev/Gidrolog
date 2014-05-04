#include "locationmodel.h"
#include <databaseaccessor.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>


LocationModel::LocationModel(QObject *parent) :
    TableModel(parent)
{
}

LocationModel::~LocationModel()
{
//    items.clear();
}

void LocationModel::setItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

    sql = "SELECT id, name, width, length, deep FROM location";
    query->prepare(sql);
    query->exec();

    rCount = query->size() + 1;
    cCount = query->record().count();
    int pos = 0;

    while (query->next())
    {
        Location *loc = new Location();

        loc->setId(query->value("id").toUInt());
        loc->setName(query->value("name").toString());
        loc->setWidth(query->value("width").toFloat());
        loc->setLength(query->value("length").toFloat());
        loc->setDeep(query->value("deep").toFloat());
        loc->setPosition(pos);
        items.append(loc);
        pos++;
    }
    qDebug()<<"Locations size: "<<items.size();
    delete query;
}

QVariant LocationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res;
    if (role != Qt::DisplayRole)
    {
        return res;
    }

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
            res = QVariant("Широта");
            break;
        case 3:
            res = QVariant("Долгота");
            break;
        case 4:
            res = QVariant ("Глубина");
            break;
        default:
            break;
        }
    }
    else
    {
        res = (section < rCount - 1) ? QVariant(section + 1) : QVariant("+");
    }
    return res;
}
QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if (index.isValid() != true || role != Qt::DisplayRole || index.row() >= rCount - 1)
    {
        return res;
    }

    switch(index.column())
    {
    case 0:
        res = items[index.row()]->getId();
        break;
    case 1:
        res = items[index.row()]->getName();
        break;
    case 2:
        res = items[index.row()]->getWidth();
        break;
    case 3:
        res = items[index.row()]->getLength();
        break;
    case 4:
        res = items[index.row()]->getDeep();
        break;
    }


//    items[index.row()]->setPosition(index.row());
    return res;

}

bool LocationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() != true || value == QVariant("") || role != Qt::EditRole)
    {
        return false;
    }

    int row = index.row();
    Location *loc;
    qDebug()<<"items size: "<< items.size();
    loc = (row < rCount -1) ? items[index.row()] : new Location();

    switch (index.column())
    {
    case 1:
        loc->setName(value.toString());
        break;
    case 2:
        loc->setWidth(value.toFloat());
        break;
    case 3:
        loc->setLength(value.toFloat());
        break;
    case 4:
        loc->setDeep(value.toFloat());
        break;
    default:
        break;
    }

    loc->setPosition(row);

    if (row < rCount - 1)
    {
        items[row] = loc;
        if (items_to_update.contains(loc) != true)
        {
            items_to_update.append(loc);
        }
    }
    else
    {
        items.append(loc);
        items_to_save.append(loc);
        insertRows(rCount,1);
    }
    return true;
}

Qt::ItemFlags LocationModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags res = Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    if (index.column() == 0)
    {
        res = Qt::ItemIsEnabled;
    }
    return res;
}

void LocationModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int last_id;
    int pos = -1;

    sql = "INSERT INTO location (name, width, length, deep)";
    sql += "VALUES (:name, :width, :length, :deep)";
    query->prepare(sql);

    while(items_to_save.isEmpty() != true)
    {
        query->bindValue(":name",items_to_save.first()->getName());
        query->bindValue(":width",items_to_save.first()->getWidth());
        query->bindValue(":length", items_to_save.first()->getLength());
        query->bindValue(":deep", items_to_save.first()->getDeep());
        query->exec();

        last_id = query->lastInsertId().toUInt();
        pos = findItemInPosition(items_to_save.first()->getPosition());
        items[pos]->setId(last_id);
        items_to_save.removeFirst();
    }
    delete query;
}
void LocationModel::updateItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    Location *loc = new Location();

    sql = "UPDATE location ";
    sql += "SET (name = :name, width = :width, length = :length, deep = :deep) ";
    sql += "WHERE id = :id";
    query->prepare(sql);

    while (items_to_update.isEmpty() != true)
    {
        loc = items_to_update.first();
        query->bindValue("id", loc->getId());
        query->bindValue(":name", loc->getName());
        query->bindValue(":width", loc->getWidth());
        query->bindValue(":length", loc->getLength());
        query->bindValue(":deep", loc->getDeep());
        query->exec();

        items_to_update.removeFirst();
    }
    delete loc;
    delete query;
}

void LocationModel::removeItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    int pos = -1;

    sql = "DELETE FROM location WHERE id = :id";
    query->prepare(sql);

    while(items_to_delete.isEmpty() != true)
    {
        query->bindValue(":id", items_to_delete.first()->getId());
        query->exec();

        items_to_delete.removeFirst();
    }
    delete query;
}

int LocationModel::findItemInPosition(unsigned int pos)
{
    int i = -1;
    while (items[++i]->getPosition() != pos && i < items.size());

    return (items[i]->getPosition() == pos) ? i : -1;
}

void LocationModel::setItemsToDelete(int *mass)
{
    int count = 0;
    int first = mass[0];
    int index = -1;

    while (mass[count] != -1)
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
    removeRows(first,count);
}
