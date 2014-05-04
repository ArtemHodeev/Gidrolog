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
    table_names.append("analitic_type");
}
DBCreator::~DBCreator()
{}

/*
 * F:
 *  void DBCreator::checkTables()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция проверяет: какие таблицы есть в схеме БД, а каких нет. Те таблицы, которые отсутствуют в схеме
 *  функция запоминает и вызывает функцию createSchema для этих таблиц.
*/
void DBCreator::checkTables()
{
    QSqlQuery *query = new QSqlQuery(db);
    QString sql = "";
    QString name = "";
    QStringList unknow_table_names;
    QStringList tables_in_db;

    sql = "show tables";
    query->prepare(sql);
    query->exec();

    // получение всх таблиц схемы БД
    while(query->next())
    {
        name = query->value("Tables_in_gydro").toString();
        tables_in_db.append(name);
    }

    // Последовательное выполнение циклов, позволяет создать таблицы без ошибок, если таблицы указаны в верном порядке в table_names
    // Определение недостающих таблиц
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

/*
 * F:
 *  void DBCreator::checkSchema()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция проверяет: существует ли указанная БД. Если БД не существует, то функция создает ее.
 *  Функция делает указанныю при в настройках БД активной. После выбора активной БД, функция вызывает
 *  функция запоминает и вызывает функцию createSchema для этих таблиц.
*/
void DBCreator::checkSchema()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    bool sign = false;

    sql = "show databases";
    query->exec();

    while(query->next())
    {
        if (query->value("Database").toString().compare("gydro") == 0)
        {
            sign = true;
        }
    }

    //Если БД  gydro нет, то создать ее
    if (sign == false)
    {
        sql = "CREATE DATABASE gydro";
        query->prepare(sql);
        query->exec();
    }

    //Сделать БД "gydro" активной
    query->exec("USE gydro");

    checkTables();
    delete query;
}

/*
 * F:
 *  void DBCreator::createSchema(QStringList tables)
 * I:
 *  QStringList tables - список названий таблиц
 * O:
 *  true/false - (особой роли пока не играет)
 * D:
 *  Функция вызывает для каждой таблицы из tables функцию createTable
*/
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

/*
 * F:
 *  bool DBCreator::createTable(QString table_name)
 * I:
 *  QString table_name - название таблицы, которую нужно создать
 * O:
 *  true - если таблица создана
 *  false - если таблица не создана
 * D:
 *  Функция создает таблицу table_name со всеми ключами.
*/
bool DBCreator::createTable(QString table_name)
{
    QString query_sql = "";
    int ind = table_names.indexOf(table_name);
    bool set_val = false; // Флаг: нужно ли заполнять таблицу factor значениямя
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
            set_val = true;
        break;
    case 8:
        query_sql = "CREATE TABLE analitic_type ";
            query_sql +="( ";
            query_sql += "id int NOT NULL AUTO_INCREMENT, ";
            query_sql += "type_id int NOT NULL, ";
            query_sql += "PRIMARY KEY (id), ";
            query_sql += "FOREIGN KEY (type_id) ";
            query_sql += " REFERENCES water_type(id) ";
            query_sql += " ON UPDATE CASCADE ON DELETE CASCADE ";
            query_sql += ")";
        break;
    default:
        break;
    }

    bool res = false;
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare(query_sql);
    res = query->exec();

    if (set_val == true)
    {
        writeFactor();
    }
    delete query;
    return res;
}
void DBCreator::writeFactor()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
//    QString sql = "";

    query->exec("INSERT INTO factor(name) VALUES ('min_count')");
    query->exec("INSERT INTO factor(name) VALUES ('max_corell')");
    query->exec("INSERT INTO factor(name) VALUES ('max_error_count')");

}
