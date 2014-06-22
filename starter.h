#ifndef STARTER_H
#define STARTER_H
#include <QMap>
#include <QString>

class Starter
{
    QMap<QString, unsigned int>* map;
public:
    Starter();
    ~Starter();
    unsigned int saveSampleSet(QString name);
    QMap<QString, unsigned int>* getSampleSets();
};

#endif // STARTER_H
