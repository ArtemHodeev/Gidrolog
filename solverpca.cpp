#include "solverpca.h"
#include <QDebug>
#include <alglib/dataanalysis.h>

SolverPCA::SolverPCA()
{

}
SolverPCA::~SolverPCA()
{
//    delete []temp_mass;
    delete []append_mass;
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

void SolverPCA::makePlurals(int count, QVector<unsigned int> selected_items, QVector<unsigned int> item_list)
{
    size = count;
    num = count - selected_items.size();
    custom_items = selected_items;
    QVector<unsigned int> available_items;

    for (int i = 0; i < item_list.size(); i ++)
    {
        if (selected_items.contains(item_list[i]) == false)
        {
            available_items.append(item_list[i]);
        }
    }

    makePlurals(num,available_items);
}
void SolverPCA::addPlural()
{
    append_mass = new unsigned int[size];
//    unsigned int *m = new unsigned int[size];
    int index = 0;

    //Внесение компонентов, которые указал пользователь
    for (index = 0; index < custom_items.size(); index ++)
    {
        append_mass[index] = custom_items[index];
    }

    // Внесение компонентов множества, кторые сгенерировала система
    for (int i = 0; i < num;  i ++)
    {
        append_mass[index] = temp_mass[i];
        index ++;
    }

    QString str = "plural: ";
    for (int i = 0; i < size;  i++)
    {
        str += QString("%1 | ").arg(append_mass[i]);
    }
    qDebug()<<str;
    plurals.append(append_mass);
}



void SolverPCA::genPlural(int count, int pos)
{
    if (count == 0)
    {
        addPlural();
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
void SolverPCA::setAnaliticSamples(QVector<Sample *> analitic_samples)
{
    samples = analitic_samples;
}

void SolverPCA::pickItemValues(unsigned int item_id)
{
    QHash<unsigned int, ItemInSample>::iterator iterator;
    QVector<double> values;

    for (int i = 0; i < samples.size(); i ++)
    {
        iterator = samples[i]->getComponents()->find(item_id);
        values.append(iterator.value().getValue());
    }
    item_values.insert(item_id,values);
}

void SolverPCA::lookForCompletePlurals()
{
//    unsigned int *temp;
    alglib::real_2d_array dataset;
    alglib::real_2d_array res_dataset;
    alglib::real_1d_array variars;
    alglib::ae_int_t rows;
    alglib::ae_int_t cols;
    alglib::ae_int_t info;
    double *container = new double[size * samples.size()];
    QHash<unsigned int, QVector<double> >::iterator item_value_iterator;

    rows = samples.size();
    cols = size;
    info = 0;

    // Собрать значеняи компонента в пробах в хэш, для того что бы каждый раз не исобирать
    QHash<QString, unsigned int>::iterator iter;
    for (iter = Names::params->begin(); iter != Names::params->end(); iter ++)
    {
        pickItemValues(iter.value());
    }

    for (int plural_ind = 0; plural_ind < plurals.size(); plural_ind ++)
    {
        for (int row_ind = 0; row_ind < samples.size(); row_ind ++)
        {
            // создать массив данных для проведения метода PCA
            // Container заполняется согласно тому как будет разбираться в real_2d_array.setcontainer()
            for (int col_ind = 0; col_ind < size; col_ind ++)
            {
                item_value_iterator = item_values.find(plurals[plural_ind][col_ind]);
//                QVector<double> t = item_value_iterator.value();
                container[row_ind * size + col_ind] = item_value_iterator.value()[row_ind];
            }
        }
            // подготовка массива данных
            dataset.setcontent(rows, cols, container);

            //метод PCA
            alglib::pcabuildbasis(dataset,rows, cols,info,variars, res_dataset);

            QString str = "";
            qDebug()<<"======================";
            for (int i = 0; i < res_dataset.rows(); i ++)
            {

                for (int j = 0; j < res_dataset.cols(); j ++)
                {
                    str += QString("%1 ").arg(res_dataset(i,j));
                }
                qDebug()<<str;
                str = "";
            }
    }
    delete []container;

}
