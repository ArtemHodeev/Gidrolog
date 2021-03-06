#include "samplemodel.h"
#include <databaseaccessor.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <names.h>

SampleModel::SampleModel(QObject *parent) :
    TableModel(parent)
{

    setParams();
    setWaterTypes();
    setLocation();
    setAnaliticId();
}
SampleModel::~SampleModel()
{
//    delete params;
//    delete water_types;
//    delete locations;
}
QVariant SampleModel::data(const QModelIndex &index, int role) const
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
        // Вывод данных на экран
        QVariant res = QVariant("");
        unsigned int loc_id;
        QString loc_name = "";
        if (index.row() < rCount - 1 )
        {
            switch (index.column()) {
            case 0:
                res = QVariant(items.at(index.row())->getId());
                break;
            case 1:
                //            Место отбора
            {
                loc_id = items.at(index.row())->getLocationId();
                loc_name = Names::locations->key(loc_id);
                res = QVariant(loc_name);
                break;
            }
            case 2:
                //            Дата
            {
                res = QVariant(items.at(index.row())->getDate());
                break;
            }
            case 3:
                //            Тип водной массы
            {
                loc_id = items.at(index.row())->getWaterId();
                loc_name = Names::water_types->key(loc_id);
                res = QVariant(loc_name);
                break;
            }
            case 4:
                //            Комментарий
            {
                res = QVariant(items.at(index.row())->getComment());
                break;
            }
            default:
                //            Параметр
            {
                // Вывод значений параметров пробы
                QString param_name = headers[index.column()];
                unsigned int param_id = Names::params->value(param_name);
                double param_value = -1;

                if (items[index.row()]->getComponents()->contains(param_id) == true)
                {
                    param_value = items[index.row()]->getComponents()->value(param_id).getValue();
                }

                //если в пробе есть параметр с id равынм param_id,
                //то взять  и отобразить его значение, если нет, то отобразить Н/О
                res = param_value < 0 ? QVariant("Н/О") : QVariant(param_value);
                break;
            }
            }
        }

        return res;
    }
//    else if (role == Qt::CheckStateRole)
//    {
//        if (index.column() == 0)
//        {
//            return true;
//        }
//        else{
//        return QVariant();}
//    }
    else
    {
        return QVariant();
    }
}

QVariant SampleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    QVariant res;
    if (orientation == Qt::Horizontal)
    {
        res = QVariant(headers[section]);
    }
    else
    {
        res = (section < rCount - 1) ? QVariant(section + 1) : QVariant("+");
    }

    return res;
}
Qt::ItemFlags SampleModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool SampleModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || value == QVariant(""))
    {
        return false;
    }

    bool sign = false;
    bool main_info_changed = false;
    Sample *i;
    i = (index.row() < rCount - 1) ? items.at(index.row()) : new Sample();

    // Обновление информации
    switch(index.column())
    {
    case 1:
//        Место отбора
    {
        unsigned int loc_id = Names::locations->value(value.toString());
        i->setLocationId(loc_id);
        sign = true;
        main_info_changed = true;
        break;
    }
    case 2:
//        Дата
    {
//        QDateTime d = QDateTime::fromString(value.toString(),"yyyy-MM-dd hh:mm");

//        d = value.toDateTime();

        i->setDate(value.toDateTime());
        sign = true;
        main_info_changed = true;
        break;
    }
//        Тип водной массы
    case 3:
    {
        unsigned int loc_id = Names::water_types->value(value.toString());
        i->setWaterId(loc_id);
        sign = true;
        main_info_changed = true;
        break;
    }
//        Комментарий
    case 4:
    {
        if (index.model()->index(index.row(),1).data() == 0 ||
                index.model()->index(index.row(),2).data() == "" ||
                index.model()->index(index.row(),3).data() == 0)
        {
            QMessageBox::about(NULL,QString("Предупреждение"),QString("Укажите место взятия, дату отбора пробы и тип водной массы"));
            sign = false;

        }
        else
        {
            i->setComment(value.toString().trimmed());
            main_info_changed = true;
            sign = true;
        }
        break;
    }
        //Редактируются значения параметров пробы
    default:
    {
        if (index.model()->index(index.row(),1).data() == 0 ||
                 index.model()->index(index.row(),2).data() == "" ||
                 index.model()->index(index.row(),3).data() == 0)
        {
            QMessageBox::about(NULL,QString("Предупреждение"),QString("Укажите место взятия, дату отбора пробы и тип водной массы"));
            sign = false;
        }
        else
        {
            QString param_name = headers[index.column()]; // распознавание имени столбца - имя параметра
            unsigned int param_id = Names::params->value(param_name); // распознавание id параметра
            ItemInSample item_in_sample;

            //Если у пробы добавился параметр.
            if (index.data().toString().compare("Н/О") == 0 && value.toDouble() >= 0)
            {
                item_in_sample.setChanged(1);
            }
            //Если у параметра проб изменилось значение
            else if (index.data().toString().compare("Н/О") != 0 && value.toDouble() >= 0)
            {
                item_in_sample.setChanged(2);
            }
            //Если параметр удалалили из пробы (не определялся)
            else if(value.toDouble() < 0)
            {
                item_in_sample.setChanged(3);
            }

            item_in_sample.setValue(value.toDouble());
            item_in_sample.setItemId(param_id);
            item_in_sample.setSampleId(i->getId());

            items_in_sample_update.append(item_in_sample);

//            i->getComponents()->insert(param_id, item_in_sample);
//            i->setPosition(index.row());

//            //добавление редактируемой пробы/строки в список проб на обновление в БД
//            int pos = items_to_update.indexOf(i);
//            if (pos < 0)
//            {
//                items_to_update.append(i);
//            }
//            else
//            {
//                items_to_update[pos] = i;
//            }
        }

        break;
    } // default:

    }// switch (index.column())
    i->setPosition(index.row());

    //Проба создается, если заполенены место отбора, дата отбора и тип водной массы
