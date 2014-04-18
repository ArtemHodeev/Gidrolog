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


private:
    DatabaseAccessor();
    DatabaseAccessor& operator=(const DatabaseAccessor&);
    DatabaseAccessor(const DatabaseAccessor&);
//    void checkTables();
////    bool createSchema();
//    bool createSchema(QStringList tables);
//    bool createTable(QString table_name);
////    bool createDb();
    static QSqlDatabase db;

};

#endif // DATABASEACCESSOR_H
