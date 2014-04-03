#include "samplemodel.h"
#include <databaseaccessor.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QRegExp>
SampleModel::SampleModel(QObject *parent) :
    TableModel(parent)
{
    params = new QHash<QString, unsigned int>();
//    headers = new QHash<unsigned int, QString>();
}
SampleModel::~SampleModel()
{
    delete params;
//    delete headers;
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

            double param_value = items[index.row()]->getComponents()->value(param_id);
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
    if (index.row() < rCount - 1)
    {
        qDebug()<<"column: "<<index.column();
        switch(index.column())
        {
        case 1:
            items[index.row()]->setLocationId(value != "" ? value.toUInt() : index.data().toUInt());
            break;
        case 2:
        {
            QRegExp r("\\d{1,2}\,|\.\\d{1,2}\,|\.\\d{4}");
            QDate d;
            qDebug()<<"ExactMatch: "<<r.exactMatch(value.toString());
            QVariant v = value;

            qDebug()<<"Value: " << v.toString();
            if (value != "" )
            {

                d = QDate::fromString(value.toString(),"dd.mm.yyyy");
            }
            else
            {
                d = QDate::fromString(index.data().toString(),"dd/mm/yyyy");
            }
            qDebug()<<"Is Valid: "<<d.isValid();
            qDebug()<<"Is NULL: "<<d.isNull();
            items[index.row()]->setDate(d);
            break;
        }
        case 3:
            items[index.row()]->setComment(value.toString());
            break;
        default:
            QString param_name = headers[index.column()];
            unsigned int param_id = params->value(param_name);
            items[index.row()]->getComponents()->insert(param_id, value != "" ? value.toDouble() : index.data().toDouble());// = index.data().toDouble();
            //        items[index.row()]->getComponents()->value(param_id) = value != "" ? value.toDouble() : index.data().toDouble();
            break;
        }
        if (!items_to_save.contains(items[index.row()]))
        {
            items_to_save.append(items[index.row()]);
        }
    }
    else
    {
        if (value != "")
        {
            bool sign = true;
            Sample *i = new Sample();
            switch(index.column())
            {
            case 1:
                i->setLocationId(value != "" ? value.toUInt() : index.data().toUInt());
                break;
            case 2:
                i->setDate(value != "" ? value.toDate() : index.data().toDate());
                break;
            case 3:
                i->setComment(value.toString());
                break;
            default:
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
            }
            if (sign)
            {
                items.append(i);
                items_to_save.append(i);
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
void SampleModel::saveItems()
{

}
