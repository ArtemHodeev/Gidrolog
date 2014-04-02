#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <QSqlDatabase>
class Connector
{

public:
    Connector();
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual QSqlDatabase getDb() = 0;
};

#endif // CONNECTOR_H
