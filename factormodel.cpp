#include "factormodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <databaseaccessor.h>

FactorModel::FactorModel()
{
    lost_count = 100;
    error_count = 100;
    corell = 90;
    analitic_id = 0;
}

FactorModel::~FactorModel()
{
}

FactorModel::setItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QSring sql = "";

    sql = "SELECT name, value FROM factor";
    query->exec(sql);
//    query->next();

    while(query->next())
    {
        if (query->value("lost_count"))
        {
            lost_count = query->value("value").toFloat();
        }
        else if(query->value("error_count"))
        {
            error_count = query->value("value").toFloat();
        }
        else if(query->value("corell"))
        {
            corell = query->value("value").toFloat();
        }
    }

    sql = "SELECT type_id FROM anatitic_type";
    query->exec(sql);

    if(query->next())
    {
        analitic_id = query->value("type_id").toUInt();
    }

    delete query;
}
FactorModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

    sql = "UPDATE factor SET (value = :val) WHERE name = :name";
    query->prepare(sql);

    query->bindValue(":name", 'lost_count');
    query->bindValue(":val", lost_count);
    query->exec();

    query->bindValue(":name", 'error_count');
    query->bindValue(":val", error_count);
    query->exec();

    query->bindValue(":name", 'corell');
    query->bindValue(":val", corell);
    query->exec();

    sql = "SELECT id FROM analitic_type LIMIT 1";
    query->exec(sql);

    unsigned int id = 0;
    if (query->size() != 0)
    {
        query->first();
        id = query->value("id").toUInt();
        sql = "UPDATE analitic_type SET (type_id = :analitic_id) WHERE id = :id";
        query->prepare(sql);
        query->bindValue(":id", id);
        query->bindValue(":analitic_id",analitic_id);
    }
    else
    {
        sql = "INSERT INTO analitic_type (type_id) VALUES(:type_id)";
        query->prepare(sql);
        query->bindValue(":type_id", analitic_id);
    }
    query->exec();

//    if ()
}
