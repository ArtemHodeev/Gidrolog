#include "solverpca.h"
#include <QDebug>

SolverPCA::SolverPCA()
{

}
SolverPCA::~SolverPCA()
{

}

void SolverPCA::makePlurals(int count, QVector<unsigned int> item_list)
{
     temp_mass = new unsigned int[count];
     mass = item_list;
     num = count;
//     size = item_list.size();
     genPlural(count, 0);

     qDebug()<<"plurals size: "<< plurals.size();
     delete []temp_mass;

}
void SolverPCA::print(int k)
{
    QString str = "plural: ";
    for (int i = 0; i < k;  i++)
    {
        str += QString("%1 | ").arg(temp_mass[i]);
    }
    qDebug()<<str;
}
void SolverPCA::genPlural(int count, int pos)
{
    if (count == 0)
    {
        print(num);
        plurals.append(temp_mass);
    }
    else
    {
        for (int i = pos; i < mass.size() - count + 1; i ++)
        {
            temp_mass[num - count] = mass[i];
            genPlural(count - 1, i + 1);
        }
    }
}

