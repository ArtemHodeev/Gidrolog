#include "starter.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <databaseaccessor.h>

Starter::Starter()
{
    map = new QMap<QString, unsigned int>();
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    query->exec("SELECT id, name FROM sample_set");
    while(query->next())
    {
        map->insert(query->value("name").toString(), query->value("id").toUInt());
    }
}
Starter::~Starter()
{
    delete map;
}
unsigned int Starter::saveSampleSet(QString name)
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    unsigned int last_id = 0;

    query->prepare("INSERT INTO sample_set (name) VALUES (:name)");
    query->bindValue(":name", name);
    query->exec();
    last_id = query->lastInsertId().toUInt();
    delete query;
    return last_id;
}

QMap<QString, unsigned int> *Starter::getSampleSets()
{
    return map;
}
