#include "pgconnector.h"
#include <QDebug>
PgConnector::PgConnector()
{
    connected = false;
    dbName = "";
    dbHost = "";
    dbUser = "";
    dbPassword = "";
    dbPort = 0;
}
PgConnector::PgConnector(const QString name, const QString host, const QString user, const QString password, const int port)
{
    dbName =name;
    dbHost = host;
    dbUser = user;
    dbPassword = password;
    dbPort = port;
    connected = false;
}
//PgConnector::~PgConnector()
//{
//    delete query;
//}
bool PgConnector::connect()
{
    if (connected == false){
        db =  QSqlDatabase::addDatabase("QPSQL");
        db.setDatabaseName(dbName);
        db.setHostName(dbHost);
        db.setUserName(dbUser);
        db.setPassword(dbPassword);
        db.setPort(dbPort);
    }
    if(db.open()){
        connected = true;
    }
    else{
        connected = false;
        qDebug()<<db.lastError().text();
    }
    return connected;
}

void PgConnector::disconnect()
{
    if (connected == true){
        db.close();
        connected = false;
    }
}
QSqlDatabase PgConnector::getDb()
{
    return db;
}
