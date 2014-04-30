#include "confirmitemmodel.h"
#include<QSqlQuery>
#include <databaseaccessor.h>
#include <QDebug>
#include <names.h>
#include <QSqlError>

ConfirmItemModel::ConfirmItemModel(QObject *parent):
    ItemModel(parent)
{
}
ConfirmItemModel::~ConfirmItemModel()
{
}
QVariant ConfirmItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        case 2:
            res = QVariant("Тип компонента*");
            break;
        case 3:
            res = QVariant("Минимальное занчение");
            break;
        case 4:
            res = QVariant("Ошибка измерения");
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
QVariant ConfirmItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() /*|| role != Qt::DisplayRole*/)
    {
        return QVariant();
    }
    QVariant res;
    if (role == Qt::DisplayRole){

        switch(index.column())
        {
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
        default:
            break;
        }
    }


//    else if (role == Qt::CheckStateRole)
//    {
//        return (index.column() == 1) ? true : QVariant();
//    }
    return res;
}

Qt::ItemFlags ConfirmItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags res = QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    if (index.column() == 1)
    {
        res = Qt::ItemIsEnabled;
    }

    return res;
}
bool ConfirmItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || value == QVariant(""))
    {
        return false;
    }

    int row = index.row();

    switch(index.column())
    {
    case 1:
        items[row]->setName(value.toString());
        break;
    case 2:
        items[row]->setTypeId(value.toUInt());
        break;
    case 3:
        items[row]->setMinValue(value.toDouble());
        break;
    case 4:
        items[row]->setErrorLine(value.toDouble());
        break;
    };

    emit(on_itemChanged());
    return true;
}
void ConfirmItemModel::setItems(QVector<Item*> new_items)
{
    for (int i = 0; i < new_items.size(); i ++)
    {
        items.append(new_items[i]);
    }

    rCount = new_items.size();
    cCount = 5;
}
void ConfirmItemModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    QString sql = QString("INSERT INTO item (name, type_id, min_value, error_line)");
    sql += ("VALUES(:name,:type_id,:min_value,:error_line)");
    query->prepare(sql);

    while(!items.isEmpty())
    {
        query->bindValue(":name", items.first()->getName());
        query->bindValue(":type_id", items.first()->getTypeId());
        query->bindValue(":min_value", items.first()->getMinValue());
        query->bindValue(":error_line",items.first()->getErrorLine());
        query->exec();
        qDebug()<<"Error on save item: "<< query->lastError().text();
        Names::params->insert(items.first()->getName(),query->lastInsertId().toUInt());
        items.removeFirst();
    }
    delete query;
}
bool ConfirmItemModel::hasEmptyType()
{
    bool sign = false;
    for (int i = 0; i < rCount; i ++)
    {
        if (items[i]->getTypeId() == 0)
        {
            sign = true;
            break;
        }
    }
    return sign;
}