//    if (sign == true )
//    {

        if (index.row() < rCount - 1 && main_info_changed == true)
        {
            items[index.row()] = i;
            if (items_to_update.contains(i) != true)
            {
                items_to_update.append(i);
            }
        }
        else if (index.row() == rCount - 1 && sign == true)
        {
            items.append(i);
            items_to_save.append(i);
            insertRows(rCount,1);
        }


//    }


    return true;
}

void SampleModel::setWaterTypes()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    QString name = "";
    unsigned int id;
    sql = "SELECT id, name FROM water_type";
    query->exec(sql);

    while(query->next())
    {
        id = query->value("id").toUInt();
        name = query->value("name").toString();
        Names::water_types->insert(name,id);
    }

    delete query;
}
void SampleModel::setLocation()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    QString name = "";
    unsigned int id;
    sql = "SELECT id, name FROM location";
    query->exec(sql);

    while(query->next())
    {
        id = query->value("id").toUInt();
        name = query->value("name").toString();
        Names::locations->insert(name,id);
    }

    delete query;
}
void SampleModel::setParams()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    QString name = "";
    unsigned int id;
    sql = "SELECT id, name, type_id FROM item WHERE display = true ORDER BY type_id";
    query->exec(sql);

    while(query->next())
    {
        id = query->value("id").toUInt();
        name = query->value("name").toString();
        Names::params->insert(name,id);
    }

    delete query;
}
void SampleModel::setAnaliticId()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    sql = "SELECT type_id FROM analitic_type LIMIT 1";
    query->exec(sql);
    if (query->first())
    {
        Names::analitic_id = query->value("type_id").toUInt();
    }
}
void SampleModel::deleteAllSamples()
{
    int *mass = new int[items.size() + 1];

    for (int i = 0; i < items.size(); i ++)
    {
        mass[i] = i;
    }
    mass[items.size()] = -1;
    setItemsToDelete(mass);
    delete mass;
}
void SampleModel::setSamples(QVector<Sample *> sample_mass)
{
//    deleteAllSamples();
    items.clear();
    rCount = 0;
    for (int i = 0; i < sample_mass.size(); i ++)
    {
        items.append(sample_mass[i]);
        items_to_save.append(sample_mass[i]);
//        items_to_update.append(sample_mass[i]);
        rCount ++;
    }

}
void SampleModel::setItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QSqlQuery *q = new QSqlQuery(DatabaseAccessor::getDb());
    int pos = 0;

    query->prepare("SELECT id, sample_set_id, location_id, water_type_id, sample_date, comment FROM sample WHERE sample_set_id = :sample_set");
    query->bindValue(":sample_set", Names::sample_set_id);
    query->exec();

    rCount = query->size() + 1;

    while (query->next())
    {
        Sample *s = new Sample();
        QHash<unsigned int, ItemInSample> *temp_params = new QHash<unsigned int, ItemInSample>();

        s->setId(query->value("id").toUInt());
        s->setLocationId(query->value("location_id").toUInt());
        s->setSampleSetId(query->value("sample_set_id").toUInt());
        s->setWaterId(query->value("water_type_id").toUInt());
        s->setDate(query->value("sample_date").toDateTime());
        s->setComment(query->value("comment").toString());
        s->setPosition(pos);

        q->prepare("SELECT item_id, value FROM item_sample WHERE sample_set_id = :set_id AND sample_id = :s_id");
        q->bindValue(":set_id", Names::sample_set_id);
        q->bindValue(":s_id",query->value("id"));
        q->exec();

        while(q->next())
        {
            ItemInSample item_in_sample;
            item_in_sample.setItemId(q->value("item_id").toUInt());
            item_in_sample.setValue(q->value("value").toDouble());

            temp_params->insert(q->value("item_id").toUInt(),item_in_sample);
        }
        pos ++;
        s->setComponents(temp_params);
        items.append(s);
    }
    delete query;
    delete q;
}

