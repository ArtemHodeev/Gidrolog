#ifndef NAMES_H
#define NAMES_H
#include <QHash>
#include <QString>

class Names
{
private:
    Names();
    Names(const Names &other);
    Names& operator=(const Names &other);

public:
    static QHash<QString, unsigned int> *params;// все какие есть параметры в БД.
//                                          QString - имя параметра, unsigned int - id параметра из БД
    static QHash<QString, unsigned int> *water_types; // все типы водных масс из БД
//                                                QSqtring - имя типа водной массы, unsigned int - id из БД
    static QHash<QString, unsigned int> *locations;// все места взятия из БД
    //                                                QSqtring - имя места взятия, unsigned int - id из БД
    static QHash<QString, unsigned int> *item_types;// все типы компонентов из БД
    //                                                QSqtring - имя типа компонента, unsigned int - id из БД
    static unsigned int analitic_id;
};

#endif // NAMES_H
