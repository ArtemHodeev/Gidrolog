#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H
#include <QSqlDatabase>
#include <QSettings>

class DatabaseAccessor
{

public:
    static QSqlDatabase getDb();
    static void reconnect();

private:
    QSettings *setting;
    static QSqlDatabase db; // открытая БД

    DatabaseAccessor();
    DatabaseAccessor& operator=(const DatabaseAccessor&);
    DatabaseAccessor(const DatabaseAccessor&);


};

#endif // DATABASEACCESSOR_H
