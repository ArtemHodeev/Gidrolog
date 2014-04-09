#include "samplemodel.h"
#include <databaseaccessor.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
SampleModel::SampleModel(QObject *parent) :
    TableModel(parent)
{
    params = new QHash<QString, unsigned int>();
}
SampleModel::~SampleModel()
{
    delete params;
}
QVariant SampleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }
    QVariant res = QVariant("");
    if (index.row() < rCount - 1 )
    {
        switch (index.column()) {
        case 0:
            res = QVariant(items.at(index.row())->getId());
            break;
        case 1:
            res = QVariant(items.at(index.row())->getLocationId());
            break;
        case 2:
            res = QVariant(items.at(index.row())->getDate());
            break;
        case 3:
            res = QVariant(items.at(index.row())->getWaterId());
            break;
        case 4:
            res = QVariant(items.at(index.row())->getComment());
            break;

        default:

            QString param_name = headers[index.column()];

            unsigned int param_id = params->value(param_name);
            //если в пробе есть параметр с id равынм param_id, то взять  и отобразить его значение, если нет, то отобразить Н/О
            double param_value = -1;

            if (items[index.row()]->getComponents()->contains(param_id) == true)
            {
                param_value = items[index.row()]->getComponents()->value(param_id).getValue();
            }
//            qDebug()<<"param_value for s_id = "<<items[index.row()]<< " : "<<param_value;
            res = param_value < 0 ? QVariant("Н/О") : QVariant(param_value);
            break;
        }
    }

    return res;
}
int SampleModel::rowCount(const QModelIndex &parent) const
{
    return rCount;
}
int SampleModel::columnCount(const QModelIndex &parent) const
{
    return cCount;
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
        if (section < rCount - 1)
        {
            res = QVariant(section + 1);
        }
        else
        {
            res = QVariant("+");
        }
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
    if (!index.isValid() || role != Qt::EditRole)
    {
        return false;
    }

    //если режактируется существующая проба
    if (index.row() < rCount - 1)
    {
        switch(index.column())
        {
        case 1:
        {
            items[index.row()]->setLocationId(value != "" ? value.toUInt() : index.data().toUInt());
            break;
        }
        case 2:
        {
           //узнаем: была ли введена новая дата или нужно оставить старую
            QVariant v = value != "" ? value.toString() : index.data().toString();
            QDate d = QDate::fromString(v.toString(),"yyyy-MM-dd");

            items[index.row()]->setDate(d);
            break;
        }
        case 3:
        {
            items[index.row()]->setWaterId(value.toUInt());
            break;
        }
        case 4:
        {
            items[index.row()]->setComment(value.toString());
            break;
        }
        default:
        {
            QString param_name = headers[index.column()]; // распознавание имени столбца - имя параметра
            unsigned int param_id = params->value(param_name); // распознавание id параметра

            ItemInSample item_in_sample;
            if (value != "")
            {
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
                items[index.row()]->getComponents()->insert(param_id, item_in_sample);
                items[index.row()]->setPosition(index.row());

                //добавление редактируемой пробы/строки в список проб на обновление в БД
                int pos = items_to_update.indexOf(items[index.row()]);
                if (pos < 0)
                {
                    items_to_update.append(items[index.row()]);
                }
                else
                {
                    items_to_update[pos] = items[index.row()];
                }
            }// if (value != "")
            else
            {
                if (index.data().toString().compare("Н/О") == 0)
                {
                    item_in_sample.setValue(-1);
                }
                else
                {
                    item_in_sample.setValue(index.data().toDouble());
                }
            }
            break;
        }//defualt
        }//switch(index.column())
    }//if (index.row() < rCount - 1)

    // Если создается новая проба
    else
    {
        if (value != "")
        {
            bool sign = true;
            Sample *i = new Sample();

            switch(index.column())
            {
            case 1:
            {
                i->setLocationId(value.toUInt());
                sign = true;

                break;
            } // case 1:
            case 2:
            {
                QDate d = QDate::fromString(value.toString(),"yyyy-MM-dd");

                i->setDate(d);
                sign = true;

                 break;
            }// case 2:
            case 3:
            {
                i->setWaterId(value.toUInt());
                sign = true;

                break;
            }
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
                    i->setComment(value.toString());
                    sign = true;
                }
                break;
            } // case 3:
            default:
            {
                QMessageBox::about(NULL,QString("Предупреждение"),QString("Укажите место взятия и дату отбора пробы"));
                sign = false;

                break;
            } // default:

            }// switch (index.column())

            //Проба создается, если заполенены место отбора, дата отбора и тип водной массы
            if (sign == true)
            {
                items.append(i);
                qDebug()<<"Contains: "<<items_to_save.contains(i);
                if (!items_to_save.contains(i))
                {
                    items_to_save.append(i);
                }
                insertRows(rCount,1);
            }
        }
    }
    return true;
}

