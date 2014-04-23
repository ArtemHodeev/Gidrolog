#include "importer.h"
#include <QDebug>
#include <xlsx/xlsxcellrange.h>
#include <xlsx/xlsxcell.h>
#include <QSqlQuery>
#include <QSqlError>
#include <databaseaccessor.h>

Importer::Importer()
{
    params = new QHash<QString, unsigned int>();
    locations = new QHash<QString, unsigned int>();
    water_types = new QHash<QString, unsigned int>();
    file = new QXlsx::Document();
}

Importer::Importer(QString file_path)
{
    params = new QHash<QString, unsigned int>();
    locations = new QHash<QString, unsigned int>();
    water_types = new QHash<QString, unsigned int>();
    file = new QXlsx::Document(file_path);
}
Importer::~Importer()
{
    delete params;
    delete locations;
    delete water_types;
}
QVector<Sample*> Importer::import()
{
    checkHeaders();
//    qDebug()<<"unknow_params size: "<<unknow_params.size();
    QVector<Sample*> s;// = new Sample()[file->dimension().rowCount()];
//    Sample s1;// = getSample(2);
//    qDebug()<<"location: "<<s1.getLocationId();
//     qDebug()<<"date: "<<s1.getDate();
//      qDebug()<<"type: "<<s1.getWaterId();
//       qDebug()<<"comment: "<<s1.getComment();
//       qDebug()<<"components size: "<< s1.getComponents()->size();
//       QHash<unsigned int, ItemInSample>::iterator param_iterator;
//       for (param_iterator = s1.getComponents()->begin(); param_iterator != s1.getComponents()->end(); param_iterator++)
//       {
//           qDebug()<<"item_id: "<<param_iterator.value().getItemId();
//           qDebug()<<"value: "<<param_iterator.value().getValue();
//       }
       for (int i = 2; i <= file->dimension().rowCount(); i ++)
       {
           Sample *s1 = getSample(i);
           s.append(s1);
       }
//           Sample s1;// = getSample(2);
       QHash<unsigned int, ItemInSample>::iterator param_iterator;
       for(int i = 0; i < s.size(); i ++){
           qDebug()<<"location: "<<s[i]->getLocationId();
            qDebug()<<"date: "<<s[i]->getDate();
             qDebug()<<"type: "<<s[i]->getWaterId();
              qDebug()<<"comment: "<<s[i]->getComment();
              qDebug()<<"components size: "<< s[i]->getComponents()->size();
              qDebug()<<"==============================";
       }

    return s;
}
Sample* Importer::getSample(int num)
{
    Sample *s = new Sample();
//    QXlsx::Cell cell;

    s->setLocationId(file->read(num,1).toUInt());
    s->setDate(file->read(num,2).toDate());
    s->setWaterId(file->read(num,3).toUInt());
    s->setComment(file->read(num,4).toString());

    QHash<unsigned int, ItemInSample> *components = new  QHash<unsigned int, ItemInSample>();
    QHash<unsigned int, unsigned int>::iterator param_iterator; // первый аргумент - номер параметра из файла; второй - id параметра в БД

    for (int i = 5; i <= file->dimension().columnCount(); i ++)
    {
        ItemInSample item_in_sample;
        param_iterator = params_from_file.find(i);
        item_in_sample.setItemId(param_iterator.value());
        item_in_sample.setValue(file->read(num,i).toDouble());

        components->insert(param_iterator.value(), item_in_sample);
    }

    s->setComponents(components);
    return s;
}
void Importer::setParams(QHash<QString,unsigned int> *new_params)
{
//    qDebud
    QHash<QString, unsigned int>::iterator i;
    for (i = new_params->begin(); i != new_params->end(); i ++)
    {
//        qDebug()<<"key: "<< i.key() <<"  value: " << i.value();
        params->insert(i.key(),i.value());
    }
}
void Importer::checkHeaders()
{
    // Сравнение всех заголовков из файла с заголовками системы
    for (int i = 5; i <= file->dimension().columnCount(); i ++)
    {
        QString param_name = file->read(1,i).toString();
//        qDebug()<<"param_name: "<< param_name;
//        qDebug()<<"Length: "<<param_name.length();
        if (params->contains(param_name) == false)
        {
            unknow_params.insert(param_name,i);
        }
        else
        {
            params_from_file.insert(i,params->value(param_name));
        }
    }

    // Сохранение всех неизвестных параметров из файла.
    // В бующем должен быть выбор: какие параметры сохранить.
    // Должна быть отдельная функция.
    QHash<QString,unsigned int>::iterator iter;
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

    sql = "INSERT INTO item (name, type_id) VALUES (:name, :type_id)";
    query->prepare(sql);
    for (iter = unknow_params.begin(); iter != unknow_params.end(); iter ++ )
    {
        query->bindValue(":name", iter.key());
        query->bindValue(":type_id", 1);
        query->exec();
        qDebug()<<"last error: "<<query->lastError().text();
        unsigned int last_id = query->lastInsertId().toUInt();

        params_from_file.insert(iter.value(),last_id);
        params->insert(iter.key(), last_id);

    }
//    qDebug()<<"Params size in inporter: "<<params->size();
//    QHash<unsigned int, unsigned int>::iterator it;
//    for (it = params_from_file.begin(); it != params_from_file.end(); it ++ )
//    {
//        qDebug()<<"Number: "<<it.key()<<"| Id: "<<it.value();
//    }
}
void Importer::checkWaterTypes()
{
    for (int i = 2; i <= file->dimension().rowCount(); i ++)
    {
        QString water_type_name = file->read(i,3).toString();

        if (water_types->contains(water_type_name) != true)
        {
            unknow_water_types.insert(water_type_name,i);
        }

    }
//    QHash<QString,unsigned int>::iterator iter;
//    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
//    QString sql = "";

//    sql = "INSERT INTO water_type (name) VALUES (:name)";
//    query->prepare(sql);
//    for (iter = unknow_water_types.begin(); iter != unknow_water_types.end(); iter ++)
//    {
//        query->bindValue(":name", iter.key());
//        query->exec();
//        water_types->insert(iter.key(), query->lastInsertId());
//    }
}