void SampleModel::resetModel()
{
    beginResetModel();
    setHeaders();
    endResetModel();
}
void SampleModel::resetModel(QVector<Sample *> sample_mass)
{
    beginResetModel();
    setHeaders();
    setWaterTypes();
    setLocation();
    setParams();
    setSamples(sample_mass);

    endResetModel();
}

void SampleModel::setHeaders()
{
    cCount = Names::params->size() + 5;

    headers.clear();
    if(headers.size() == 0)
    {
        headers.append(QString("id"));
        headers.append(QString("Место взятия"));
        headers.append(QString("Дата взятия"));
        headers.append(QString("Тип водной массы"));
        headers.append(QString("Комментарий"));
    }

    QHash<QString, unsigned int>::iterator i;

    for (i = Names::params->begin(); i != Names::params->end(); i ++)
    {
        headers.append(i.key());
    }
}

void SampleModel::updateItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    while (!items_to_update.empty())
    {
        query->prepare("UPDATE sample SET location_id = :location, water_type_id = :water_id, sample_date = :date, comment = :comment WHERE sample_set_id = :set_id AND id = :id ");
        query->bindValue(":set_id",Names::sample_set_id);
        query->bindValue(":id",items_to_update.first()->getId());
        query->bindValue(":location",items_to_update.first()->getLocationId());
        query->bindValue(":water_id", items_to_update.first()->getWaterId());
        query->bindValue(":date", items_to_update.first()->getDate().toString("yyyy-MM-dd hh:mm"));
        query->bindValue(":comment",items_to_update.first()->getComment());
        query->exec();
        items_to_update.removeFirst();
    }
//        QHash<unsigned int, ItemInSample>::iterator i;
//        unsigned int s_id = 0;
//        s_id = items_to_update.first()->getId();
        QString sql = "";

        QVector<ItemInSample>::iterator iter;

        for (iter = items_in_sample_update.begin(); iter != items_in_sample_update.end(); iter ++)
        {
            switch(iter->getChanged())
            {
            case 0:
               continue;
            case 1:
               sql = "INSERT INTO item_sample (sample_set_id, sample_id, item_id, value) VALUES (:set_id,:s_id, :i_id, :value)";
               break;
            case 2:
               sql = "UPDATE item_sample SET value = :value WHERE sample_set_id = :set_id AND sample_id = :s_id AND item_id = :i_id";
               break;
            case 3:
               sql = "DELETE FROM item_sample WHERE sample_set_id = :set_id AND sample_id = :s_id AND item_id = :i_id";
               break;
            default:
               break;
            }

            iter->setChanged(0);

            query->prepare(sql);
            query->bindValue(":set_id", Names::sample_set_id);
            query->bindValue(":s_id", iter->getSampleId());
            query->bindValue(":i_id",iter->getItemId());
            query->bindValue(":value",iter->getValue());
            query->exec();
        }//for (i = items_to_update.first()->getComponents()->begin(); i != items_to_update.first()->getComponents()->end(); i ++)
        items_in_sample_update.clear();
