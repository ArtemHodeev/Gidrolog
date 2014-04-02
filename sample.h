#ifndef SAMPLE_H
#define SAMPLE_H
#include <QDate>
#include <QHash>

class Sample
{
public:
    Sample();
    ~Sample();
    unsigned int getId();
    unsigned int getSampleSetId();
    QDate getDate();
    unsigned int getLocationId();
    QString getComment();
    QHash<QString, double> *getComponents();
    void setId(unsigned int id);
    void setSampleSetId(unsigned int id);
    void setDate(QDate date);
    void setLocationId(unsigned int location_id);
    void setComment(QString comment);
    void setComponents(QHash<QString, double> *components);
private:
    unsigned int id;
    unsigned int sample_set_id;
    QDate date;
    unsigned int location_id;
    QString comment;

    QHash<QString, double> *components;
};

#endif // SAMPLE_H
