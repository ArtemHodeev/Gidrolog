#include "watertypemodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <databaseaccessor.h>

WaterTypeModel::WaterTypeModel(QObject *parent):TableModel(parent)
{
}

WaterTypeModel::~WaterTypeModel()
{
}

QVariant WaterTypeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() != true || role != Qt::DisplayRole || index.row() == rCount - 1 )
    {
        return QVariant();
    }

    QVariant res;
    switch (index.column()) {
    case 0:
        res = QVariant(items[index.row()]->getId());
        break;
    case 1:
        res = QVariant(items[index.row()]->getName());
        break;
    default:
        break;
    }

    return res;
}

QVariant WaterTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res;

    if (role != Qt::DisplayRole)
    {
        return res;
    }

    if (orientation == Qt::Vertical)
    {
        res = (section < rCount - 1) ? QVariant(section + 1) : QVariant("+");
    }

    else
    {
        switch(section)
        {
        case 0:
            res = QVariant("Id");
            break;
        case 1:
            res = QVariant("Название");
            break;
        default:
            break;
        }
    }
    return res;
}

bool WaterTypeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() != true || role != Qt::EditRole || value == QVariant(""))
    {
        return false;
    }

    WaterType *wt;
    wt = (index.row() < rCount - 1) ? items[index.row()] : new WaterType();
    qDebug()<<"index: "<<index.row();
    qDebug()<<"id: "<<wt->getId();
    qDebug()<<"name: "<<wt->getName();

    switch (index.column()) {
    case 1:
        wt->setName(value.toString());
        break;
    default:
        break;
    }

//    items.insert(index.row(), wt);

    if (index.row() < rCount - 1)
    {
        items[index.row()] = wt;
        if (items_to_update.contains(wt) != true)
        {
            qDebug()<<"add to update";
            items_to_update.append(wt);
        }
    }
    else
    {
        wt->setPosition(index.row());
        items.append(wt);
        items_to_save.append(wt);
        insertRows(rCount,1);
    }

    return true;
}

Qt::ItemFlags WaterTypeModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEditable|QAbstractTableModel::flags(index);
}

void WaterTypeModel::setItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

    sql = "SELECT id, name FROM water_type";
    query->prepare(sql);
    query->exec();
    rCount = query->size() + 1;
    cCount = query->record().count();
    qDebug()<<"Error on retriving water_type: "<<query->lastError().text();
    int pos = 0;
    while (query->next())
    {
        WaterType *wt = new WaterType();
        wt->setId(query->value("id").toUInt());
        wt->setName(query->value("name").toString());
        wt->setPosition(pos);
        pos ++;

        items.append(wt);
    }
    qDebug()<<"Items size: "<<items.size();
    delete query;
}

void WaterTypeModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    sql = "INSERT INTO water_type(name) VALUES (:name)";
    query->prepare(sql);
    qDebug()<<"Items_to_save size: "<<items_to_save.size();
    while(items_to_save.isEmpty() != true)
    {
        qDebug()<<"Save";
        query->bindValue(":name", items_to_save.first()->getName());
        query->exec();
        qDebug()<<"Error on saving items: "<<query->lastError().text();

        items_to_save.removeFirst();
    }
    delete query;
}

void WaterTypeModel::updateItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int wt_id;
    sql = "UPDATE water_type SET name = :name WHERE id = :wt_id";
    query->prepare(sql);
    qDebug()<<"Items_to_update size: "<<items_to_update.size();
    while (items_to_update.empty() != true)
    {
        if (items_to_update.first()->getId() != 0)
        {
            wt_id = items_to_update.first()->getId();
        }
        else
        {
            QSqlQuery *q = new QSqlQuery(DatabaseAccessor::getDb());
            q->exec("SELECT id FROM water_type");
            q->seek(items_to_update.first()->getPosition());
            wt_id = q->value("id").toUInt();
            delete q;
        }

        query->bindValue(":wt_id", wt_id);
        query->bindValue(":name", items_to_update.first()->getName());
        query->exec();

        items_to_update.removeFirst();
    }
    delete query;
}

void WaterTypeModel::removeItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int wt_id;
    sql = "DELETE FROM water_type WHERE id = :wt_id";
    query->prepare(sql);
     qDebug()<<"Items_to_delete size: "<<items_to_delete.size();
    while (items_to_delete.empty() != true)
    {
        if (items_to_delete.first()->getId() != 0)
        {
            wt_id = items_to_delete.first()->getId();
            query->bindValue(":wt_id", wt_id);
            query->exec();
        }
//        else
//        {
//            QSqlQuery *q = new QSqlQuery(DatabaseAccessor::getDb());
//            q->exec("SELECT id FROM water_type");
//            q->seek(items_to_delete.first()->getPosition());
//            wt_id = q->value("id").toUInt();
//            delete q;
//        }

//        query->bindValue(":wt_id", wt_id);
//        query->exec();
//        qDebug()<<"Error on deleting: "<<query->lastError().text();

        items_to_delete.removeFirst();
    }
    delete query;
}

/*
 *Функция удаляет элемент из items и сокращает его размер, а указание: какие строки удалять не меняется
 * т.е. нужно удалить строки 7 и 8, а всего строк (items.size()) 8. после удаления 7ой строки вектро items
 * сократится и в нем уже не будет строки 8. Решение проблеммы: замена QVector на QHash или QMap, либо каждый раз
 * проверять для каждого элемента items поле position.
 *

*/
void WaterTypeModel::setItemsToDelete(unsigned int *mass)
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
    qDebug()<<"Items_to_delete size: "<<items_to_delete.size();
    removeRows(first,count);
}

int WaterTypeModel::findItemInPosition(unsigned int pos)
{
    int i = -1;
    while (items[++i]->getPosition() != pos && i < items.size());

    return (items[i]->getPosition() == pos) ? i : -1;
}
