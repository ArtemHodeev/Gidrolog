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

//    qDebug()<<"=========================";
    bool need = false;
    unsigned int pos = 0;
//    qDebug()<<"Other size: "<<other.size();
    for (int i = 0; i < other.size(); i ++)
    {
        // проверка процентов пропусков и ошибочных данных компонента на
        // приборную ошибку и количество пропусков
        if (other[i]->getLostCount() > lost_count || other[i]->getErrorCount() > error_count)
        {
            need = true;
        }
        // проверка на корреляцию. Для последнего нет смысла проверять,
        // сним уже все предыдущие проверили свою корреляцию
        if (i < other.size() - 1)
        {
            for (int j = 0; j < other[i]->getCorrelations().size(); j ++)
            {
                // корреляция компонентов превышает допустимую
                if (other[i]->getCorrelations()[j]->getCorell() > corell)
                {
                    need = true;
                }
            }
        }

        // если компонент не прошел какую-нибудь проверку,
        // то его нужно добавить в список компонентов, которые будут отображаться на экране
        if (need == true)
        {
            other[i]->setPosition(pos);
            items.append(other[i]);

            pos ++;
            need = false;
        }
    }
    rCount = items.size();
    cCount = 3;
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
//        qDebug()<<"mass[count]: "<< mass[count];
//        qDebug()<<"pos: "<< pos;
        if (pos != -1)
        {
            items_to_delete.append(items[pos]);
            items.removeAt(pos);
        }
        count ++;
    }
    removeRows(first,count,QModelIndex());
//    removeColumns(first,count,QModelIndex());
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
    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        unsigned int item_id = items[section]->getItemId();
        res = QVariant(Names::params->key(item_id));
    }
    else if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
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
            res = QVariant("Корреляция");
            break;
//        case 3:
//            res = QVariant("Итог");
//            break;
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
    unsigned int item_id = items[index.row()]->getItemId();

    int item_index = 0;
    item_index = findItemById(item_id);

    if (role == Qt::BackgroundColorRole)
    {
        QColor color;

        switch(index.column())
        {
        case 0:
            color = (items[item_index]->getLostCount() < lost_count) ? QColor(Qt::green) : QColor(Qt::red);
            break;
        case 1:
            color = (items[item_index]->getErrorCount() < error_count) ? QColor(Qt::green) : QColor(Qt::red);
            break;
        case 2:
        {
            QVector<ItemCorellation*> item_correl = items[item_index]->getCorrelations();
//            QString correl_str = "";
            bool hasFail = false;
            for (int i = 0; i < item_correl.size(); i ++)
            {
                if (item_correl[i]->getCorell() > corell)
                {
                    hasFail = true;
                }
            }
            color = (hasFail == false) ? QColor(Qt::green) : QColor(Qt::red);
            break;
        }
        default:
            color = Qt::white;
        }
        res = QVariant(color);
    }
    else if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            res = QVariant(items[item_index]->getLostCount());
            break;
        case 1:
            res = QVariant(items[item_index]->getErrorCount());
            break;
        case 2:
        {
            QVector<ItemCorellation*> item_correl = items[item_index]->getCorrelations();
            QString correl_str = "";
            for (int i = 0; i < item_correl.size(); i ++)
            {
                if (item_correl[i]->getCorell() > corell)
                {
                correl_str += QString("%1 (%2); ").arg(Names::params->key(item_correl[i]->getItemId()))
                                                  .arg(item_correl[i]->getCorell());
                }
            }
            res = QVariant(correl_str);
            break;
        }
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
        i++;
    }
    return (i < items.size()) ? i : -1;
}
void ConfirmCalculatorModel::backup()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QSqlQuery *query_backup = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    QString sql_backup = "";

    query_backup->exec("DELETE FROM item_sample_backupS WHERE id > 0");

    sql = "SELECT item_id, sample_id, value FROM item_sample";
    sql_backup = "INSERT INTO item_sample_backup (item_id, sample_id, value) ";
    sql_backup += "VALUE (:i_id, :s_id, :val)";
    query_backup->prepare(sql_backup);

    query->exec(sql);
    while (query->next())
    {
        query_backup->bindValue(":i_id", query->value("item_id").toUInt());
        query_backup->bindValue(":s_id", query->value("sample_id").toUInt());
        query_backup->bindValue(":val", query->value("value").toDouble());
        query_backup->exec();
    }

    delete query;
    delete query_backup;
}
