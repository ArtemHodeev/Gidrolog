#include "databaseaccessor.h"
#include <QDebug>
#include <QSqlQuery>
//#include <QSettings>
#include <QDialog>
#include <connectdb.h>

DatabaseAccessor::DatabaseAccessor()
{

    if (!db.isOpen())
    {
        setting = new QSettings();

        db = QSqlDatabase::addDatabase("QMYSQL");
        setting->beginGroup("dbSettings");
        db.setDatabaseName(setting->value("name","").toString());
        db.setHostName(setting->value("host","localhost").toString());
        db.setUserName(setting->value("user","user").toString());
        db.setPassword(setting->value("password","123").toString());
        db.setPort(setting->value("port",3306).toInt());
        setting->endGroup();

        if (!db.open())
        {
            QDialog *dlg = new ConnectDB();
            dlg->exec();
            delete dlg;
        }
    }
}

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

void DatabaseAccessor::reconnect()
{
    if (db.isOpen())
    {
        db.close();
    }
    db = getDb();

}
