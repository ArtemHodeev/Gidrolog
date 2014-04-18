#include "databaseaccessor.h"
#include <QDebug>
#include <QSqlQuery>
#include<QStringList>
DatabaseAccessor::DatabaseAccessor()
{
    if (!db.isOpen())
    {
//        db = QSqlDatabase::addDatabase("QPSQL","pgConnect");
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setDatabaseName(dbName);
        db.setHostName(dbHost);
        db.setUserName(dbUser);
        db.setPassword(dbPassword);
        db.setPort(dbPort);
        if (!db.open())
        {
            qDebug() << "DB was not opened";
        }
    }
}


QString DatabaseAccessor::dbName = "";
QString DatabaseAccessor::dbHost = "localhost";
QString DatabaseAccessor::dbUser = "";
QString DatabaseAccessor::dbPassword = "";
int DatabaseAccessor::dbPort = 5432;
QSqlDatabase DatabaseAccessor::db = QSqlDatabase();

QSqlDatabase DatabaseAccessor::getDb()
{
    DatabaseAccessor instance;
    return instance.db;
}

//bool DatabaseAccessor::createDb()
//{

//}
//void DatabaseAccessor::checkTables()
//{
//    QSqlQuery *query = new QSqlQuery(db);

//    QString sql = "SELECT * FROM information_schema.tables where table_schema = 'gydro'";
//    QString name = "";
//    QStringList table_names;
//    QStringList unknow_table_names;

//    // Указываются таблицы БД
//    table_names.append("sample_set");
//    table_names.append("item_type");
//    table_names.append("water_type");
//    table_names.append("location");
//    table_names.append("item");
//    table_names.append("sample");
//    table_names.append("item_sample");
//    table_names.append("factor");

//    query->prepare(sql);
//    query->exec();

//    while(query->next())
//    {
//        name = query->value("table_name").toString();

//        if (table_names.contains(name) != true)
//        {
//            unknow_table_names.append(name);
//        }
//    }

//    createSchema(unknow_table_names);
//    delete query;

//}
//void DatabaseAccessor::checkSchema()
//{
//    QSqlQuery *query = new QSqlQuery(db);
//    QString sql = "";

//    sql = "SELECT schema_name FROM information_schema.schemata WHERE schema_name = 'gydro'";
//    query->prepare(sql);
//    query->exec();
//    query->first();
//    //Если схемы  gydro нет
//    if (query->size() == 0)
//    {
//        sql = "CREATE SCHEMA gydro";
//        query->prepare(sql);
//        query->exec();
//    }

//    checkTables();
//    delete query;
//}
//bool DatabaseAccessor::createSchema(QStringList tables)
//{
//    QString query_sql = "";
//    bool res = true;
//    // если схема пустая, то создать все таблицы. порядок важен, т.к. при создании вторичных ключей важно наличий внешней таблицы
//    if (tables.isEmpty() == true)
//    {
//        createTable("sample_set");
//        createTable("item_type");
//        createTable("water_type");
//        createTable("location");
//        createTable("item");
//        createTable("sample");
//        createTable("item_sample");
//        createTable("factor");
//    }
//    // если в схеме присутствуют не все таблицы. создать не достающие.
//    // Проблемма: если добавляется таблица и для ее внешнего ключа нет еще внешней таблицы, то таблица не создастся
//    else
//    {
//        while(tables.isEmpty() != true)
//        {
//            if (createTable(tables.first()) == false)
//            {
//                res = false;
//            }

//            tables.removeFirst();
//        }
//    }
//    return res;
//}
//bool DatabaseAccessor::createTable(QString table_name)
//{
//    QString query_sql = "";
//    enum table{item, item_type, water_type,location,sample_set,sample,item_sample,factor};
//    if ()

