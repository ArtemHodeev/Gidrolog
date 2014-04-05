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
            res = QVariant(items.at(index.row())->getComment());
            break;

        default:

            QString param_name = headers[index.column()];

            unsigned int param_id = params->value(param_name);
            //если в пробе есть параметр с id равынм param_id, то взять отобразить его значение, если нет, то отбразить Н/О
            double param_value = -1;
            if (items[index.row()]->getComponents()->contains(param_id))
            {
                param_value = items[index.row()]->getComponents()->value(param_id);
            }

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
        res = QVariant(section + 1);
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

            qDebug()<<"Date: "<< d.toString("yyyy-MM-dd");
            items[index.row()]->setDate(d);

            break;
        }
        case 3:
        {
            items[index.row()]->setComment(value.toString());
            break;
        }
        default:
        {
            QString param_name = headers[index.column()];
            unsigned int param_id = params->value(param_name);
            items[index.row()]->getComponents()->insert(param_id, value != "" ? value.toDouble() : index.data().toDouble());
            break;
        }
        }

        //добавление редактируемой пробы/строки в список проб на обновление в БД
        if (!items_to_update.contains(items[index.row()]))
        {
            items_to_update.append(items[index.row()]);
        }
    }
    // Если создает новая проба
    else
    {
        qDebug()<< "Value: "<<value;
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
                if (index.model()->index(index.row(),1).data() == 0 || index.model()->index(index.row(),2).data() == "")
                {
                    QMessageBox::about(NULL,QString("Предупреждение"),QString("Укажите место взятия и дату отбора пробы"));
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
                if (index.model()->index(index.row(),1).data() == 0 || index.model()->index(index.row(),2).data() == "")
                {
                    QMessageBox::about(NULL,QString("Предупреждение"),QString("Укажите место взятия и дату отбора пробы"));
                    sign = false;
                }
                else
                {
                    sign = true;
                    QString param_name = headers[index.column()];
                    unsigned int param_id = params->value(param_name);
                    i->getComponents()->insert(param_id, value != "" ? value.toDouble() : index.data().toDouble());// = index.data().toDouble();
                }
                break;
            } // default:


            }// switch (index.column())

            //Если не заполнины место отбора и дата отбора, то проба не создается
            if (sign == true)
            {
                items.append(i);
                if (!items_to_save.contains(i))
                {
                    items_to_save.append(i);
                }
//                items_to_save.append(i);
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

    column_count_query->prepare("SELECT sample_id, count(item_id) c FROM gydro.item_sample GROUP BY sample_id ORDER BY c DESC LIMIT 1");
    column_count_query->exec();

    column_count_query->next();
    cCount = column_count_query->value("c").toInt() + query->record().count() -1;
    rCount = query->size() + 1;

    while (query->next())
    {
        Sample *s = new Sample();
        QHash<unsigned int, double> *temp_params = new QHash<unsigned int, double>();

        s->setId(query->value("id").toUInt());
        s->setLocationId(query->value("location_id").toUInt());
        s->setSampleSetId(query->value("sample_set_id").toUInt());
        s->setDate(query->value("sample_date").toDate());
        s->setComment(query->value("comment").toString());

        q->prepare("SELECT item_id, value FROM gydro.item_sample WHERE sample_id = :s_id");
        q->bindValue(":s_id",query->value("id"));
        q->exec();
        while(q->next())
        {
            temp_params->insert(q->value("item_id").toUInt(),q->value("value").toDouble());
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
        query->prepare("UPDATE gydro.sample SET location_id = :location, sample_date = :date, comment = :comment WHERE id = :id");

        query->bindValue(":id",items_to_update.first()->getId());
        query->bindValue(":location",items_to_update.first()->getLocationId());
        query->bindValue(":date", items_to_update.first()->getDate().toString("yyyy-MM-dd"));
        query->bindValue(":comment",items_to_update.first()->getComment());

        query->exec();
        qDebug()<<"Error on update sample: "<<query->lastError().text();

        query->prepare("UPDATE gydro.item_sample SET value = :value WHERE sample_id = :s_id AND item_id = :i_id");
        QHash<unsigned int, double>::iterator i;
        for (i = items_to_update.first()->getComponents()->begin(); i != items_to_update.first()->getComponents()->end(); i ++)
        {
            query->bindValue(":s_id",items_to_update.first()->getId());
            query->bindValue(":i_id",i.key());
            query->bindValue(":value",i.value());

            qDebug()<<"s_id: "<<items_to_update.first()->getId();
            qDebug()<<"i_id: "<<i.key();
            qDebug()<<"value: "<<i.value();


            query->exec();
            qDebug()<<"Error on update item_sample: "<<query->lastError().text();
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
    while (!items_to_save.empty())
    {
        sql = "INSERT INTO gydro.sample (sample_set_id, location_id, sample_date, comment) ";
        sql += "VALUES (:sample_set_id,:location_id,:date,:comment)";
        query->prepare(sql);
        query->bindValue(":sample_set_id",1);
        query->bindValue(":location_id",items_to_save.first()->getLocationId());
        query->bindValue(":date", items_to_save.first()->getDate().toString("yyyy-MM-dd"));
        query->bindValue(":comment", items_to_save.first()->getComment());
        query->exec();

        qDebug()<<"Error on insert sample: "<<query->lastError().text();
//        query->finish();
//        QSqlQuery *query1 = new QSqlQuery(DatabaseAccessor::getDb());
        sql = "INSERT INTO gydro.item_sample (sample_id, item_id, value) ";
        sql += "VALUES (:s_id, :i_id, :value)";

        query->prepare(sql);
        QHash<unsigned int, double>::iterator i;
        for (i = items_to_save.first()->getComponents()->begin(); i != items_to_save.first()->getComponents()->end(); i ++)
        {
            query->bindValue(":s_id",items_to_save.first()->getId());
            query->bindValue(":i_id",i.key());
            query->bindValue(":value",i.value());

            query->exec();
            qDebug()<<"Error on insert item_sample: "<<query->lastError().text();
//            query1->finish();
        }
//        delete query;

        items_to_save.removeFirst();
    }

}
void  SampleModel::on_actionSave_triggered()
{
    updateItems();
    saveItems();
}
