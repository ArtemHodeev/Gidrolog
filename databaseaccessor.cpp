#include "databaseaccessor.h"
#include <QDebug>
#include <QSqlQuery>
#include<QStringList>
DatabaseAccessor::DatabaseAccessor()
{
    if (!db.isOpen())
    {
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

// Инициализация параметров по умолчанию
QString DatabaseAccessor::dbName = "";
QString DatabaseAccessor::dbHost = "localhost";
QString DatabaseAccessor::dbUser = "";
QString DatabaseAccessor::dbPassword = "";
int DatabaseAccessor::dbPort = 5432;
QSqlDatabase DatabaseAccessor::db = QSqlDatabase();

/*
 * F:
 *  QSqlDatabase DatabaseAccessor::getDb()
 * I:
 *  --
 * O:
 *  Возвращяет ссылку на открытую БД
 * D:
 *  функция создает подключение к БД, если оно еще не было установлено, и возвращает открытую БД
 */
QSqlDatabase DatabaseAccessor::getDb()
{
    DatabaseAccessor instance;
    return instance.db;
}
