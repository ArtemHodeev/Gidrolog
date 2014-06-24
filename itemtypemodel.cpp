#include "itemtypemodel.h"
#include <databaseaccessor.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>


ItemTypeModel::ItemTypeModel(QObject *parent) :
    TableModel(parent)
{

}


ItemTypeModel::~ItemTypeModel()
{
//    items.clear();
}

void ItemTypeModel::setItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

    sql = "SELECT id, name, priority FROM item_type";
    query->prepare(sql);
    query->exec();

    rCount = query->size() + 1;
    cCount = query->record().count();
    int pos = 0;

    while (query->next())
    {
        ItemType *item_type = new ItemType();

        item_type->setId(query->value("id").toUInt());
        item_type->setName(query->value("name").toString());
        item_type->setPriority(query->value("priority").toUInt());
        item_type->setPosition(pos);
        items.append(item_type);
        pos++;
    }
    qDebug()<<"ItemTypes size: "<<items.size();
    delete query;
}

QVariant ItemTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            res = QVariant("Приоритет");
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
QVariant ItemTypeModel::data(const QModelIndex &index, int role) const
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
        QVariant res;
        if (index.row() < rCount - 1 )
        {
            switch(index.column())
            {
            case 0:
                res = items[index.row()]->getId();
                break;
            case 1:
                res = items[index.row()]->getName();
                break;
            case 2:
                res = items[index.row()]->getPriority();
                break;
            }
        }
        return res;
    }
    else
    {
         return QVariant();
    }
}

bool ItemTypeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() != true || value == QVariant("") || role != Qt::EditRole)
    {
        return false;
    }

    int row = index.row();
    ItemType *item_type;
    qDebug()<<"items size: "<< items.size();
    item_type = (row < rCount -1) ? items[index.row()] : new ItemType();

    switch (index.column())
    {
    case 1:
        item_type->setName(value.toString());
        break;
    case 2:
        item_type->setPriority(value.toUInt());
        break;
    default:
        break;
    }

    item_type->setPosition(row);

    if (row < rCount - 1)
    {
        items[row] = item_type;
        if (items_to_update.contains(item_type) != true)
        {
            items_to_update.append(item_type);
        }
    }
    else
    {
        items.append(item_type);
        items_to_save.append(item_type);
        insertRows(rCount,1);
    }
    emit(dataChanged(index, index));
    return true;
}

Qt::ItemFlags ItemTypeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags res = Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    if (index.column() == 0)
    {
        res = Qt::ItemIsEnabled;
    }
    return res;
}

void ItemTypeModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int last_id;
    int pos = -1;

    sql = "INSERT INTO item_type (name, priority) ";
    sql += "VALUES (:name, :priority)";
    query->prepare(sql);

    while(items_to_save.isEmpty() != true)
    {
        query->bindValue(":name",items_to_save.first()->getName());
        query->bindValue(":priority",items_to_save.first()->getPriority());
        query->exec();

        last_id = query->lastInsertId().toUInt();
        pos = findItemInPosition(items_to_save.first()->getPosition());
        items[pos]->setId(last_id);
        items_to_save.removeFirst();
    }
    delete query;
}
void ItemTypeModel::updateItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    ItemType *item_type = new ItemType();

    sql = "UPDATE item_type ";
    sql += "SET name = :name, priority = :priority ";
    sql += "WHERE id = :it_id";
    query->prepare(sql);

    while (items_to_update.isEmpty() != true)
    {
        item_type = items_to_update.first();
        query->bindValue(":it_id", item_type->getId());
        query->bindValue(":name", item_type->getName());
        query->bindValue(":priority", item_type->getPriority());
        query->exec();

        items_to_update.removeFirst();
    }
    delete item_type;
    delete query;
}

void ItemTypeModel::removeItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    int pos = -1;

    sql = "DELETE FROM item_type WHERE id = :id";
    query->prepare(sql);

    while(items_to_delete.isEmpty() != true)
    {
        query->bindValue(":id", items_to_delete.first()->getId());
        query->exec();

        items_to_delete.removeFirst();
    }
    delete query;
}

int ItemTypeModel::findItemInPosition(unsigned int pos)
{
    int i = -1;
    while (items[++i]->getPosition() != pos && i < items.size());

    return (items[i]->getPosition() == pos) ? i : -1;
}

void ItemTypeModel::setItemsToDelete(int *mass)
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
    for (int i=0; i < items.size(); i++)
    {
        items[i]->setPosition(i);
    }
    if (first >= 0)
        removeRows(first+1,count);
}

