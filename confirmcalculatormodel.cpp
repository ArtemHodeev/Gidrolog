#include "confirmcalculatormodel.h"
#include <QSqlQuery>
#include <databaseaccessor.h>
#include <names.h>
#include <QDebug>
#include <QColor>

ConfirmCalculatorModel::ConfirmCalculatorModel(QObject *parent) :
    TableModel(parent)
{
//    params = new QHash<int, unsigned int>();
//    setParams();
    setFactors();
}
ConfirmCalculatorModel::~ConfirmCalculatorModel()
{
//    delete params;
}
//void ConfirmCalculatorModel::setParams()
//{
//    int ind = 0;
//    QHash<QString, unsigned int>::iterator i;
//    for(i = Names::params->begin(); i != Names::params->end(); i ++)
//    {
//        params->insert(ind,i.value());
//        ind ++;
//    }

//}

void ConfirmCalculatorModel::setItems()
{
    return;
}
void ConfirmCalculatorModel::setItems(QVector<ItemInfo *> other)
{
    rCount = 3;
//    qDebug()<<"=========================";
    unsigned int pos = 0;
//    qDebug()<<"Other size: "<<other.size();
    for (int i = 0; i < other.size(); i ++)
    {
        if (other[i]->getLostCount() > lost_count || other[i]->getErrorCount() > error_count)
        {
            other[i]->setPosition(pos);
            items.append(other[i]);
//            items.last()->setPosition(pos);
//            qDebug()<<"items: "<< Names::params->key(items)
            pos ++;
        }
    }
    showItems();
//    qDebug()<<"items size: "<<items.size();
    cCount = items.size();
}
void ConfirmCalculatorModel::showItems()
{
    for (int i = 0; i < items.size(); i ++)
    {
        qDebug()<<"item name: "<< Names::params->key(items[i]->getItemId());
        qDebug()<<"position: "<< items[i]->getPosition();
    }
}
void ConfirmCalculatorModel::setFactors()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    QString name = "";

    sql = "SELECT name, value FROM factor";
    query->exec(sql);

    while(query->next())
    {
        name = query->value("name").toString();
        if(name.compare("corell") == 0)
        {
            corell = query->value("value").toFloat();
        }
        else if(name.compare("lost_count") == 0)
        {
            lost_count = query->value("value").toFloat();
        }
        else if (name.compare("error_count") == 0)
        {
            error_count = query->value("value").toFloat();
        }
    }
    delete query;
}
void ConfirmCalculatorModel::setItemsToDelete(int *mass)
{
    int first = mass[0];
    int count = 0;
    int pos = 0;
    while (mass[count] != -1)
    {

        pos = findItemInPosition( mass[count]);
        qDebug()<<"mass[count]: "<< mass[count];
        qDebug()<<"pos: "<< pos;
        if (pos != -1)
        {
            items_to_delete.append(items[pos]);
            items.removeAt(pos);
        }
        count ++;
    }
    removeColumns(first,count,QModelIndex());
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

    sql = "UPDATE item SET display = false WHERE id = :id";
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
        unsigned int item_id = items[section]->getItemId();
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
int ConfirmCalculatorModel::findItemById(unsigned int &item_id) const
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
    if (index.isValid() != true)
    {
        return res;
    }
    else if (role == Qt::BackgroundColorRole)
    {
        QColor color;
        switch(index.row())
        {
        case 0:
            color = (items[index.column()]->getLostCount() < lost_count) ? QColor(Qt::green) : QColor(Qt::red);
            break;
        case 1:
            color = (items[index.column()]->getErrorCount() < error_count) ? QColor(Qt::green) : QColor(Qt::red);
            break;
        default:
            color = Qt::white;
        }
        res = QVariant(color);
    }
    else if(role == Qt::DisplayRole)
    {
        unsigned int item_id = items[index.column()]->getItemId();

        int item_index = 0;
        item_index = findItemById(item_id);

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
    }

    return res;
}
bool ConfirmCalculatorModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}
Qt::ItemFlags ConfirmCalculatorModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
int ConfirmCalculatorModel::findItemInPosition(unsigned int pos)
{
    int i = 0;
    while(i < items.size() && items[i]->getPosition() != pos)
    {
        qDebug()<<"pos: "<<pos;
        qDebug()<<"item.pos: "<< items[i]->getPosition();
        i++;
    }
    return (i < items.size()) ? i : -1;
}
