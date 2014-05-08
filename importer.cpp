#include "importer.h"
#include <QDebug>
#include <xlsx/xlsxcellrange.h>
#include <xlsx/xlsxcell.h>
#include <QSqlQuery>
#include <QSqlError>
#include <databaseaccessor.h>
#include <item.h>
#include <watertype.h>
#include <location.h>
#include <names.h>
#include <QDialog>
#include <confirmimport.h>
#include <QTableView>
#include <itemmodel.h>
#include <confirmwatertypemodel.h>
#include <confirmitemmodel.h>
#include <confirmlocationmodel.h>

Importer::Importer()
{
//    params = new QHash<QString, unsigned int>();
//    locations = new QHash<QString, unsigned int>();
//    water_types = new QHash<QString, unsigned int>();
    file = new QXlsx::Document();
}

Importer::Importer(QString file_path)
{
//    params = new QHash<QString, unsigned int>();
//    locations = new QHash<QString, unsigned int>();
//    water_types = new QHash<QString, unsigned int>();
    file = new QXlsx::Document(file_path);
}
Importer::~Importer()
{
//    delete params;
//    delete locations;
//    delete water_types;
}

/*
 *F:
 *   bool Importer::confirm()
 * I:
 *  --
 * O:
 *  true - если пользователь подтвердил сохранение неизвестных параметров.
 *  false - если пользователь нажал отмену
 * D:
 *  Функция предоставляет диалог с пользователем для подтверждения сохранения новых параметров полученных из файла.
 *  Если приимпорте не встретился ни один параметр, которые неизвестен системе то диалога не вызывается.
 */
bool Importer::confirm()
{
    QDialog *dlg = new ConfirmImport();// окно диалога с пользователем
    ConfirmImport *c_i = new ConfirmImport(); //
    bool canceled = false; // Содержит флаг: была ли произведена отмена действий импорта проб из файла
    bool sign = false; // флаг: есть модель для отображения, т.е. есть параметры неизвестные системе

    // если встретились неизвестные системе компоненты
    if (unknow_params.isEmpty() != true)
    {
        QVector<Item*> p; // вектор компонентов, элементы которого - объекты типа Item
        QHash<QString, unsigned int>::iterator i; // итератор по неизвестным компонентам

        // Создание для каждого неизвестного системе компонента соответствующие объект Item
        for ( i = unknow_params.begin(); i != unknow_params.end(); i ++)
        {
            Item *it = new Item();
            it->setName(i.key());
            p.append(it);
        }
        // создание наполнение модели данных параметров, которая будет отображаться в диалоге
        ConfirmItemModel *model = new ConfirmItemModel();
        model->setItems(p);
        c_i->setParamModel(model);

//        delete model;
        sign = true;
    }

    // если встретились неизвестные системе типы водных масс
    if (unknow_water_types.isEmpty() != true)
    {
        QVector<WaterType*> p; // вектор типов водных масс, элементы которого - объекты WaterType
        QHash<QString, unsigned int>::iterator i; // итератор по неизвестным системе типов водных масс

        // Создание для каждого неизвестного системе типа водной массы соответствующие объект WaterType
        for ( i = unknow_water_types.begin(); i != unknow_water_types.end(); i ++)
        {
            WaterType *wt = new WaterType();
            wt->setName(i.key());
            p.append(wt);
        }

        // создание наполнение модели данных типов водных масс, которая будет отображаться в диалоге
        ConfirmWaterTypeModel *model = new ConfirmWaterTypeModel();
        model->setItems(p);
        c_i->setWaterModel(model);

//        delete model;
        sign = true;
    }

    // если встретились неизвестные системе места отбора
    if (unknow_locations.isEmpty() != true)
    {
        QVector<Location*> l;// вектор мест отбора, элементы которого - объекты Location
        QHash<QString, unsigned int>::iterator i; // итератор по неизвестным системе мест взятия

        // Создание для каждого неизвестного системе места взятия соответствующие объект Location
        for ( i = unknow_locations.begin(); i != unknow_locations.end(); i ++)
        {
            Location* loc = new Location();
            loc->setName(i.key());
            l.append(loc);
        }

        // создание наполнение модели данных мест взятия, которая будет отображаться в диалоге
        ConfirmLocationModel *model = new ConfirmLocationModel();
        model->setItems(l);
        c_i->setLocationModel(model);
        sign = true;
//        delete model;
    }
    // если есть модель для отображения, то открыть диалог
    if (sign == true)
    {
        dlg = c_i;
        dlg->exec();

        // определение действия пользователя на дальнейшие дейстия с неизвестными параметрами: отменить импорт или сохранить
        canceled = c_i->isCanceled();
    }

    delete dlg;
    return canceled;
//    delete c_i;
}

/*
 *F:
 *   void Importer::createParamsFromFile()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция определяет для каждого компонента в системе его id из БД и создает хэш из этих данных
 */
void Importer::createParamsFromFile()
{
    for (int i = 5; i <= file->dimension().columnCount(); i ++)
    {
        QString param_name = file->read(1,i).toString();
        params_from_file.insert(i,Names::params->value(param_name));
    }
}

/*
 *F:
 *   QVector<Sample*> Importer::import()
 * I:
 *  --
 * O:
 *  Вектор элементов Sample, который содержит в себе все пробы из файла
 * D:
 *  Функция определяет неизвестные системе параметры, считывает данные из файла и сосдает пробы на основе считанных данных из файла
 */
