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

private:
    void checkTables();
    bool createSchema(QStringList tables);
    bool createTable(QString table_name);
    QSqlDatabase db;
    QVector<QString> table_names; // все необходимые для работы системы таблицы БД
};

#endif // DBCREATOR_H
