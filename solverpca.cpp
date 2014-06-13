#include "solverpca.h"
#include <QDebug>
#include <alglib/dataanalysis.h>

SolverPCA::SolverPCA()
{
    analitic_average = new QHash<unsigned int, double>();
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
    alglib::real_2d_array dataset; // матрица данных для проведения pca
    alglib::real_2d_array res_dataset; // матрица нагрузок
    alglib::real_1d_array variars; // массив весов компонентов
    alglib::ae_int_t rows; // число строк в матрице данных
    alglib::ae_int_t cols; // число столбцов в матрице данных
    alglib::ae_int_t info;
    QVector<Sample*> analitic_samples;
    int analitic_count = 0;
    double *container;
    QHash<unsigned int, QVector<double> >::iterator item_value_iterator;
    double first_component = 0; // нагрузка на первую компоненту
    double second_component = 0; // нагрузка на вторую компоненту
    double sum_w = 0; // сумма весов компонентов
    int plur_count = 0;
    QVector<double> vec_item_value;
//    QHash<unsigned int, QVector<double> > item_values;

//    item_values = pickValues()

    analitic_samples = samples_by_type.value(Names::analitic_id);
    analitic_count = analitic_samples.size();
    container = new double[size * analitic_count + size];
    rows = analitic_count;
    cols = size;
    info = 0;
    int score = 0;
//    int complete_count=0;


    // Собрать значеняи компонента в пробах в хэш, для того что бы каждый раз не собирать
    QHash<QString, unsigned int>::iterator iter;
    for (iter = Names::params->begin(); iter != Names::params->end(); iter ++)
    {
        pickItemValues(iter.value());
    }
    qDebug()<<"plurals count: "<<plurals.size();
    // цикл по всем сгенерированным множествам
    for (int plural_ind = 0; plural_ind < plurals.size(); plural_ind ++)
    {
        //цикл по все пробам для сбора значений концентрации компонента
        for (int row_ind = 0; row_ind < analitic_count; row_ind ++)
        {

            // создать массив данных для проведения метода PCA
            // Container заполняется согласно тому как будет разбираться в real_2d_array.setcontainer()
            // цикл по количеству компонентов в наборе (особенность создания контейнера)
            for (int col_ind = 0; col_ind < size; col_ind ++)
            {
                vec_item_value = pickValues(plurals[plural_ind][col_ind], Names::analitic_id);

//                if (row_ind >= vec_item_value.size())
//                qDebug()<<"name: "<<Names::params->key(plurals[plural_ind][col_ind]) << " value: " << vec_item_value[row_ind];

                score ++;
                container[row_ind * size + col_ind] = vec_item_value[row_ind];

//                item_value_iterator = item_values.find(plurals[plural_ind][col_ind]);
//                container[row_ind * size + col_ind] = item_value_iterator.value()[row_ind];
            }
        }
        QString str = "";
            // подготовка массива данных
        for (int i = 0; i < score; i ++)
        {
//                sum_w += variars(i);

//                str += QString("%1 ").arg(container[i]);
        }
//        qDebug()<<"container: "<< str;
        str = "";
            dataset.setcontent(rows, cols, container);
//            dataset.
//            qDebug()<< "score: "<< score;
            //метод PCA
//            qDebug()<<"dataset cols: "<< dataset.cols();
//            qDebug()<<"dataset rows: "<< dataset.rows();
//            qDebug()<<"dataset stride: "<< dataset.getstride();
//            qDebug()<<"dataset cols: "<< dataset.;
            alglib::pcabuildbasis(dataset, rows, cols, info, variars, res_dataset);


            sum_w = 0;
            first_component = 0;
            second_component = 0;
//            qDebug()<<"**********************";
            for (int i = 0; i < variars.length(); i ++)
            {
                sum_w += variars(i);
            }
            first_component = variars(0) / sum_w;
            second_component = variars(1) / sum_w;
//            qDebug()<<"info: "<<info;
//            qDebug()<<"var 0: "<<(double) variars(0);
//            qDebug()<<"var 1: "<<(double) variars(2);
//            qDebug() <<"sum: "<< QVariant(sum_w).toString();
//            qDebug()<<"first: "<<first_component;
//            qDebug()<<"second: "<<second_component;
//            qDebug()<<"power: "<<first_component + second_component;
            for (int i = 0; i < variars.length(); i ++)
            {
//                sum_w += variars(i);

//                str += QString("%1 ").arg(variars(i) / sum_w);
            }
//            qDebug()<<str;
            str = "";
//            qDebug()<<"======================";
            if ((first_component + second_component) > 0.9)
            {
                plur_count ++;
//                for (int i = 0; i < res_dataset.rows(); i ++)
//                {

//                    for (int j = 0; j < res_dataset.cols(); j ++)
//                    {
//                        str += QString("%1 ").arg(res_dataset(i,j));
//                    }
////                    qDebug()<<str;
//                    str = "";
//                }
            }
//            qDebug()<<"**********************";
    }
    qDebug()<<"complete count: "<<plur_count;
    delete []container;

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
    double *mass; // массив значений концентраций дял компонента
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
        if (water_samples[i]->getComponents()->contains(item_id))
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
