#include "analiticpoints.h"

AnaliticPoints::AnaliticPoints()
{
}
AnaliticPoints::~AnaliticPoints()
{
    samples.clear();
}
QString AnaliticPoints::getNameU1() const
{
    return u_1_name;
}
QString AnaliticPoints::getNameU2() const
{
    return u_2_name;
}
unsigned int AnaliticPoints::getWaterType() const
{
    return water_type;
}
QVector<SampleInfo> AnaliticPoints::getSamples() const
{
    return samples;
}

void AnaliticPoints::setNameU1(QString u_2_name)
{
    this->u_1_name = u_1_name;
}
void AnaliticPoints::setNameU2(QString u_1_name)
{
    this->u_2_name = u_2_name;
}
void AnaliticPoints::setWaterType(unsigned int water_type)
{
    this->water_type = water_type;
}
void AnaliticPoints::setSamples(QVector<SampleInfo> new_sample)
{
    for (int i = 0; i < new_sample.size(); i ++)
    {
        SampleInfo s_i;
        s_i.setDate(new_sample[i].getDate());
        s_i.setLocationId(new_sample[i].getLocationId());
        s_i.setU1(new_sample[i].getU1());
        s_i.setU2(new_sample[i].getU2());
        s_i.setWaterId(new_sample[i].getWaterId());

        samples.append(s_i);
    }
}
