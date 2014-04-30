#include "confirmlocationmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <databaseaccessor.h>
#include <QDebug>
#include <names.h>

ConfirmLocationModel::ConfirmLocationModel(QObject *parent) :
    LocationModel(parent)
{
}
ConfirmLocationModel::~ConfirmLocationModel()
{
}

void ConfirmLocationModel::setItems(QVector<Location *> new_items)
{
    rCount = new_items.size();
    cCount = 5;
    for (int i = 0; i < new_items.size(); i ++)
    {
        items.append(new_items[i]);
    }
}

QVariant ConfirmLocationModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            res = QVariant("Название");
            break;
        case 2:
            res = QVariant("Широта");
            break;
        case 3:
            res = QVariant("Долгота");
            break;
        case 4:
            res = QVariant("Глубина");
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

QVariant ConfirmLocationModel::data(const QModelIndex &index, int role) const
{
    QVariant res;

    if (index.isValid() != true || role != Qt::DisplayRole)
    {
        return res;
    }
    int row = index.row();
    switch (index.column())
    {
    case 1:
        res = items[row]->getName();
        break;
    case 2:
        res = items[row]->getWidth();
        break;
    case 3:
        res = items[row]->getLength();
        break;
    case 4:
        res = items[row]->getDeep();
        break;
    default:
        break;
    }
    return res;
}

bool ConfirmLocationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() != true || role != Qt::EditRole || value == QVariant(""))
    {
        return false;
    }

    int row = index.row();
    switch(index.column())
    {
    case 2:
        items[row]->setWidth(value.toFloat());
        break;
    case 3:
        items[row]->setLength(value.toFloat());
        break;
    case 4:
        items[row]->setDeep(value.toFloat());
        break;
    default:
        break;
    }
    return true;
}

Qt::ItemFlags ConfirmLocationModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags res = Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    if (index.column() == 0 || index.column() == 1)
    {
        res = Qt::ItemIsEnabled;
    }
    return res;
}

void ConfirmLocationModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

    sql = "INSERT INTO location (name, width, length, deep) ";
    sql += "VALUES (:name, :width, :length, :deep)";

    query->prepare(sql);

    while(items.isEmpty() != true)
    {
        query->bindValue(":name", items.first()->getName());
        query->bindValue(":width",items.first()->getWidth());
        query->bindValue(":length",items.first()->getLength());
        query->bindValue(":deep", items.first()->getDeep());
        query->exec();

        Names::locations->insert(items.first()->getName(),query->lastInsertId().toUInt());
        items.removeFirst();
    }
    delete query;
}
