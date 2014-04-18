#ifndef DBCREATOR_H
#define DBCREATOR_H
#include <QVector>
#include <QSqlDatabase>


class DBCreator
{
public:
    DBCreator(QSqlDatabase db);
    ~DBCreator();
    void checkSchema();
//    DatabaseAccessor();
private:
    void checkTables();
//    bool createSchema();
    bool createSchema(QStringList tables);
    bool createTable(QString table_name);
//    bool createDb();
    QSqlDatabase db;
    QVector<QString> table_names;
};

#endif // DBCREATOR_H