//        items_to_update.removeFirst();
//    }

    delete query;
}
void SampleModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QSqlQuery *query_1 = new QSqlQuery(DatabaseAccessor::getDb());

    QString sql = "";
    QString sql_items = "";
    int pos = -1;

    sql = "INSERT INTO sample (sample_set_id, location_id, sample_date, water_type_id, comment) ";
    sql += "VALUES (:sample_set_id,:location_id,:date,:water_id,:comment)";
    query->prepare(sql);

    sql_items = "INSERT INTO item_sample (sample_set_id, sample_id, item_id, value) VALUES (:set_id,:s_id, :i_id, :value)";
    query_1->prepare(sql_items);

    //Сохраняется информаци о пробе, без указания: какие параметры  в ней содержатся.
    //Параметры пробы могут быть сохранены только при обновлении пробы

    while (!items_to_save.empty())
    {
        query->bindValue(":sample_set_id",Names::sample_set_id);
        query->bindValue(":location_id",items_to_save.first()->getLocationId());
        query->bindValue(":water_id", items_to_save.first()->getWaterId());
        query->bindValue(":date", items_to_save.first()->getDate().toString("yyyy-MM-dd hh:mm"));
        query->bindValue(":comment", items_to_save.first()->getComment());
        query->exec();

        pos = findItemInPosition(items_to_save.first()->getPosition());
        items[pos]->setId(query->lastInsertId().toUInt());
        QHash<unsigned int, ItemInSample>::iterator i;
        unsigned int s_id = 0;
        s_id = query->lastInsertId().toUInt();


        for (i = items_to_save.first()->getComponents()->begin(); i != items_to_save.first()->getComponents()->end(); i ++)
        {
            query_1->bindValue(":set_id", Names::sample_set_id);
            query_1->bindValue(":s_id", s_id);
            query_1->bindValue(":i_id",i.value().getItemId());
            query_1->bindValue(":value",i.value().getValue());
            query_1->exec();
        }

//            iter->setChanged(0);



//        }//for (i = items_to_update.first()->getComponents()->begin(); i != items_to_update.first()->getComponents()->end(); i ++)
        items_to_save.removeFirst();
    }
    delete query;
}
void SampleModel::removeItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int s_id = 0;

    while(!items_to_delete.empty())
    {
        Sample *s = items_to_delete.first();
        QHash<unsigned int, ItemInSample>::iterator i;

        s_id = s->getId();
        sql = "DELETE FROM item_sample WHERE sample_set_id = :set_id AND sample_id = :s_id";
//        sql = "DELETE FROM item_sample WHERE sample_set_id = :set_id AND sample_id = :s_id AND item_id = :i_id";
        query->prepare(sql);

//        //удаление всех параметров пробы
//        for (i = s->getComponents()->begin(); i != s->getComponents()->end(); i ++)
//        {
        qDebug()<<"s_id: "<< s_id;
        qDebug()<<"set_id: "<<Names::sample_set_id;
        QString str = QString("DELETE FROM item_sample WHERE sample_set_id = %1 AND sample_id = %2").arg(Names::sample_set_id).arg(s_id);
        qDebug() << "query1: "<< str;

            query->bindValue(":set_id", Names::sample_set_id);
            query->bindValue(":s_id", s_id);
//            query->bindValue(":i_id", i.key());
            query->exec();
//        }
qDebug()<<"error on item_sample deleting: "<< query->lastError().text();
        // удаление пробы
        sql = "DELETE FROM sample WHERE sample_set_id = :set_id AND id = :s_id";
        str = QString("DELETE FROM sample WHERE sample_set_id = %1 AND id = %2").arg(Names::sample_set_id).arg(s_id);
                qDebug() << "query2: "<< str;
        query->prepare(sql);
        query->bindValue(":s_id", s_id);
        query->bindValue(":set_id", Names::sample_set_id);

        query->exec();
        qDebug()<<"error on sample deleting: "<< query->lastError().text();

        items_to_delete.removeFirst();
    }
    delete query;
}
void SampleModel::setItemsToDelete(int *mass)
{
    int count = 0;
    unsigned int first = mass[count];
    int index = -1;

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
    removeRows(first,count);
}
int SampleModel::findItemInPosition(int pos)
{
    int i = 0;
    while (i < items.size() && items[i]->getPosition() != pos)
    {
        i ++;
    }

    return (i<items.size()) ? i : -1;
}

QVector<Sample*> SampleModel::getSample()
{
    return items;
}
