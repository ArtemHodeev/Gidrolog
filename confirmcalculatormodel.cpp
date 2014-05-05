#include "confirmcalculatormodel.h"
#include <QSqlQuery>
#include <databaseaccessor.h>
#include <names.h>
#include <QDebug>

ConfirmCalculatorModel::ConfirmCalculatorModel(QObject *parent) :
    TableModel(parent)
{
    params = new QHash<unsigned int, unsigned int>();
    setParams();
}
ConfirmCalculatorModel::~ConfirmCalculatorModel()
{
    delete params;
}
void ConfirmCalculatorModel::setParams()
{
    int ind = 0;
    QHash<QString, unsigned int>::iterator i;
    for(i = Names::params->begin(); i != Names::params->end(); i ++)
    {
        params->insert(ind,i.value());
        ind ++;
    }
}
void ConfirmCalculatorModel::setItems()
{
    return;
}
void ConfirmCalculatorModel::setItems(QVector<ItemInfo *> other)
{
    rCount = 4;
    cCount = other.size();
    for (int i = 0; i < other.size(); i ++)
    {
        items.append(other[i]);
    }
}
void ConfirmCalculatorModel::saveItems()
{
    return;
}
void ConfirmCalculatorModel::updateItems()
{
    return;
}
void ConfirmCalculatorModel::removeItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int  id = 0;

    sql = "UPDATE item SET display = true WHERE id = :id";
    query->prepare(sql);

    while(items_to_delete.isEmpty() != true)
    {
        id = items_to_delete.first()->getItemId();
        query->bindValue(":id",id);
        query->exec();
        QString item_name = Names::params->key(id);
        Names::params->remove(item_name);
        items_to_delete.removeFirst();
    }
}
QVariant ConfirmCalculatorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant res;
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        unsigned int item_id = params->value(section);
        res = QVariant(Names::params->key(item_id));
    }
    else if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            res = QVariant("Пропусков");
            break;
        case 1:
            res = QVariant("Ошибочных данных");
            break;
        case 2:
            res = QVariant("Корелляция");
            break;
        case 3:
            res = QVariant("Итог");
            break;
        default:
            break;
        }
    }
    return res;

}
int ConfirmCalculatorModel::findItemById(const unsigned int &item_id) const
{
    int i = 0;
    while (i < items.size() && items[i]->getItemId() != item_id)
    {
        i ++;
    }
    return (i == items.size()) ? -1 : i;
}
QVariant ConfirmCalculatorModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if (index.isValid() != true || role != Qt::DisplayRole)
    {
        return res;
    }

    unsigned int item_id = params->find(index.column()).value();
    int item_index = findItemById(item_id);

    switch(index.row())
    {
    case 0:
        res = QVariant(items[item_index]->getLostCount());
        break;
    case 1:
        res = QVariant(items[item_index]->getErrorCount());
        break;
    default:
        break;
    }

    return res;
}
bool ConfirmCalculatorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}
Qt::ItemFlags ConfirmCalculatorModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled;
}
