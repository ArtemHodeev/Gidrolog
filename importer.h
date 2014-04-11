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
    //  QHash нужен для того, чтобы знать какие параметры из файла пользователь не хочет
    //  добавлять в систему
    QXlsx::Document *file;

public:
    Importer();
    Importer(QString file_path);
    ~Importer();
    void setParams(QHash<QString,unsigned int> *new_params);
    void setLocations(QHash<QString, unsigned int> *locations);
    void setWaterTypes(QHash<QString, unsigned int> *water_types);
    void import();
    Sample *getSample();
private:
    void checkHeaders();
    void checkLocations();
    void checkWaterTypes();

//    QVector<QString> getValue();

};

#endif // Importer_H
