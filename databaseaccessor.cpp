#include "databaseaccessor.h"
#include <QDebug>
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

