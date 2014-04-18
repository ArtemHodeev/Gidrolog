#include "dbcreator.h"
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QSqlDriver>
#include "databaseaccessor.h"

DBCreator::DBCreator(QSqlDatabase db)
{
    this->db = db;
    //Указываются таблицы БД. Проблеммы: порядок указания важен, если у таблицы есть вторичные ключи,
    // то эта таблица должна быть указана после таблицы внешнего ключа.
    table_names.append("sample_set");
    table_names.append("item_type");
    table_names.append("water_type");
    table_names.append("location");
    table_names.append("item");
    table_names.append("sample");
    table_names.append("item_sample");
    table_names.append("factor");
}
DBCreator::~DBCreator()
{}
void DBCreator::checkTables()
{
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "show tables";

    QString name = "";
    QStringList unknow_table_names;

    query->prepare(sql);
    query->exec();
    QStringList tables_in_db;

    while(query->next())
    {
        name = query->value("Tables_in_gydro").toString();
        tables_in_db.append(name);
    }
    for(int i = 0; i < table_names.size(); i ++)
    {
        if (tables_in_db.contains(table_names[i]) != true)
        {
            unknow_table_names.append(table_names[i]);
        }
    }

    createSchema(unknow_table_names);
    delete query;
}
void DBCreator::checkSchema()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());

    QString sql = "";
    sql = "show databases";
    query->exec();

    //Если схемы  gydro нет
    bool sign = false;
    while(query->next())
    {
        if (query->value("Database").toString().compare("gydro") == 0)
        {
            sign = true;
        }
    }
    if (sign == false)
    {
        sql = "CREATE DATABASE gydro";
        query->prepare(sql);
        query->exec();
    }

    query->exec("USE gydro");

    checkTables();
    delete query;
}
bool DBCreator::createSchema(QStringList tables)
{
    QString query_sql = "";
    bool res = true;

    // если в схеме присутствуют не все таблицы. создать не достающие.
    // Проблемма: если добавляется таблица и для ее внешнего ключа нет еще внешней таблицы, то таблица не создастся

        while(tables.isEmpty() != true)
        {
            qDebug()<<"tables.first(): "<<tables.first();
            if (createTable(tables.first()) == false)
            {
                res = false;
            }

            tables.removeFirst();
        }
    return res;
}
bool DBCreator::createTable(QString table_name)
{
    QString query_sql = "";
    int ind = table_names.indexOf(table_name);

    switch (ind) {
    case 0:
        query_sql = "CREATE TABLE sample_set ";
            query_sql +="( ";
            query_sql += "id int NOT NULL AUTO_INCREMENT, ";
            query_sql += "name varchar(128) DEFAULT '', ";
            query_sql += "PRIMARY KEY (id) ";
            query_sql += ")";
        break;

    case 1:
        query_sql = "CREATE TABLE item_type ";
            query_sql +="( ";
            query_sql += "id int NOT NULL AUTO_INCREMENT, ";
            query_sql += "name varchar(128) DEFAULT '', ";
            query_sql += "priority int NOT NULL DEFAULT 0, ";
            query_sql += "PRIMARY KEY (id)";
            query_sql += ")";
        break;
    case 2:
        query_sql = "CREATE TABLE water_type ";
            query_sql +="( ";
            query_sql += "id int NOT NULL AUTO_INCREMENT, ";
            query_sql += "name varchar(128) DEFAULT '', ";
            query_sql += "PRIMARY KEY (id) ";
            query_sql += ")";
        break;
    case 3:
        query_sql = "CREATE TABLE location ";
            query_sql +="( ";
            query_sql += "id int NOT NULL AUTO_INCREMENT, ";
            query_sql += "name varchar(128) DEFAULT '', ";
            query_sql += "width real DEFAULT 0, ";
            query_sql += "length real DEFAULT 0, ";
            query_sql += "deep real DEFAULT 0, ";
            query_sql += "PRIMARY KEY (id) ";
            query_sql += ")";
        break;
    case 4:
        query_sql = "CREATE TABLE item ";
            query_sql +="( ";
            query_sql += "id int NOT NULL AUTO_INCREMENT, ";
            query_sql += "name varchar(128) NOT NULL DEFAULT '', ";
            query_sql += "type_id int NOT NULL, ";
            query_sql += "min_value double precision DEFAULT 0, ";
            query_sql += "error_line double precision DEFAULT 0, ";
            query_sql += "PRIMARY KEY (id), ";
            query_sql += "FOREIGN KEY (type_id) ";
            query_sql +=   "REFERENCES item_type (id) ";
            query_sql +=   "ON UPDATE CASCADE ON DELETE RESTRICT ";
            query_sql += ")";

        break;
    case 5:
        query_sql = "CREATE TABLE sample ";
        query_sql += "            ( ";
        query_sql += "            id int NOT NULL AUTO_INCREMENT, ";
        query_sql += "            sample_set_id int NOT NULL, ";
        query_sql += "           sample_date date NOT NULL, ";
        query_sql += "            location_id int NOT NULL, ";
        query_sql += "            comment varchar(1024) DEFAULT '', ";
        query_sql += "            water_type_id int NOT NULL, ";
        query_sql += "            PRIMARY KEY (id), ";
        query_sql += "            FOREIGN KEY (location_id) ";
        query_sql += "              REFERENCES location(id) ";
        query_sql += "              ON UPDATE CASCADE ON DELETE RESTRICT, ";
        query_sql += "             FOREIGN KEY (sample_set_id) ";
        query_sql += "              REFERENCES sample_set(id) ";
        query_sql += "              ON UPDATE CASCADE ON DELETE RESTRICT, ";
        query_sql += "             FOREIGN KEY (water_type_id) ";
        query_sql += "              REFERENCES water_type(id) ";
        query_sql += "              ON UPDATE CASCADE ON DELETE RESTRICT ";
        query_sql += "            )";
        break;
    case 6:
            query_sql = "CREATE TABLE item_sample ";
            query_sql += "( ";
            query_sql += "id int NOT NULL AUTO_INCREMENT, ";
            query_sql += "sample_id int NOT NULL, ";
            query_sql += "item_id int NOT NULL, ";
            query_sql += "value double NOT NULL DEFAULT -1, ";
            query_sql += "PRIMARY KEY (id), ";
            query_sql += "FOREIGN KEY (sample_id) ";
            query_sql += "              REFERENCES sample(id) ";
            query_sql += "              ON UPDATE CASCADE ON DELETE RESTRICT, ";
            query_sql += "FOREIGN KEY (item_id) ";
            query_sql += "              REFERENCES item(id) ";
            query_sql += "              ON UPDATE CASCADE ON DELETE RESTRICT ";
            query_sql += ")";
        break;
    case 7:
        query_sql = "CREATE TABLE factor ";
            query_sql +="( ";
            query_sql += "id int NOT NULL AUTO_INCREMENT, ";
            query_sql += "name varchar(128) DEFAULT '', ";
            query_sql += "value double precision DEFAULT 0, ";
            query_sql += "PRIMARY KEY (id) ";
            query_sql += ")";
        break;
    default:
        break;
    }

    bool res = false;
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare(query_sql);
    res = query->exec();

    delete query;
    return res;
}
