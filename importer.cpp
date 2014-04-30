#include "importer.h"
#include <QDebug>
#include <xlsx/xlsxcellrange.h>
#include <xlsx/xlsxcell.h>
#include <QSqlQuery>
#include <QSqlError>
#include <databaseaccessor.h>
#include <item.h>
#include <watertype.h>
#include <location.h>
#include <names.h>
#include <QDialog>
#include <confirmimport.h>
#include <QTableView>
#include <itemmodel.h>
#include <confirmwatertypemodel.h>
#include <confirmitemmodel.h>
#include <confirmlocationmodel.h>

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
bool Importer::confirm()
{
    QDialog *dlg = new ConfirmImport();
    ConfirmImport *c_i = new ConfirmImport();
    bool canceled = false;
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
        c_i->setParamModel(model);

//        delete model;
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

//        delete model;
        sign = true;
    }
    if (unknow_locations.isEmpty() != true)
    {
        QVector<Location*> l;
        QHash<QString, unsigned int>::iterator i;
        for ( i = unknow_locations.begin(); i != unknow_locations.end(); i ++)
        {
            Location* loc = new Location();
            loc->setName(i.key());
            l.append(loc);
        }

        ConfirmLocationModel *model = new ConfirmLocationModel();
        model->setItems(l);
        c_i->setLocationModel(model);
        sign = true;
//        delete model;
    }
    if (sign == true)
    {
        dlg = c_i;
        dlg->exec();
        canceled = c_i->isCanceled();
    }
    delete dlg;
    return canceled;
//    delete c_i;
}
void Importer::createParamsFromFile()
{
    for (int i = 5; i <= file->dimension().columnCount(); i ++)
    {
        QString param_name = file->read(1,i).toString();
        params_from_file.insert(i,Names::params->value(param_name));
    }
}
QVector<Sample*> Importer::import()
{
    checkHeaders();
    checkWaterTypes();
    checkLocations();
    QVector<Sample*> s;
    bool sign = confirm();

    if (sign != true)
    {
        createParamsFromFile();

        for (int i = 2; i <= file->dimension().rowCount(); i ++)
        {
            Sample *s1 = getSample(i);
            s.append(s1);
        }
    }
    return s;
}
Sample* Importer::getSample(int num)
{
    Sample *s = new Sample();

    QString water_name;
    unsigned int water_id;
    QString location_name;

    unsigned int location_id;

    location_name = file->read(num,1).toString();
    location_id = Names::locations->value(location_name);

    water_name = file->read(num,3).toString();
    water_id = Names::water_types->value(water_name);

    s->setLocationId(location_id);
    s->setDate(file->read(num,2).toDate());
    s->setWaterId(water_id);
    s->setComment(file->read(num,4).toString());
    s->setPosition(num - 2);

    QHash<unsigned int, ItemInSample> *components = new  QHash<unsigned int, ItemInSample>();
    QHash<unsigned int, unsigned int>::iterator param_iterator; // первый аргумент - номер параметра из файла; второй - id параметра в БД

    for (int i = 5; i <= file->dimension().columnCount(); i ++)
    {
        ItemInSample item_in_sample;
        double val = file->read(num,i).toDouble();
        param_iterator = params_from_file.find(i);
        item_in_sample.setItemId(param_iterator.value());
        item_in_sample.setValue(val);
        if (val >= 0)
        {
            item_in_sample.setChanged(1);
        }

        components->insert(param_iterator.value(), item_in_sample);
    }

    s->setComponents(components);
    return s;
}
void Importer::setParams(QHash<QString,unsigned int> *new_params)
{
    QHash<QString, unsigned int>::iterator i;
    for (i = new_params->begin(); i != new_params->end(); i ++)
    {
        params->insert(i.key(),i.value());
    }
}
void Importer::setWaterTypes(QHash<QString, unsigned int> *new_types)
{
    QHash<QString, unsigned int>::iterator i;
    for (i = new_types->begin(); i != new_types->end(); i ++)
    {
        water_types->insert(i.key(),i.value());
    }
}

void Importer::setLocations(QHash<QString, unsigned int> *new_locations)
{
    QHash<QString, unsigned int>::iterator i;
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
        if (Names::params->contains(param_name) == false)
        {
            unknow_params.insert(param_name,i);
        }
    }
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
}
