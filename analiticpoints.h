#ifndef ANALITICPOINTS_H
#define ANALITICPOINTS_H
#include <QString>
#include <sampleinfo.h>
#include <QVector>


class AnaliticPoints
{
    QString u_1_name;
    QString u_2_name;
    unsigned int water_type;
    QVector<SampleInfo> samples;

public:
    AnaliticPoints();
    ~AnaliticPoints();
    void setNameU1(QString u_2_name);
    void setNameU2(QString u_1_name);
    void setWaterType(unsigned int water_type);
    void setSamples(QVector<SampleInfo> new_sample);
    QString getNameU1() const;
    QString getNameU2() const;
    unsigned int getWaterType() const;
    QVector<SampleInfo> getSamples() const;
};

#endif // ANALITICPOINTS_H
