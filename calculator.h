#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QVector>
#include <sample.h>
#include <iteminfo.h>
#include <QVector>
#include <QHash>

class Calculator
{
public:
    Calculator();
    ~Calculator();
    void setItems(QVector<Sample*> new_items);
//    void setAnaliticId(unsigned int id);
    void getAnaliticItems();
    ItemInfo *checkItem(unsigned int item_id);
    QVector<ItemInfo*> getInfo();
//    QVector<>

private:
    float corell;
    float lost_count;
    float error_count;

    void setItemError();
    QVector<Sample*> items; // все пробы системы
//    unsigned int analitic_id;
    QHash<unsigned int, double> *item_error; // хэш содержит пары <id компонента, ошибка измерения>
    QVector<Sample*> analitic_sample; // пробы только анализируемого типа
};

#endif // CALCULATOR_H
