#include "calculator.h"
#include <names.h>
#include <QSqlQuery>
#include <QDebug>
#include <databaseaccessor.h>
#include <math.h>


Calculator::Calculator()
{
    item_error = new QHash<unsigned int, double>();
    setItemError();
}
Calculator::~Calculator()
{
    delete item_error;
}

/*
 * F:
 *   void Calculator::setItems(QVector<Sample *> new_sample)
 * I:
 *   new_sample - все пробы системы
 * O:
 *   --
 * D:
 *   Функция устанавливает items.
 */
void Calculator::setItems(QVector<Sample *> new_items)
{
    for (int i = 0; i < new_items.size(); i ++)
    {
        items.append(new_items[i]);
    }

    getAnaliticItems();
}

/*
 * F:
 *   void Calculator::setItemError()
 * I:
 *   --
 * O:
 *   --
 * D:
 *   Функция извлекает из БД id компонентов и значения их ошибок измерения.
 */
void Calculator::setItemError()
{
    QSqlQuery *query = new QSqlQuery(DatabaseAccessor::getDb());
    QString sql = "";
    unsigned int item_id = 0;
    double error = 0;

    sql = "SELECT id, error_line FROM item";
    query->exec(sql);

    while (query->next())
    {
        item_id = query->value("id").toUInt();
        error = query->value("error_line").toDouble();
        item_error->insert(item_id, error);
    }

    delete query;
}

/*
 * F:
 *   void Calculator::getAnaliticItems()
 * I:
 *   --
 * O:
 *   --
 * D:
 *   Функция извлекает из всех проб системы, пробы только анализируемого типа.
 */
void Calculator::getAnaliticItems()
{
    // Перебор всех проб. Если проба анализирумого типа, то занести ее в analitic_sample
    for (int i = 0; i < items.size(); i ++)
    {
        if (items[i]->getWaterId() == Names::analitic_id)
        {
            analitic_sample.append(items[i]);
        }
    }
}

/*
 * F:
 *   ItemInfo* Calculator::checkItem(unsigned int item_id)
 * I:
 *   item_id - id элемента, который следует проверить на пропуски в пробах и на ошибку измерения
 * O:
 *   Объект ItemInfo* содержащий информацию во скольких пробах отсутствует элемент,
 *   и во скольких пробах имеет ошибочное значение
 * D:
 *   Функция проверяет элемент во всех пробах анализируемого типа, если данного компонента нет в пробе,
 *   то счетчик пропусков увеличивается, если компонент присутствует в пробе, но его значение
 *   меньше чем удвоенное значение его ошибки измерения, то увеличивается счетчик ошибок измерения.
 */
ItemInfo* Calculator::checkItem(unsigned int item_id)
{
    ItemInfo *info = new ItemInfo();
    float lost = 0;
    float error = 0;

    info->setItemId(item_id);

    // Перебор всех проб анализируемого типа
    for (int i = 0; i < analitic_sample.size(); i ++)
    {
        // Поиск компонента в текущей пробе
        QHash<unsigned int, ItemInSample>::iterator sample_iter = analitic_sample[i]->getComponents()->find(item_id);

        //если компонент отсутствует
        if (sample_iter == analitic_sample[i]->getComponents()->end())
        {
            lost ++;
        }

        else
        {
            ItemInSample item_in_sample = sample_iter.value();
            // Поиск значения ошибки измерения для компонента
            QHash<unsigned int, double>::iterator item_error_iter = item_error->find(item_id);

            // Если значение компонента в пробе меньше чем удвоенное значение
            // ошибки измерения для компонента
            if (item_in_sample.getValue() < 2 * item_error_iter.value())
            {
                error ++;
            }
        }
    }

    // Получение информации о прорусках и ошибочных данных в процентах
    lost = lost / analitic_sample.size() * 100;
    error = error / analitic_sample.size() * 100;
    checkCorrelation();
    info->setErrorCount(error);
    info->setLostCount(lost);
    return info;
}
void Calculator::checkCorrelation()
{
    int len = 10;
    double *mass = new double[len];
    double *mass2 = new double[len];

    for (int i = 0; i < len; i ++)
    {
        mass[i] = double(i) / 3 + double(i) * 3;
        mass2[i] = mass[i] *  mass[i];
//        qDebug()<<"mass[i]: "<< mass[i];
//        qDebug()<<"mass2[i]: "<< mass2[i];
    }

    double res = getCorrelation(mass, mass2, len);
    qDebug()<<"res: "<< res;
}
/*
 * F:
 *   QVector<ItemInfo*> Calculator::getInfo()
 * I:
 *   --
 * O:
 *   Вектор содержащий в себе объекты для каждого параметра, в которых содержится информация
 *   количестве пропусков и количестве ошибочных значений соответствующего параметра в пробах
 * D:
 *   Функция переберает все параметры системы и для каждого параметра вызывается функция checkItem.
 *
 */
QVector<ItemInfo*> Calculator::getInfo()
{
    QVector<ItemInfo*> list_info;
    QHash<QString, unsigned int>::iterator i;
    ItemInfo *info;

    for ( i = Names::params->begin(); i != Names::params->end(); i ++)
    {
        info = new ItemInfo();

        info = checkItem(i.value());
        list_info.append(info);
    }
    delete info;
    return list_info;
}

double Calculator::getAverage(double *mass, int length)
{
    double sum = 0;
    for (int i = 0; i < length; i ++)
    {
        sum += mass[i];
    }
    return sum / length;
}
double Calculator::getAverageSquare(double *mass_1, double average_1, double *mass_2, double average_2, int length)
{
    double sum = 0;
    double temp = 0;
    qDebug()<<"====================";
    for (int i = 0; i < length; i ++)
    {
        qDebug()<<"mass_1: "<< mass_1[i];
        qDebug()<<"mass_2: "<< mass_2[i];
        temp = (mass_1[i] - average_1) * (mass_2[i] - average_2);
        sum += temp;
        qDebug()<<"temp: "<< temp;
    }

    return sum;
}
double Calculator::getCorrelation(double *mass1, double *mass2, int mass_len)
{
    double average_mass1 = 0;
    double average_mass2 = 0;
    double sum_average_square_mass_1 = 0;
    double sum_average_square_mass_2 = 0;
    double sum_average = 0;
    double correlation = 0;
    double *average_square_mass_1;
    double *average_square_mass_2;
    double *average;

    average_square_mass_1 = new double[mass_len];
    average_square_mass_2 = new double[mass_len];
    average = new double[mass_len];

    average_mass1 = getAverage(mass1, mass_len);
    average_mass2 = getAverage(mass2, mass_len);


    sum_average_square_mass_1 = getAverageSquare(mass1, average_mass1, mass1, average_mass1, mass_len);
    sum_average_square_mass_2 = getAverageSquare(mass2, average_mass2, mass2, average_mass2, mass_len);
    sum_average = getAverageSquare(mass1,average_mass1, mass2, average_mass2, mass_len);

    correlation = sum_average / (sqrt(sum_average_square_mass_1 * sum_average_square_mass_2));

    qDebug()<<"average_mass1: "<< average_mass1;
    qDebug()<<"average_mass2: "<< average_mass2;

    qDebug()<<"s1: "<< sum_average_square_mass_1;
    qDebug()<<"s2: "<< sum_average_square_mass_2;
    qDebug()<<"s3: "<< sum_average;

    qDebug()<<"corellation: "<< correlation;


    return correlation;
}
