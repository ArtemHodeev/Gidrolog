#include "calculator.h"
#include <names.h>
#include <QSqlQuery>
#include <QDebug>
#include <databaseaccessor.h>


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
void Calculator::setItems(QVector<Sample *> new_sample)
{
    for (int i = 0; i < new_sample.size(); i ++)
    {
        items.append(new_sample[i]);
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
///*
// * F:
// *   void Calculator::setItemError()
// * I:
// *   --
// * O:
// *   --
// * D:
// *   Функция извлекает из БД id компонентов и значения их ошибок измерения.
// */
//void Calculator::setAnaliticId(unsigned int id)
//{
//    analitic_id = id;
//}

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
    int lost = 0;
    int error = 0;


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
//            qDebug()<<"Value: "<<item_error_iter.value();

            // Если значение компонента в пробе меньше чем удвоенное значение
            // ошибки измерения для компонента
            if (item_in_sample.getValue() < 2 * item_error_iter.value())
            {
                error ++;
            }
        }
    }
    qDebug()<<"Item: "<<Names::params->key(item_id);
    qDebug()<<"lost: "<< lost;
    qDebug()<<"error: "<<error;
    info->setErrorCount(error);
    info->setLostCount(lost);
    return info;
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
    for ( i = Names::params->begin(); i != Names::params->end(); i ++)
    {
        ItemInfo *info = new ItemInfo();

        info = checkItem(i.value());
        list_info.append(info);
    }
//    for (int i = 0; i < list_info.size(); i ++)
//    {
//        qDebug()<<"item_id: "<<list_info[i]->getItemId();
//        qDebug()<<"lost: "<<list_info[i]->getLostCount();
//        qDebug()<<"error: "<<list_info[i]->getErrorCount();
//        qDebug()<<"============================";
//    }
    qDebug()<<"List_info size: "<<list_info.size();
    return list_info;
}
