#include "databaseaccessor.h"
#include <QDebug>
#include <QSqlQuery>
#include<QStringList>
DatabaseAccessor::DatabaseAccessor()
{
    if (!db.isOpen())
    {
        db = QSqlDatabase::addDatabase("QPSQL","pgConnect");
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

bool DatabaseAccessor::createDb()
{

}
bool DatabaseAccessor::checkTables()
{
    QSqlQuery *query = new QSqlQuery(db);

    QString sql = "SELECT * FROM information_schema.tables where table_schema = 'gydro'";
    QStringList table_names;
    table_names.append("item");
    table_names.append("item_sample");
    table_names.append("water_type");
    table_names.append("item_type");
    table_names.append("factor");
    table_names.append("location");
    table_names.append("sample_set");

    query->prepare(sql);
    query->exec();
    while(query->next())
    {
//        if ()
    }

}
