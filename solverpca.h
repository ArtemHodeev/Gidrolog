#ifndef SOLVERPCA_H
#define SOLVERPCA_H
#include <QVector>
#include <names.h>
#include <sample.h>
#include <standartization.h>
#include <QMap>
#include <sampleinfo.h>

class SolverPCA: public Standartization
{
public:
    SolverPCA();
    ~SolverPCA();
    void makePlurals(int count, QVector<unsigned int> item_list);
    void makePlurals(int count, QVector<unsigned int> selected_items, QVector<unsigned int> item_list);
//    void setAnaliticSamples(QVector<Sample*> samples);
//    void setOtherSamples(QVector<Sample*> other_samples);
    void lookForCompletePlurals();
    void standart();
    void getAnaliticAverage();
    void setItems(QVector<Sample*> new_items);
    QVector<SampleInfo> getSamplesInfo(unsigned int *items_set, unsigned int water_id);
    QHash<unsigned int*, double**> complete_plurals; // <множество, у которого на первые
                                             //две компоненты прихидится суммарная дисперсия более 90%,
                                             // матрица нагрузок для этого множеств
private:
    QVector<unsigned int> custom_items; // компоненты, которые пользоваетль указал как обязательные

//    QHash< QVector<unsigned int*>, QVector<unsigned int*> > set_info;
    QVector<unsigned int *> plurals; // содержит все сгенерированные множества
    unsigned int *temp_mass; // временное множество, которое было сгенерировано
    unsigned int *append_mass; // множество, которое будет добавлено в plurals
    QVector<unsigned int> mass; // множество элесментов, из которых генерируется plurals
//    QVector<Sample*> samples;
    QVector<Sample*> items; // все пробы в системе
    QHash<unsigned int, QVector<double> > item_values; // хэш <id компонета, все значения компонента в пробах>
    QHash<unsigned int, double> *analitic_average; // хэш <id компонента,
                                                   // средне арифметическое значение концентрации в пробах анализируемого типа>
    int num; // размер генерируемых множеств
    int size; // размер итогового множества после генерации
    QMap<unsigned int, QVector<Sample*> > samples_by_type;
    void genPlural(int count, int pos);
    void addPlural();

//    void pickItemValues(unsigned int item_id);
    QVector<double> pickValues(unsigned int item_id, unsigned int water_id);
    void pickSamples();
};

#endif // SOLVERPCA_H
