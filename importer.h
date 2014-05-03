#ifndef Importer_H
#define Importer_H
#include <QHash>
#include <sample.h>
#include <xlsx/xlsxdocument.h>
#include <QVector>

class Importer
{
//    Q_OBJECT
    QHash<QString, unsigned int> *params; // параметры проб известные системе // возможно не нужен
    QHash<QString, unsigned int> *locations; // места отбора проб известные системе // возможно не нужен
    QHash<QString, unsigned int> *water_types; // типы водных масс известные системе // возможно не нужен
    QHash<QString, unsigned int> unknow_params; // неизвестные системе параметров.
    QHash<QString, unsigned int> unknow_locations; // неизвестные системе места отбора // достаточно QVector
    QHash<QString, unsigned int> unknow_water_types; // неизвестные системе типы водных масс // достаточно QVector
    QHash<unsigned int, unsigned int> params_from_file; // параметры из файла.
    QHash<unsigned int, unsigned int> water_types_from_file; // типы водных масс из файла // не нужен

//      Первый праметр - номер параметра в файле, второй - id параметра в БД

    QXlsx::Document *file; // файл с пробами

public:
    Importer();
    Importer(QString file_path);
    ~Importer();
    void setParams(QHash<QString,unsigned int> *new_params);
    void setLocations(QHash<QString, unsigned int> *new_locations);
    void setWaterTypes(QHash<QString, unsigned int> *new_types);
    QVector<Sample*> import();
    Sample *getSample(int num);
//    Sample *getSample();

private:
    void checkHeaders();
    void checkLocations();
    void checkWaterTypes();
    bool confirm();
    void createParamsFromFile();

//    QVector<QString> getValue();

};

#endif // Importer_H
