#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H
#include <QSqlDatabase>

class DatabaseAccessor
{

public:
    static QString dbName;
    static QString dbHost;
    static QString dbUser;
    static QString dbPassword;
    static int dbPort;

    static QSqlDatabase getDb();
    bool createDb();
    bool checkTables();

private:
    DatabaseAccessor();
    DatabaseAccessor& operator=(const DatabaseAccessor&);
    DatabaseAccessor(const DatabaseAccessor&);

    static QSqlDatabase db;
};

#endif // DATABASEACCESSOR_H
