#ifndef SAMPLEINFO_H
#define SAMPLEINFO_H
#include <QDateTime>
class SampleInfo
{
public:
    SampleInfo();
    SampleInfo(const SampleInfo &other);
    ~SampleInfo();
    void setDate(QDateTime date);
    void setLocationId(unsigned int location_id);
    void setWaterId(unsigned int water_id);
    void setU1(double u_1);
    void setU2(double u_2);
    QDateTime getDate() const;
    unsigned int getLocationId() const;
    unsigned int getWaterId() const;
    double getU1() const;
    double getU2() const;

private:
    QDateTime date;
    unsigned int location_id;
    unsigned int water_id;
    double u_1;
    double u_2;

};

#endif // SAMPLEINFO_H
