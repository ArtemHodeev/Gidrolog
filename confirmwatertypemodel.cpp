#include "confirmwatertypemodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <databaseaccessor.h>

ConfirmWaterTypeModel::ConfirmWaterTypeModel(QObject *parent):
    WaterTypeModel(parent)
{
}
ConfirmWaterTypeModel::~ConfirmWaterTypeModel()
{

}

Qt::ItemFlags ConfirmWaterTypeModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled;
}

QVariant ConfirmWaterTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        case 1:
            res = QVariant("Назваие");
            break;
        default:
            break;
        }
    }
    else
    {
        res = QVariant(section + 1);
    }

    return res;
}

QVariant ConfirmWaterTypeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() != true || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    QVariant res;
    switch(index.column())
    {
    case 1:
        res = items[index.row()]->getName();
        break;
    default:
        break;
    }
    return res;
}

void ConfirmWaterTypeModel::setItems(QVector<WaterType*> new_items)
{
    for (int i = 0; i < new_items.size(); i ++)
    {
        items.append(new_items[i]);
    }
    rCount = new_items.size();
    cCount = 2;
}

void ConfirmWaterTypeModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql ="";
    sql = "INSERT INTO water_type (name) VALUES (:name)";

    query->prepare(sql);
    while (items.isEmpty()!= true)
    {
        query->bindValue(":name",items.first()->getName());
        query->exec();
        items.removeFirst();
    }
    delete query;
}
