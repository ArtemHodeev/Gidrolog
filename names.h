#ifndef NAMES_H
#define NAMES_H
#include <QHash>
#include <Qstring>

class Names
{
    Names();

    Names& operator=(const Names &other);
public:
    Names(const Names &other);
    static QHash<QString, unsigned int> *params;// все какие есть параметры в БДQ.
//                                          QString - имя параметра, unsigned int - id параметра из БД
    static QHash<QString, unsigned int> *water_types; // все типы водных масс из БД
//                                                QSqtring - имя типа водной массы, unsigned int - id из БД
    static QHash<QString, unsigned int> *locations;
};

#endif // NAMES_H
