#ifndef SOLVERPCA_H
#define SOLVERPCA_H
#include <QVector>
#include <names.h>
#include <sample.h>

class SolverPCA
{
public:
    SolverPCA();
    ~SolverPCA();
    void makePlurals(int count, QVector<unsigned int> item_list);
    void makePlurals(int count, QVector<unsigned int> selected_items, QVector<unsigned int> item_list);
    void setAnaliticSamples(QVector<Sample*> analitic_samples);
    void lookForCompletePlurals();
private:
    QVector<unsigned int> custom_items; // компоненты, которые пользоваетль указал как обязательные
    QVector<unsigned int*> complete_plurals; // множества у которых на первые
                                             //две компоненты прихидится суммарная дисперсия более 90%
    QVector<unsigned int *> plurals; // содержит все сгенерированные множества
    unsigned int *temp_mass; // временное множество, которое бвло сгенерировано
    unsigned int *append_mass; // множество которое будет добавлено в plurals
    QVector<unsigned int> mass; // множество элесментов, из которых генерируется plurals
    QVector<Sample*> samples;
    QHash<unsigned int, QVector<double> > item_values; // хэш <id компонета, все значения компонента в пробах>

    int num; // размер генерируемых множеств
    int size; // размер итогового множества после генерации
    void genPlural(int count, int pos);
    void addPlural();

    void pickItemValues(unsigned int item_id);
};

#endif // SOLVERPCA_H
