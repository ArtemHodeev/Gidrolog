#include "sampleinfo.h"

SampleInfo::SampleInfo()
{
    location_id = 0;
    water_id = 0;
    u_1 = 0;
    u_2 = 0;
}

SampleInfo::SampleInfo(const SampleInfo &other)
{
    this->date = other.getDate();
    this->location_id = other.getLocationId();
    this->water_id = other.getWaterId();
    this->u_1 = other.getU1();
    this->u_2 = other.getU2();
}

SampleInfo::~SampleInfo()
{

}
void SampleInfo::setDate(QDateTime date)
{
    this->date = date;
}
void SampleInfo::setLocationId(unsigned int location_id)
{
    this->location_id = location_id;
}
void SampleInfo::setWaterId(unsigned int water_id)
{
    this->water_id = water_id;
}
void SampleInfo::setU1(double u_1)
{
    this->u_1 = u_1;
}
void SampleInfo::setU2(double u_2)
{
    this->u_2 = u_2;
}
QDateTime SampleInfo::getDate() const
{
    return date;
}
unsigned int SampleInfo::getLocationId() const
{
    return location_id;
}
unsigned int SampleInfo::getWaterId() const
{
    return water_id;
}
double SampleInfo::getU1() const
{
    return u_1;
}
double SampleInfo::getU2() const
{
    return u_2;
}
