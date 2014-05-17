#ifndef SOLVERPCA_H
#define SOLVERPCA_H
#include <QVector>
#include <names.h>

class SolverPCA
{
public:
    SolverPCA();
    ~SolverPCA();
    void makePlurals(int count, QVector<unsigned int> item_list);

private:
    QVector<unsigned int *> plurals; // содержит все сгенерированные множества
    unsigned int *temp_mass; // временное множество. Добавляется в plurals
    QVector<unsigned int> mass; // множество элесментов, из которых генерируется plurals
    int num; // размер генериемых множеств
//    int size; // размер генериемых множеств
    void genPlural(int count, int pos);
    void print(int k);

};

#endif // SOLVERPCA_H