QVector<Sample*> Importer::import()
{
    //Поиск неизвестных системе параметров
    checkHeaders();
    checkWaterTypes();
    checkLocations();
    QVector<Sample*> s;

    bool sign = confirm();

    // Если пользователь подтвердил созранение неизвестных параметров
    if (sign != true)
    {
        createParamsFromFile();

        // Считывание строк из файла и создание для каждой строки объект Sample
        // считывает со второй строки файла, т.к. первая - это заголовки
        for (int i = 2; i <= file->dimension().rowCount(); i ++)
        {
            Sample *s1 = getSample(i);
            s.append(s1);
        }
    }
    return s;
}

/*
 *F:
 *   Sample* Importer::getSample(int num)
 * I:
 *  int num - номер строки для которой из которой нужно стделать объект Sample
 * O:
 *  Возвращается адрес объекта Sample для строки num
 * D:
 *  Функция считывает строку num из файла и создает на ее основе объект типа Sample* и возвращает его
 */
Sample* Importer::getSample(int num)
{
    Sample *s = new Sample();

    QString water_name;
    unsigned int water_id;
    QString location_name;
    unsigned int location_id;

    // Идентификация места взятия
    location_name = file->read(num,1).toString();
    location_id = Names::locations->value(location_name);

    // Идентификация типа водной массы
    water_name = file->read(num,3).toString();
    water_id = Names::water_types->value(water_name);

    // Инициализация пробы - объекта Sample
    s->setLocationId(location_id);
    s->setDate(file->read(num,2).toDate());
    s->setWaterId(water_id);
    s->setComment(file->read(num,4).toString());
    s->setPosition(num - 2);

    QHash<unsigned int, ItemInSample> *components = new  QHash<unsigned int, ItemInSample>();
    QHash<unsigned int, unsigned int>::iterator param_iterator; // первый аргумент - номер параметра из файла; второй - id параметра в БД

    // Определение параметров пробы
    for (int i = 5; i <= file->dimension().columnCount(); i ++)
    {
        ItemInSample item_in_sample;
        double val = file->read(num,i).toDouble();

        param_iterator = params_from_file.find(i);
        item_in_sample.setItemId(param_iterator.value());

        //Если парметр определялся в пробе, то его следует сохранить в пробе
        if (val >= 0)
        {
            item_in_sample.setValue(val);
            item_in_sample.setChanged(1);
        }

        components->insert(param_iterator.value(), item_in_sample);
    }

    s->setComponents(components);
    return s;
}

//// Возможно уже не нужна
//void Importer::setParams(QHash<QString,unsigned int> *new_params)
//{
//    QHash<QString, unsigned int>::iterator i;
//    for (i = new_params->begin(); i != new_params->end(); i ++)
//    {
//        params->insert(i.key(),i.value());
//    }
//}
//// Возможно уже не нужна
//void Importer::setWaterTypes(QHash<QString, unsigned int> *new_types)
//{
//    QHash<QString, unsigned int>::iterator i;
//    for (i = new_types->begin(); i != new_types->end(); i ++)
//    {
//        water_types->insert(i.key(),i.value());
//    }
//}
//// Возможно уже не нужна
//void Importer::setLocations(QHash<QString, unsigned int> *new_locations)
//{
//    QHash<QString, unsigned int>::iterator i;
//    for (i = new_locations->begin(); i != new_locations->end(); i ++)
//    {
//        locations->insert(i.key(),i.value());
//    }
//}

/*
 *F:
 *   void Importer::checkHeaders()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция осуществляет проверку параметров(компонентов) пробы, если встретился неизвестный системе компонент, то запоминается
 *  он и его место.
 */
void Importer::checkHeaders()
{
    // Сравнение всех заголовков из файла с заголовками системы
    // сравнение происходит с пяго заголовка, т.к. первые четыре считаются неизменными и их порядок не меняется.
    for (int i = 5; i <= file->dimension().columnCount(); i ++)
    {
        QString param_name = file->read(1,i).toString();
        if (Names::params->contains(param_name) == false)
        {
            unknow_params.insert(param_name,i);
        }
    }
}

/*
 *F:
 *   void Importer::checkWaterTypes()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция осуществляет проверку типов водных масс, если встретился неизвестный системе тип, то запоминается
 *  он и его место (место излишне).
 */
void Importer::checkWaterTypes()
{
    // Сравнение всех типов водных масс из файла с типами, известными системе
    // Считываение начинается со второй строки, т.к. первая строка - это заголовки столбцов
    // Считываются данные только третьего столбца, т.к. считается, что положение типа водной массы статично и не меняется
    for (int i = 2; i <= file->dimension().rowCount(); i ++)
    {
        QString water_type_name = file->read(i,3).toString();

        if (Names::water_types->contains(water_type_name) != true)
        {
            unknow_water_types.insert(water_type_name,i);
        }
    }
}

/*
 *F:
 *   void Importer::checkLocations()
 * I:
 *  --
 * O:
 *  --
 * D:
 *  Функция осуществляет проверку мест взятия, если встретилось неизвестное системе место взяти, то запоминается
 *  его название и его место (место излишне).
 */
void Importer::checkLocations()
{
    // Сравнение всех мест взятия из файла с местами взятия, известными системе
    // Считываение начинается со второй строки, т.к. первая строка - это заголовки столбцов
    // Считываются данные только первого столбца, т.к. считается, что положение места взятия ы статично и не меняется
    for (int i = 2; i <= file->dimension().rowCount(); i ++)
    {
        QString location_name = file->read(i,1).toString();

        if (Names::locations->contains(location_name) != true)
        {
            unknow_locations.insert(location_name,i);
        }

    }
}