unsigned int SampleModel::getVaule(QString param_name) const
{
    return params->value(param_name);

}
void SampleModel::setItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QSqlQuery *q = new QSqlQuery(DatabaseAccessor::getDb());
    QSqlQuery *column_count_query = new QSqlQuery(DatabaseAccessor::getDb());

    query->prepare("SELECT id, sample_set_id, location_id, sample_date, comment FROM gydro.sample");
    query->exec();

//    column_count_query->prepare("SELECT sample_id, count(item_id) c FROM gydro.item_sample GROUP BY sample_id ORDER BY c DESC LIMIT 1");
    column_count_query->prepare("SELECT count(id)c FROM gydro.item");
    column_count_query->exec();

    column_count_query->next();
    cCount = column_count_query->value("c").toInt() + query->record().count() -1;
    rCount = query->size() + 1;

    while (query->next())
    {
        Sample *s = new Sample();
        QHash<unsigned int, ItemInSample> *temp_params = new QHash<unsigned int, ItemInSample>();

        s->setId(query->value("id").toUInt());
        s->setLocationId(query->value("location_id").toUInt());
        s->setSampleSetId(query->value("sample_set_id").toUInt());
        s->setWaterId(query->value("water_type_id").toUInt());
        s->setDate(query->value("sample_date").toDate());
        s->setComment(query->value("comment").toString());

        q->prepare("SELECT item_id, value FROM gydro.item_sample WHERE sample_id = :s_id");
        q->bindValue(":s_id",query->value("id"));
        q->exec();

        while(q->next())
        {
            ItemInSample item_in_sample;
            item_in_sample.setItemId(q->value("item_id").toUInt());
            item_in_sample.setValue(q->value("value").toDouble());

            temp_params->insert(q->value("item_id").toUInt(),item_in_sample);
        }

        s->setComponents(temp_params);
        items.append(s);
    }
    delete query;
    delete q;
}
void SampleModel::setHeaders()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    query->prepare("SELECT id, name, type_id FROM gydro.item ORDER BY type_id");
    query->exec();

    headers.append(QString("id"));
    headers.append(QString("Место взятия"));
    headers.append(QString("Дата взятия"));
    headers.append(QString("Тип водной массы"));
    headers.append(QString("Комментарий"));

    QString name = QString("");
    while (query->next())
    {
        name = query->value("name").toString().trimmed();
        headers.append(name);
        params->insert(name, query->value("id").toUInt());
    }
}
void SampleModel::updateItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    qDebug()<<"Size of items_to_update: "<<items_to_update.size();
    while (!items_to_update.empty())
    {
        query->prepare("UPDATE gydro.sample SET location_id = :location, water_type_id = :water_id, sample_date = :date, comment = :comment WHERE id = :id");

        query->bindValue(":id",items_to_update.first()->getId());
        query->bindValue(":location",items_to_update.first()->getLocationId());
        query->bindValue(":water_id", items_to_update.first()->getWaterId());
        query->bindValue(":date", items_to_update.first()->getDate().toString("yyyy-MM-dd"));
        query->bindValue(":comment",items_to_update.first()->getComment());

        query->exec();

        QHash<unsigned int, ItemInSample>::iterator i;
        unsigned int s_id = 0;
        QString sql = "";

        for (i = items_to_update.first()->getComponents()->begin(); i != items_to_update.first()->getComponents()->end(); i ++)
        {
            // Если проба новая, т.е. системе не известен ее id из БД, то найти эту пробу.
            if (items_to_update.first()->getId() == 0)
            {
                QSqlQuery *sample_id_query = new QSqlQuery(DatabaseAccessor::getDb());
                sql = "INSERT INTO gydro.item_sample (sample_id, item_id, value) VALUES (:s_id, :i_id, :value)";

                sample_id_query->prepare("SELECT id FROM gydro.sample");
                sample_id_query->exec();
                sample_id_query->seek(items_to_update.first()->getPosition());

                s_id = sample_id_query->value("id").toUInt();
                delete sample_id_query;
            }
            else
            {
//                qDebug()<<"Changed: "<<i->getChanged();
                switch(i->getChanged())
                {
                case 1:
                    sql = "INSERT INTO gydro.item_sample (sample_id, item_id, value) VALUES (:s_id, :i_id, :value)";
                    break;
                case 2:
                    sql = "UPDATE gydro.item_sample SET (value = :value WHERE sample_id = :s_id AND item_id = :i_id)";
                    break;
                case 3:
                    sql = "DELETE FROM gydro.item_sample WHERE sample_id = :s_id AND item_id = :i_id";
                    break;
                default:
                    break;
                }

                s_id = items_to_update.first()->getId();
            }

            query->prepare(sql);

            query->bindValue(":s_id",s_id);
            query->bindValue(":i_id",i.key());
            query->bindValue(":value",i.value().getValue());

            query->exec();
        }

        items_to_update.removeFirst();
    }

    delete query;
}
void SampleModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    QString sql = "";
    qDebug()<<"Size of items_to_save: "<<items_to_save.size();

    //Сохраняется информаци о пробе, без указания: какие параметры  в ней содержатся.
    //Параметры пробы могут быть сохранены только при обновлении пробы
    while (!items_to_save.empty())
    {
        sql = "INSERT INTO gydro.sample (sample_set_id, location_id, sample_date, water_type_id, comment) ";
        sql += "VALUES (:sample_set_id,:location_id,:date,:water_id,:comment)";
        query->prepare(sql);

        query->bindValue(":sample_set_id",1);
        query->bindValue(":location_id",items_to_save.first()->getLocationId());
        query->bindValue(":water_id", items_to_save.first()->getWaterId());
        query->bindValue(":date", items_to_save.first()->getDate().toString("yyyy-MM-dd"));
        query->bindValue(":comment", items_to_save.first()->getComment());
        query->exec();
        items_to_save.removeFirst();
    }

}
void SampleModel::removeItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    qDebug()<<"Size of items_to_delete: "<<items_to_delete.size();
    while(!items_to_delete.empty())
    {
        Sample *s = items_to_delete.first();
        sql = "DELETE FROM gydro.item_sample WHERE sample_id = :s_id AND item_id = :i_id";
        query->prepare(sql);

        QHash<unsigned int, ItemInSample>::iterator i;
        unsigned int s_id = 0;
        for (i = s->getComponents()->begin(); i != s->getComponents()->end(); i ++)
        {
            // Определение id удаляемой пробы
            // Если проба была недавно добавлена в системе и сохранена,
            // то в БД у нее появляется id, но система о нем еще не знает
            if (s->getId() == 0)
            {
                QSqlQuery *q = new QSqlQuery(DatabaseAccessor::getDb());
                q->prepare("Select id FROM gydro.sample");
                q->exec();
                q->seek(s->getPosition());
                s_id = q->value("id").toUInt();
            }
            else
            {
                s_id = s->getId();
            }
            query->bindValue(":s_id", s_id);
            query->bindValue(":i_id", i.key());
            query->exec();
        }

        sql = "DELETE FROM gydro.sample WHERE id = :s_id";
        query->prepare(sql);
        query->bindValue(":s_id", s_id);
        query->exec();

        items_to_delete.removeFirst();
    }
    delete query;
}
void SampleModel::setItemsToDelete(int *mass)
{
    int count = 0;
    int first = mass[count];

    while(mass[count] != -1)
    {
        items_to_delete.append(items[mass[count]]);
        int update_index = items_to_update.indexOf(items[mass[count]]);
        int save_index = items_to_save.indexOf(items[mass[count]]);

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

        items.removeAt(mass[count]);
        count ++;
    }
    removeRows(first,count);
}
void  SampleModel::on_actionSave_triggered()
{
    saveItems();
    updateItems();
    removeItems();
}
