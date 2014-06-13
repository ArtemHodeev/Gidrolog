#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QVector>
#include <sample.h>
#include <iteminfo.h>
#include <QVector>
#include <QHash>
#include <standartization.h>

class Calculator : public Standartization
{
public:
    Calculator();
    ~Calculator();
    void setItems(QVector<Sample*> new_items);
    void getAnaliticItems();
    ItemInfo *checkItem(unsigned int item_id);
    double getCorrelation(double *mass1, double *mass2, int mass_len);
    QVector<ItemInfo*> getInfo();
    void standart();
    QVector<Sample*> getItems();

private:
    float corell;
    float lost_count;
    float error_count;

    void setItemError();

    QVector<ItemCorellation *> checkCorrelation(unsigned int item_id);
    QVector<Sample*> items; // все пробы системы
    QHash<unsigned int, double> *item_error; // хэш содержит пары <id компонента, ошибка измерения>
    QVector<Sample*> analitic_sample; // пробы только анализируемого типа
};

#endif // CALCULATOR_H