//    switch (table_name) {
//    case "item":
//        query_sql = "CREATE TABLE gydro.item ";
//            query_sql +="( ";
//            query_sql += "id bigserial NOT NULL, ";
//            query_sql += "name character(128) NOT NULL DEFAULT ''::bpchar, ";
//            query_sql += "type_id bigint NOT NULL, ";
//            query_sql += "min_value double precision DEFAULT 0, ";
//            query_sql += "error_line double precision DEFAULT 0, ";
//            query_sql += "CONSTRAINT pk_item PRIMARY KEY (id), ";
//            query_sql += "CONSTRAINT fk_item FOREIGN KEY (type_id) ";
//            query_sql +=   "REFERENCES gydro.item_type (id) MATCH SIMPLE ";
//            query_sql +=   "ON UPDATE CASCADE ON DELETE RESTRICT ";
//            query_sql += ")";
//        break;
//    case "item_type":
//        query_sql = "CREATE TABLE gydro.item_type ";
//            query_sql +="( ";
//            query_sql += "id bigserial NOT NULL, ";
//            query_sql += "name character(128) DEFAULT ''::bpchar, ";
//            query_sql += "priority smallint NOT NULL DEFAULT 0, ";
//            query_sql += "CONSTRAINT pk_item_type_key PRIMARY KEY (id)";
//            query_sql += ")";
//        break;
//    case "water_type":
//        query_sql = "CREATE TABLE gydro.water_type ";
//            query_sql +="( ";
//            query_sql += "id bigserial NOT NULL, ";
//            query_sql += "name character(128) DEFAULT ''::bpchar, ";
//            query_sql += "CONSTRAINT pk_water_type PRIMARY KEY (id) ";
//            query_sql += ")";
//        break;
//    case "location":
//        query_sql = "CREATE TABLE gydro.location ";
//            query_sql +="( ";
//            query_sql += "id bigserial NOT NULL, ";
//            query_sql += "name character(128) DEFAULT ''::bpchar, ";
//            query_sql += "width real DEFAULT 0, ";
//            query_sql += "length real DEFAULT 0, ";
//            query_sql += "deep real DEFAULT 0, ";
//            query_sql += "CONSTRAINT pk_location_key PRIMARY KEY (id) ";
//            query_sql += ")";
//        break;
//    case "sample_set":
//        query_sql = "CREATE TABLE gydro.sample_set ";
//            query_sql +="( ";
//            query_sql += "id bigserial NOT NULL, ";
//            query_sql += "name character(128) DEFAULT ''::bpchar, ";
//            query_sql += "CONSTRAINT sample_set_pk PRIMARY KEY (id) ";
//            query_sql += ")";
//        break;
//    case "sample":
//        query_sql = "CREATE TABLE gydro.sample ";
//            query_sql +="( ";
//            query_sql += "id bigserial NOT NULL, ";
//            query_sql += "sample_set_id bigint NOT NULL, ";
//            query_sql += "sample_date date NOT NULL, ";
//            query_sql += "location_id bigint NOT NULL, ";
//            query_sql += "comment character varying(1024) DEFAULT ''::character varying, ";
//            query_sql += "water_type_id bigint NOT NULL, ";
//            query_sql += "CONSTRAINT sample_pk PRIMARY KEY (id), ";
//            query_sql += "CONSTRAINT sample_fk_loc FOREIGN KEY (location_id) ";
//            query_sql +=   "REFERENCES gydro.location (id) MATCH SIMPLE ";
//            query_sql +=   "ON UPDATE CASCADE ON DELETE RESTRICT, ";
//            query_sql += "CONSTRAINT sample_fk_set FOREIGN KEY (sample_set_id) ";
//            query_sql +=   "REFERENCES gydro.sample_set (id) MATCH SIMPLE ";
//            query_sql +=   "ON UPDATE CASCADE ON DELETE RESTRICT, ";
//            query_sql += "CONSTRAINT sample_fk_water_type_id FOREIGN KEY (water_type_id) ";
//            query_sql +=   "REFERENCES gydro.water_type (id) MATCH SIMPLE ";
//            query_sql +=   "ON UPDATE CASCADE ON DELETE RESTRICT ";
//            query_sql += ")";
//        break;
//    case "item_sample":
//        query_sql = "CREATE TABLE gydro.item_sample ";
//            query_sql +="( ";
//            query_sql += "id bigserial NOT NULL, ";
//            query_sql += "sample_id bigint NOT NULL, ";
//            query_sql += "item_id bigint NOT NULL, ";
//            query_sql += "value double precision NOT NULL DEFAULT (-1), ";
//            query_sql += "CONSTRAINT item_sample_pk PRIMARY KEY (id), ";
//            query_sql += "CONSTRAINT item_sample_fk_sam FOREIGN KEY (sample_id) ";
//            query_sql +=   "REFERENCES gydro.sample (id) MATCH SIMPLE ";
//            query_sql +=   "ON UPDATE CASCADE ON DELETE RESTRICT, ";
//            query_sql += "CONSTRAINT itme_sample_fk_item FOREIGN KEY (item_id) ";
//            query_sql +=   "REFERENCES gydro.item (id) MATCH SIMPLE ";
//            query_sql +=   "ON UPDATE CASCADE ON DELETE RESTRICT, ";
//            query_sql += ")";
//        break;
//    case "factor":
//        query_sql = "CREATE TABLE gydro.factor ";
//            query_sql +="( ";
//            query_sql += "id bigserial NOT NULL, ";
//            query_sql += "name character(128) DEFAULT ''::bpchar, ";
//            query_sql += "value double precision DEFAULT 0, ";
//            query_sql += "CONSTRAINT factor_pk PRIMARY KEY (id) ";
//        break;
//    default:
//        break;
//    }

//    bool res = false;
//    QSqlQuery *query = new QSqlQuery(db);

//    query->prepare(query_sql);
//    res = query->exec();

//    delete query;
//    return res;
//}
