#include "importer.h"
#include <QDebug>
#include <xlsx/xlsxcellrange.h>

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
void Importer::import()
{
    checkHeaders();
    qDebug()<<"unknow_params size: "<<unknow_params.size();
}
void Importer::setParams(QHash<QString,unsigned int> *new_params)
{
//    qDebud
    QHash<QString, unsigned int>::iterator i;
    for (i = new_params->begin(); i != new_params->end(); i ++)
    {
        qDebug()<<"key: "<< i.key() <<"  value: " << i.value();
        params->insert(i.key(),i.value());
    }
}
void Importer::checkHeaders()
{
    for (int i = 5; i <= file->dimension().columnCount(); i ++)
    {
        QString param_name = file->read(1,i).toString();
        qDebug()<<"param_name: "<< param_name;
        if (!params->contains(param_name))
        {
            unknow_params.insert(param_name,i);
        }
    }
}
