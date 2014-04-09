#ifndef SAMPLE_H
#define SAMPLE_H
#include <QDate>
#include <QHash>
#include <iteminsample.h>

class Sample
{
public:
    Sample();
    ~Sample();
    unsigned int getId();
    unsigned int getSampleSetId();
    unsigned int getPosition();
    unsigned int getLocationId();
    unsigned int getWaterId();
    QDate getDate();
    QString getComment();
    QHash<unsigned int, ItemInSample> *getComponents();

    void setId(unsigned int id);
    void setSampleSetId(unsigned int id);
    void setPosition(unsigned int position);
    void setLocationId(unsigned int location_id);
    void setWaterId(unsigned int water_id);
    void setDate(QDate date);
    void setComment(QString comment);
    void setComponents(QHash<unsigned int, ItemInSample> *components);

private:
    unsigned int id; // id пробы из БД
    unsigned int sample_set_id; // номер набора проби из БД (пока устанавливается вручную)
    unsigned int position; // номер строки пробы в системе
    unsigned int location_id;// id места отбора пробы из БД
    unsigned int water_id;
    QDate date; // дата отбора пробы
    QString comment; // комментарий к пробе

    QHash<unsigned int, ItemInSample> *components; // список параметров пробы
};

#endif // SAMPLE_H
