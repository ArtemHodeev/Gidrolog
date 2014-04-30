#include "importer.h"
#include <QDebug>
#include <xlsx/xlsxcellrange.h>
#include <xlsx/xlsxcell.h>
#include <QSqlQuery>
#include <QSqlError>
#include <databaseaccessor.h>
#include <item.h>
#include <watertype.h>
#include <names.h>
#include <QDialog>
#include <confirmimport.h>
#include <QTableView>
#include <itemmodel.h>
#include <confirmwatertypemodel.h>
#include <confirmitemmodel.h>

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
void Importer::confirm()
{
    ConfirmImport *c_i = new ConfirmImport();
    QDialog *dlg = new ConfirmImport();
    bool sign = false;

    if (unknow_params.isEmpty() != true)
    {
        QVector<Item*> p;
        QHash<QString, unsigned int>::iterator i;
        for ( i = unknow_params.begin(); i != unknow_params.end(); i ++)
        {
            Item *it = new Item();
            it->setName(i.key());
            p.append(it);
        }
        ConfirmItemModel *model = new ConfirmItemModel();
        model->setItems(p);
//        QTableView *view = new QTableView();
//        view->setModel(model);
        c_i->setParamModel(model);
        sign = true;
    }
    if (unknow_water_types.isEmpty() != true)
    {
        QVector<WaterType*> p;
        QHash<QString, unsigned int>::iterator i;
        for ( i = unknow_water_types.begin(); i != unknow_water_types.end(); i ++)
        {
            WaterType *wt = new WaterType();
            wt->setName(i.key());
            p.append(wt);
        }
        ConfirmWaterTypeModel *model = new ConfirmWaterTypeModel();
        model->setItems(p);
        c_i->setWaterModel(model);
    }
//    c_i->setModels();
    dlg = c_i;
    dlg->exec();
    delete dlg;
}
QVector<Sample*> Importer::import()
{
    checkHeaders();
    checkWaterTypes();
    checkLocations();
    confirm();
//    qDebug()<<"unknow_params size: "<<unknow_params.size();
    QVector<Sample*> s;// = new Sample()[file->dimension().rowCount()];

//       for (int i = 2; i <= file->dimension().rowCount(); i ++)
//       {
//           Sample *s1 = getSample(i);
//           s.append(s1);
//       }
//           Sample s1;// = getSample(2);
//       QHash<unsigned int, ItemInSample>::iterator param_iterator;
//       for(int i = 0; i < s.size(); i ++){
//           qDebug()<<"location: "<<s[i]->getLocationId();
//            qDebug()<<"date: "<<s[i]->getDate();
//             qDebug()<<"type: "<<s[i]->getWaterId();
//              qDebug()<<"comment: "<<s[i]->getComment();
//              qDebug()<<"components size: "<< s[i]->getComponents()->size();
//              qDebug()<<"==============================";
//       }

    return s;
}
Sample* Importer::getSample(int num)
{
    Sample *s = new Sample();
//    QXlsx::Cell cell;

    QString water_type_name;
    unsigned int water_id;
    QString location_name;
    unsigned int location_id;

    location_name = file->read(num,1).toString();
    location_id = locations->value(location_name);
    water_type_name = file->read(num,3).toString();
    water_id = water_types->value(water_type_name);

    s->setLocationId(location_id);
    s->setDate(file->read(num,2).toDate());
    s->setWaterId(water_id);
    s->setComment(file->read(num,4).toString());
    s->setPosition(num - 1);

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
void Importer::setWaterTypes(QHash<QString, unsigned int> *new_types)
{
    QHash<QString, unsigned int>::iterator i;
    qDebug()<<"Water_type size: "<< new_types->size();
    for (i = new_types->begin(); i != new_types->end(); i ++)
    {
        water_types->insert(i.key(),i.value());
    }
}

void Importer::setLocations(QHash<QString, unsigned int> *new_locations)
{
    QHash<QString, unsigned int>::iterator i;
    qDebug()<<"loactions size: "<< new_locations->size();
    for (i = new_locations->begin(); i != new_locations->end(); i ++)
    {
        locations->insert(i.key(),i.value());
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
        if (Names::params->contains(param_name) == false)
        {
            unknow_params.insert(param_name,i);
        }
        else
        {
            params_from_file.insert(i,Names::params->value(param_name));
        }
    }

    // Сохранение всех неизвестных параметров из файла.
    // В бующем должен быть выбор: какие параметры сохранить.
    // Должна быть отдельная функция.
    QHash<QString,unsigned int>::iterator iter;
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

//    sql = "INSERT INTO item (name, type_id) VALUES (:name, :type_id)";
//    query->prepare(sql);
    for (iter = unknow_params.begin(); iter != unknow_params.end(); iter ++ )
    {
//        query->bindValue(":name", iter.key());
//        query->bindValue(":type_id", 1);
//        query->exec();
//        qDebug()<<"last error: "<<query->lastError().text();
//        unsigned int last_id = query->lastInsertId().toUInt();
        Item *i = new Item();
        i->setName(iter.key());
//        params_from_file.insert(iter.value(),last_id);
//        params->insert(iter.key(), last_id);
//        Names::params->insert(iter.key(), last_id);


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

        if (Names::water_types->contains(water_type_name) != true)
        {
            unknow_water_types.insert(water_type_name,i);
        }

    }
    QHash<QString,unsigned int>::iterator iter;
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int last_id;
    sql = "INSERT INTO water_type (name) VALUES (:name)";
//    query->prepare(sql);
//    for (iter = unknow_water_types.begin(); iter != unknow_water_types.end(); iter ++)
//    {
//        query->bindValue(":name", iter.key());
//        query->exec();
//        last_id = query->lastInsertId().toUInt();
////        water_types->insert(iter.key(), last_id);
//        Names::water_types->insert(iter.key(), last_id);
//        qDebug()<<"names: "<<Names::water_types->value(iter.key());
//    }
}
void Importer::checkLocations()
{
    for (int i = 2; i <= file->dimension().rowCount(); i ++)
    {
        QString location_name = file->read(i,1).toString();

        if (Names::locations->contains(location_name) != true)
        {
            unknow_locations.insert(location_name,i);
        }

    }
    QHash<QString,unsigned int>::iterator iter;
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int last_id;
    sql = "INSERT INTO location (name) VALUES (:name)";
    query->prepare(sql);
    for (iter = unknow_locations.begin(); iter != unknow_locations.end(); iter ++)
    {
        query->bindValue(":name", iter.key());
        query->exec();
        last_id = query->lastInsertId().toUInt();
//        locations->insert(iter.key(), last_id);
        Names::locations->insert(iter.key(), last_id);
    }
}
