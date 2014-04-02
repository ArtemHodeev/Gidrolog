#ifndef PGCONNECTOR_H
#define PGCONNECTOR_H
#include <string>
#include <QSqlDatabase>
#include <QSqlError>
#include <connector.h>
namespace connect {
class PgConnector;
}
class PgConnector : public Connector
{
    QString dbName;
    QString dbHost;
    QString dbUser;
    QString dbPassword;
    unsigned int dbPort;
    QSqlDatabase db;
    bool connected;
public:
//    QSqlDatabase db;
    PgConnector();
//    ~PgConnector();
    PgConnector(const QString name, const QString host, const QString user, const QString password, const int port);
    bool connect();
    void disconnect();
    QSqlDatabase getDb();

};

#endif // PGCONNECTOR_H
