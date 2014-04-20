#ifndef Importer_H
#define Importer_H
#include <QHash>
#include <sample.h>
#include <xlsx/xlsxdocument.h>
#include <QVector>

class Importer
{
    QHash<QString, unsigned int> *params; // параметры проб известные системе
    QHash<QString, unsigned int> *locations; // места отбора проб известные системе
    QHash<QString, unsigned int> *water_types; // типы водных масс известные системе
    QHash<QString, unsigned int> unknow_params; // неизвестные системе параметров.
    QHash<QString, unsigned int> unknow_locations; // неизвестные системе места отбора
    QHash<QString, unsigned int> unknow_water_types; // неизвестные системе типы водных масс
    QHash<unsigned int, unsigned int> params_from_file; // параметры из файла.
//      Первый праметр - номер параметра в файле, второй - id параметра в БД

    QXlsx::Document *file;

public:
    Importer();
    Importer(QString file_path);
    ~Importer();
    void setParams(QHash<QString,unsigned int> *new_params);
    void setLocations(QHash<QString, unsigned int> *locations);
    void setWaterTypes(QHash<QString, unsigned int> *water_types);
    QVector<Sample*> import();
    Sample *getSample(int num);
//    Sample *getSample();

private:
    void checkHeaders();
    void checkLocations();
    void checkWaterTypes();

//    QVector<QString> getValue();

};

#endif // Importer_H
