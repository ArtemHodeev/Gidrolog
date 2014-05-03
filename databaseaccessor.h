#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H
#include <QSqlDatabase>

class DatabaseAccessor
{

public:
    static QString dbName; // название БД
    static QString dbHost; // название хоста
    static QString dbUser; // имя пользователя
    static QString dbPassword; // пароль пользователя
    static int dbPort; // порт БД

    static QSqlDatabase getDb();

private:
    DatabaseAccessor();
    DatabaseAccessor& operator=(const DatabaseAccessor&);
    DatabaseAccessor(const DatabaseAccessor&);

    static QSqlDatabase db; // открытая БД
};

#endif // DATABASEACCESSOR_H
