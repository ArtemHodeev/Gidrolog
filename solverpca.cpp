#include "solverpca.h"
#include <QDebug>
#include <alglib/dataanalysis.h>

#include <analiticpoints.h>
#include <springpoints.h>
#include <omp.h>
#include <time.h>

SolverPCA::SolverPCA()
{
    analitic_average = new QHash<unsigned int, double>();
}
SolverPCA::~SolverPCA()
{
//    delete []temp_mass;
    delete []append_mass;
    plurals.clear();
    complete_plurals.clear();
    custom_items.clear();
    items.clear();
    analitic_average->clear();
    samples_by_type.clear();
    item_values.clear();
    mass.clear();

}

void SolverPCA::makePlurals(int count, QVector<unsigned int> item_list)
{
     temp_mass = new unsigned int[count];
     mass = item_list;
     num = count;
//     size = item_list.size();
     genPlural(count, 0);

//     qDebug()<<"plurals size: "<< plurals.size();
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

    lookForCompletePlurals();
}
void SolverPCA::addPlural()
{
    append_mass = new unsigned int[size];
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

//void SolverPCA::setAnaliticSamples(QVector<Sample *> analitic_samples)
//{
//    samples = analitic_samples;
//}

//void SolverPCA::pickItemValues(unsigned int item_id)
//{
//    QHash<unsigned int, ItemInSample>::iterator iterator;
//    QVector<double> values;

//    for (int i = 0; i < samples.size(); i ++)
//    {
//        iterator = samples[i]->getComponents()->find(item_id);
//        values.append(iterator.value().getValue());
//    }
//    item_values.insert(item_id,values);
//}

void SolverPCA::lookForCompletePlurals()
{



//    qDebug()<<"plurals count: "<<plurals.size();
    // цикл по всем сгенерированным множествам
    clock_t start;
    clock_t end;
    start = clock();
//    int plur_count = 0;
    long int all = 0;
    omp_set_num_threads(6);
#pragma omp parallel
    {
#pragma omp for
    for (int plural_ind = 0; plural_ind < plurals.size(); plural_ind ++)
    {
//        qDebug()<<"thread id: "<< omp_get_thread_num();
        alglib::real_2d_array dataset; // матрица данных для проведения pca
        alglib::real_2d_array res_dataset; // матрица нагрузок
        alglib::real_1d_array variars; // массив весов компонентов
        alglib::ae_int_t rows; // число строк в матрице данных
        alglib::ae_int_t cols; // число столбцов в матрице данных
        alglib::ae_int_t info;
        QVector<Sample*> analitic_samples;
        int analitic_count = 0;

        double *container;
        double first_component = 0; // нагрузка на первую компоненту
        double second_component = 0; // нагрузка на вторую компоненту
        double sum_w = 0; // сумма весов компонентов

        double **loading_matrix; // матрица нагрузок для подходящего набора
        QVector<double> vec_item_value;


        analitic_samples = samples_by_type.value(Names::analitic_id);
        analitic_count = analitic_samples.size();
        /*container = new double[size * analitic_count];*/// + size];
        rows = analitic_count;
        cols = size;
        info = 0;

        container = new double[size * analitic_count + size];
        //цикл по все пробам для сбора значений концентрации компонента
        for (int row_ind = 0; row_ind < analitic_count; row_ind ++)
        {
            // создать массив данных для проведения метода PCA
            // Container заполняется согласно тому как будет разбираться в real_2d_array.setcontainer()
            // цикл по количеству компонентов в наборе (особенность создания контейнера)
            for (int col_ind = 0; col_ind < size; col_ind ++)
            {
                vec_item_value = pickValues(plurals[plural_ind][col_ind], Names::analitic_id);
                container[row_ind * size + col_ind] = vec_item_value[row_ind];
//                item_value_iterator = item_values.find(plurals[plural_ind][col_ind]);
//                container[row_ind * size + col_ind] = item_value_iterator.value()[row_ind];
            }
        }

        QString str = "";
       // подготовка массива данных
       dataset.setcontent(rows, cols, container);
//       delete container;

            //метод PCA
            alglib::pcabuildbasis(dataset, rows, cols, info, variars, res_dataset);
#pragma omp atomic
            all ++;
            sum_w = 0;
            first_component = 0;
            second_component = 0;
            for (int i = 0; i < variars.length(); i ++)
            {
                sum_w += variars(i);
            }
            first_component = variars(0) / sum_w;
            second_component = variars(1) / sum_w;

            qDebug()<<"checked: "<<all;
            if ((first_component + second_component) > 0.9)
            {

//               plur_count ++;
//               qDebug()<<"complete count: "<< plur_count;

                // преобрахование матрицы нагрузок из типа alglib::real_2d_array в double**
                loading_matrix = new double*[res_dataset.rows()];
                for (int i = 0; i < variars.length(); i ++)
                           {
                               sum_w += variars(i);
                           }

                for (int i = 0; i < res_dataset.rows(); i ++)
                {
                    loading_matrix[i] = new double[res_dataset.cols()];
                    for (int j = 0; j < res_dataset.cols(); j ++)
                    {
                        loading_matrix[i][j] = res_dataset[i][j];
                    }
                }
#pragma omp critical
                {
                complete_plurals.insert(plurals[plural_ind],loading_matrix);
                }
//                getScores(plurals[plural_ind]);
            }// if ((first_component + second_component) > 0.9)
delete container;
    }// for (int plural_ind = 0; plural_ind < plurals.size(); plural_ind ++)
    }
    end = clock() - start;
    qDebug()<< "executing time: "<<(end / CLOCKS_PER_SEC);
    qDebug()<<"complete count: "<<complete_plurals.size();




}
void SolverPCA::standart()
{
    double *mass;
    QHash<QString,unsigned int>::iterator iterator;
    ItemInSample item_1;
    double average = 0; // среднее для компонента по анализируемым пробам (всегда, так определена стандартизация)
    double norm = 0; // дисперсия для значений компонента
    double res = 0; // стандартизированное значение
    int length = 0; // длина массива концентраций компонента для проб определенного типа
    QVector<double> vec_values;
    QVector<Sample*> water_samples;
    QHash<unsigned int, ItemInSample> *items_in_sample = new QHash<unsigned int, ItemInSample>();
    QHash<QString,unsigned int>::iterator water_id;

    pickSamples();
    getAnaliticAverage();

    for (water_id = Names::water_types->begin(); water_id != Names::water_types->end(); water_id ++)
    {
        water_samples = samples_by_type.value(water_id.value());
//        qDebug()<<"water_name: "<<water_id.key();
//        qDebug()<<"size: "<< water_samples.size();
        for (iterator = Names::params->begin(); iterator != Names::params->end(); iterator ++)
        {
//            qDebug()<<"name: "<< iterator.key();
            average = analitic_average->value(iterator.value());
            vec_values = pickValues(iterator.value(),water_id.value());
            length = vec_values.size();
            mass = new double [length];

            for (int i = 0; i < length; i ++)
            {
                mass[i] = vec_values[i];
            }

            // определение коэффициента нормирования
            norm = getAverageSquare(mass, average, mass, average, length);// / length;
//            qDebug()<<"cov: "<< sqrt(norm);

            //стандартизация концентраций компонента во всех пробах типа water_id
            for (int i = 0; i < water_samples.size(); i ++)
            {
                items_in_sample = water_samples[i]->getComponents();
                if (items_in_sample->contains(iterator.value()))
                {
                    // получение текущего значения концентрации и стандартизация его,
                    // после чего запись обратно в пробу
                    item_1 = items_in_sample->value(iterator.value());
                    res = (item_1.getValue() - average) / sqrt(norm);//
//                    qDebug()<<"res: "<<res;
                    item_1.setValue(res);
                    water_samples[i]->getComponents()->insert(iterator.value(),item_1);
                }
            }

        }// for (iterator = Names::params->begin(); iterator != Names::params->end(); iterator ++)
        samples_by_type.insert(water_id.value(),water_samples);

    } // for (water_id = Names::water_types->begin(); water_id != Names::water_types->end(); water_id ++)
}

void SolverPCA::getAnaliticAverage()
{
    double *mass; // массив значений концентраций для компонента
    QHash<QString,unsigned int>::iterator iterator;
//    ItemInSample item_1;
    double average = 0; // средняя концентрация для компонента по анализируемым пробам
    QVector<double> vec_values;
    int length = 0; // длина массива для нахождения среднего и диспесрисии
//    QHash<unsigned int, ItemInSample> *items_in_sample = new QHash<unsigned int, ItemInSample>();

    // для всех компонентов находится среднее по анализируемомму типу вод
    for (iterator = Names::params->begin(); iterator != Names::params->end(); iterator ++)
    {
        length = 0;
        average = 0;

        // получить все значения концентраций компонента для анализируемого типа вод
        vec_values = pickValues(iterator.value(),Names::analitic_id);
        length = vec_values.size();
        mass = new double [length];

        // преобразование вектора значений в массив
        for (int i = 0; i < length; i ++)
        {
            mass[i] = vec_values[i];
        }

        // определение среднего для компонента в пробах анализируемого типа
        average = getAverage(mass, length);
        qDebug()<<"item name: "<<iterator.key()<<" | item average: "<< average;
        analitic_average->insert(iterator.value(),average);
    }

}
void SolverPCA::setItems(QVector<Sample *> new_items)
{
    Sample *s;
    QHash<unsigned int, ItemInSample> *new_components;
    QHash<unsigned int, ItemInSample>::iterator iter;
    for (int i = 0; i < new_items.size(); i ++)
    {
        s = new Sample();
        new_components = new QHash<unsigned int, ItemInSample>();
        s->setId(new_items[i]->getId());
        s->setDate(new_items[i]->getDate());
        s->setLocationId(new_items[i]->getLocationId());
        s->setWaterId(new_items[i]->getWaterId());
        s->setComment(new_items[i]->getComment());
        s->setPosition(new_items[i]->getPosition());

        for (iter = new_items[i]->getComponents()->begin(); iter != new_items[i]->getComponents()->end(); iter ++)
        {
            ItemInSample i_s;
            i_s.setChanged(iter.value().getChanged());
            i_s.setItemId(iter.value().getItemId());
            i_s.setValue(iter.value().getValue());

            new_components->insert(iter.key(),i_s);
        }
        s->setComponents(new_components);
        items.append(s);
    }

    //    items = new_items;
}


QVector<double> SolverPCA::pickValues(unsigned int item_id, unsigned int water_id)
{
    QHash<unsigned int, ItemInSample>::iterator iterator;
    QVector<double> values;
    QVector<Sample*> water_samples;

    //получить все пробы для типа водной массы water_id
    water_samples = samples_by_type.value(water_id);

    //собрать все концентрации компонента item_id в пробах нужного типа

    for (int i = 0; i < water_samples.size(); i ++)
    {
        if (water_samples[i]->getComponents()->contains(item_id) == true)
        {
            iterator = water_samples[i]->getComponents()->find(item_id);
            values.append(iterator.value().getValue());
        }
        // ЭТО КОСТЫЛЬ, это заглушка на метод PCA при формировании dataset
        // на тот случай, когда компонент item_id есть не во всех пробах анализируемого типа
        else if(water_id == Names::analitic_id)
        {
            values.append(analitic_average->value(item_id));
        }
    }

    return values;
}
void SolverPCA::pickSamples()
{
    QHash<QString, unsigned int>::iterator water_iter;
    QVector<Sample*> water_samples;

    for(water_iter = Names::water_types->begin(); water_iter != Names::water_types->end(); water_iter ++)
    {
//        qDebug()<<"water _id: " << water_iter.key();
        for (int i = 0; i < items.size(); i ++)
        {

            if (items[i]->getWaterId() == water_iter.value())
            {
                water_samples.append(items[i]);
            }

        }
//        qDebug()<<"water_size: "<<water_samples.size();
        samples_by_type.insert(water_iter.value(),water_samples);
        water_samples.clear();
    }
}

QVector<SampleInfo> SolverPCA::getSamplesInfo(unsigned int *items_set, unsigned int water_id)
{
    double** scores_matrix; // матрица счетов для текущего набора. Для каждого источника своя матрица
    double** loads_matrix; // матрица нагрузок для текущего набора
    QVector<Sample*> samples; // проба для типа вод
    QMap<unsigned int, QVector<Sample*> >::iterator wt_iter;
    QVector<double> vec_item_value;
//    double *container;
    double first = 0; // первый столбец дял матрицы счетов
    double second = 0;// второй столбец дял матрицы счетов
    QString str = ""; // строка для вывода в дебаг
    QVector<SampleInfo> points; // список проб с информацией о дате, месте отбора, об источнике и положение пробы в координатах u1 и u2
    AnaliticPoints analitic_points;
//    SpringPoints spring_points;



    loads_matrix = complete_plurals.value(items_set);
    wt_iter = samples_by_type.find(water_id);
//    for (wt_iter = samples_by_type.begin(); wt_iter != samples_by_type.end(); wt_iter ++)
//    {
        samples = wt_iter.value();
        qDebug()<<"Samples.size(): "<<samples.size();

        // сбор информации о пробах
        for (int i = 0; i < samples.size(); i ++)
        {
            SampleInfo si;
            si.setDate(samples[i]->getDate());
            si.setLocationId(samples[i]->getLocationId());
            si.setWaterId(samples[i]->getWaterId());
            points.append(si);
        }
//        container = new double[size * samples.size() + size];
        scores_matrix = new double*[samples.size()];
//        loads_matrix = complete_plurals.find(items_set).value();
        double matrix[samples.size()][size];

        for (int row_ind = 0; row_ind < samples.size(); row_ind ++)
        {

            // создать массив данных для проведения метода PCA
            // Container заполняется согласно тому как будет разбираться в real_2d_array.setcontainer()
            // цикл по количеству компонентов в наборе (особенность создания контейнера)
            for (int col_ind = 0; col_ind < size; col_ind ++)
            {
                vec_item_value = pickValues(items_set[col_ind], water_id);

                matrix[row_ind][col_ind] = vec_item_value[row_ind];
            }
        }




         for (int i = 0; i < samples.size(); i ++)
         {
             scores_matrix[i] = new double [2];
             first = 0;
             second = 0;
             for (int j = 0; j < size; j ++)
             {
//                 qDebug()<<"matrix[i][j]:" << matrix[i][j];
//                 qDebug()<<"loads_matrix[j][0]: "<<loads_matrix[j][0];
//                 qDebug()<<"loads_matrix[j][1]: "<<loads_matrix[j][1];
                 first += matrix[i][j]*loads_matrix[j][0];
                 second += matrix[i][j]*loads_matrix[j][1];
             }
             scores_matrix[i][0] = first;
             scores_matrix[i][1] = second;
         }
         // запись координат точки
         for (int i = 0; i < samples.size(); i ++)
         {
             points[i].setU1(scores_matrix[i][0]);
             points[i].setU2(scores_matrix[i][1]);
         }
//         qDebug()<<"------------------------------------------";
//         qDebug()<<"loads";
//                  for (int i = 0; i < samples.size(); i ++)
//                  {
//                      str = "";
//                      for (int j = 0; j < size; j ++)
//                      {
//                          str += QString("%1 ").arg(loads_matrix[i][j]);
//                      }
//                      qDebug()<<str;
//                  }
//         qDebug()<<"Scores";
//         for (int i = 0; i < samples.size(); i ++)
//         {
//             str = "";
////             for (int j = 0; j < size; j ++)
////             {
//                 str = QString("%1 %2").arg(scores_matrix[i][0]).arg(scores_matrix[i][1]);
////             }
//             qDebug()<<str;
//         }
         return points;

//        for (int)
//    }

//    samples =
}
