#include "factormodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <databaseaccessor.h>
#include <names.h>
#include <QHash>

FactorModel::FactorModel()
{
    lost_count = 100;
    error_count = 100;
    corell = 100;
    analitic_id = 0;
}

FactorModel::~FactorModel()
{
}

/*
 * F:
 *   void FactorModel::setItems()
 * I:
 *   --
 * O:
 *   --
 * D:
 *   Функция устанавливает значения критериев, значения которых хранятся в БД.
 */
void FactorModel::setItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

    // Получение значений коэффициентов максимально допустимых корелляций, пропусков данных и ошибочных данных
    sql = "SELECT name, value FROM factor";
    query->exec(sql);

    while(query->next())
    {
        QString name = query->value("name").toString();
        if (name.compare("lost_count") == 0)
        {
            lost_count = query->value("value").toFloat();
        }
        else if(name.compare("error_count") == 0)
        {
            error_count = query->value("value").toFloat();
        }
        else if(name.compare("corell") == 0)
        {
            corell = query->value("value").toFloat();
        }
    }

    // Получение анализируемого типа водной массы
    sql = "SELECT type_id FROM analitic_type";
    query->exec(sql);

    if(query->next())
    {
        analitic_id = query->value("type_id").toUInt();
    }

    delete query;
}

/*
 * F:
 *   void FactorModel::saveItems()
 * I:
 *   --
 * O:
 *   --
 * D:
 *   Функция сохраняет значения критериев в БД
 */
void FactorModel::saveItems()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";

    // Сохранение значений коэффициентов максимально допустимых корелляции, ошибочных данных и пропусков
    // Происходит update, т.к. коэффициенты вносятся в БД на этапе создания таблицы
    sql = "UPDATE factor SET value = :val WHERE name = :name";
    query->prepare(sql);

    query->bindValue(":name", "lost_count");
    query->bindValue(":val", lost_count);
    query->exec();

    query->bindValue(":name", "error_count");
    query->bindValue(":val", error_count);
    query->exec();

    query->bindValue(":name", "corell");
    query->bindValue(":val", corell);
    query->exec();

    // Сохранение анализируемого типа водной массы
    sql = "SELECT id FROM analitic_type LIMIT 1";
    query->exec(sql);

    unsigned int id = 0;

    // если анализируемы тип водной массы сменился
    if (query->size() != 0)
    {
        query->first();
        id = query->value("id").toUInt();
        sql = "UPDATE analitic_type SET type_id = :analitic_id WHERE id = :id";
        query->prepare(sql);
        query->bindValue(":id", id);
        query->bindValue(":analitic_id",analitic_id);
    }
    // если анализируемый тип еще не известен (первый запуск программы)
    else
    {
        sql = "INSERT INTO analitic_type (type_id) VALUES(:type_id)";
        query->prepare(sql);
        query->bindValue(":type_id", analitic_id);
    }
    query->exec();
    qDebug()<<"error: "<<query->lastError().text();
    Names::analitic_id = analitic_id;
}

float FactorModel::getLostCount()
{
    return lost_count;
}
float FactorModel::getErrorCount()
{
    return error_count;
}
float FactorModel::getCorell()
{
    return corell;
}
unsigned int FactorModel::getAnaliticId()
{
    return analitic_id;
}

/*
 * F:
 *   QStringList FactorModel::getWaterTypes()
 * I:
 *   --
 * O:
 *   --
 * D:
 *   Функция формирует список всех типов водных масс, которые есть в системе и возвращает его
 */
QStringList FactorModel::getWaterTypes()
{
    QStringList list;
    QHash<QString, unsigned int>::iterator i;
    for (i = Names::water_types->begin(); i != Names::water_types->end(); i ++)
    {
        list.append(i.key());
    }
    return list;
}

/*
 * F:
 *   QString FactorModel::getAnaliticName()
 * I:
 *   --
 * O:
 *   --
 * D:
 *   Функция возвращает имя анализируемого типа водной массы, согласно id текущего анализируемого типа
 */
QString FactorModel::getAnaliticName()
{
    return Names::water_types->key(analitic_id);
}
void FactorModel::setCorell(const float &cor)
{
    corell = cor;
}
void FactorModel::setErrorCount(const float &error)
{
    error_count = error;
}
void FactorModel::setLostCount(const float &lost)
{
    lost_count = lost;
}
void FactorModel::setAnaliticId(const QString &name)
{
    analitic_id = Names::water_types->value(name);
}
