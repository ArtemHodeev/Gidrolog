#include "alltriangles.h"
#include <QDebug>
#include <math.h>
#include "gentriangles.h"
#include "names.h"
#include <cmath>


AllTriangles::AllTriangles()
{
//    for (int i=0;i<10;i++)
//    {
//        baseflowX.append(rand()%11);
//        baseflowY.append(rand()%12);
//    }

//    SampleInfo new_samples;
//    for (int i=0;i<10;i++)
//    {
//        new_samples.setU1(i+1);
//        new_samples.setU2(i+3);
//        samplesX.append(new_samples);
//        samplesY.append(new_samples);
//    }

//    SampleInfo new_baseflow;
//    new_baseflow.setU1(5);
//    new_baseflow.setU2(5);
//    new_baseflow.setWaterId(1);
//    new_baseflow.setLocationId(1);
//    new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//    baseflowX.append(new_baseflow);

//    new_baseflow.setU1(5);
//    new_baseflow.setU2(5);
//    new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//    new_baseflow.setLocationId(1);
//    new_baseflow.setWaterId(1);
//    baseflowY.append(new_baseflow);

//    new_baseflow.setU1(10);
//    new_baseflow.setU2(10);
//    new_baseflow.setWaterId(1);
//    new_baseflow.setLocationId(1);
//    new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//    baseflowX.append(new_baseflow);

//    new_baseflow.setU1(15);
//    new_baseflow.setU2(15);
//    new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//    new_baseflow.setLocationId(1);
//    new_baseflow.setWaterId(1);
//    baseflowY.append(new_baseflow);

//    new_baseflow.setU1(15);
//    new_baseflow.setU2(15);
//    new_baseflow.setWaterId(1);
//    new_baseflow.setLocationId(1);
//    new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//    baseflowX.append(new_baseflow);

//    new_baseflow.setU1(5);
//    new_baseflow.setU2(5);
//    new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//    new_baseflow.setLocationId(1);
//    new_baseflow.setWaterId(1);
//    baseflowY.append(new_baseflow);

//    new_baseflow.setU1(15);
//    new_baseflow.setU2(15);
//    new_baseflow.setWaterId(1);
//    new_baseflow.setLocationId(1);
//    new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//    baseflowX.append(new_baseflow);

//    new_baseflow.setU1(15);
//    new_baseflow.setU2(15);
//    new_baseflow.setDate(QDateTime::fromString("2014-05-12 00:12", "yyyy-MM-dd hh:mm"));
//    new_baseflow.setLocationId(1);
//    new_baseflow.setWaterId(1);
//    baseflowY.append(new_baseflow);

//    pair pair_of_points;
//    pair_of_points.x = 0;
//    pair_of_points.y = 0;


//    struct_of_triagnles.A.x = 0;
//    struct_of_triagnles.A.y = 0;
//    struct_of_triagnles.B.x = 0;
//    struct_of_triagnles.B.y = 0;
//    struct_of_triagnles.C.x = 0;
//    struct_of_triagnles.C.y = 0;
//    struct_of_triagnles.count_of_points = 0;
}

QVector<double> AllTriangles::getBaseflowX() const
{
    QVector <double> temp_BaseflowX;
    for (int i=0; i<getBaseflowSize(); i++)
    {
        temp_BaseflowX.append(baseflowX[i].getU1());
    }
    return temp_BaseflowX;
}

void AllTriangles::setBaseflowX(const QVector<SampleInfo> &baseflowX)
{
    this->baseflowX = baseflowX;
}
QVector<double> AllTriangles::getBaseflowY() const
{
    QVector <double> temp_BaseflowY;
    for (int i=0; i<getBaseflowSize(); i++)
    {
        temp_BaseflowY.append(baseflowY[i].getU2());
    }
    return temp_BaseflowY;
}
void AllTriangles::setBaseflowY(const QVector<SampleInfo> &baseflowY)
{
    this->baseflowY = baseflowY;
}
QVector <double> AllTriangles::getSamplesX() const
{
    QVector <double> temp_SamplesX;
    for (int i=0; i<getSamplesSize(); i++)
    {
        temp_SamplesX.append(samplesX[i].getU1());
    }
    return temp_SamplesX;
}
void AllTriangles::setSamplesX(const QVector<SampleInfo> &samplesX)
{
    this->samplesX = samplesX;
}
QVector <double> AllTriangles::getSamplesY() const
{
    QVector <double> temp_SamplesY;
    for (int i=0; i<getSamplesSize(); i++)
    {
        temp_SamplesY.append(samplesY[i].getU2());
    }
    return temp_SamplesY;
}
void AllTriangles::setSamplesY(const QVector<SampleInfo> &samplesY)
{
    this->samplesY = samplesY;
}
QVector <triagnle_struct> AllTriangles::getTriangles()
{
    return triangles;
}
QVector <SampleInfo> AllTriangles::getMass()
{
    return mass;
}
void AllTriangles::generateSets(QVector<SampleInfo> baseflowX, QVector<SampleInfo> baseflowY)
{
    for (int i=0; i<baseflowX.size(); i++)
    {
        pair_of_points.setU1(baseflowX.at(i).getU1());
        pair_of_points.setU2(baseflowY.at(i).getU2());
        pair_of_points.setDate(baseflowX.at(i).getDate());
        pair_of_points.setLocationId(baseflowX.at(i).getLocationId());
        pair_of_points.setWaterId(baseflowX.at(i).getWaterId());

        mass.append(pair_of_points);
    }

    GenTriangles tri(mass, samplesX, samplesY);

    tri.GenerationTreangles(3);
    triangles = tri.getResult();
}
void AllTriangles::setTriangles(const QVector<triagnle_struct> &triangles)
{
    this->triangles = triangles;
}
int AllTriangles::getSizeTriangles()
{
    const int s = triangles.size();
    return s;
}
int AllTriangles::getBaseflowSize() const
{
    return baseflowX.size();
}
int AllTriangles::getSamplesSize() const
{
    return samplesX.size();
}
QVector <SampleInfo> AllTriangles::getInfoSamplesX() const
{
    return samplesX;
}
QVector <SampleInfo> AllTriangles::getInfoSamplesY() const
{
    return samplesY;
}
QVector <SampleInfo> AllTriangles::getInfoBaseflowX() const
{
    return baseflowX;
}
QVector <SampleInfo> AllTriangles::getInfoBaseflowY() const
{
    return baseflowY;
}
QString AllTriangles::is_baseflow_point(double x , double y) const
{
QString str = "";
    for (int i=0; i<getBaseflowSize(); i++)
    {
        if (
                (std::abs(getInfoBaseflowX().at(i).getU1() - x) < 0.03)
             && (std::abs(getInfoBaseflowY().at(i).getU2() - y) < 0.03)
           )
        {
            str += QString("(%1; %2)\n Источник: %3")
                    .arg(getInfoBaseflowX().at(i).getU1())
                    .arg(getInfoBaseflowY().at(i).getU2())
                    .arg(Names::water_types->key(getInfoBaseflowX().at(i).getWaterId()));
            return str;
        }
    }
    return "";
}
QString AllTriangles::is_analytic_point(double x, double y) const
{
    QString str = "";
        for (int i=0; i<getSamplesSize(); i++)
        {
            if (
                    (std::abs(getInfoSamplesX().at(i).getU1() - x) <  0.03)
                 && (std::abs(getInfoSamplesY().at(i).getU2() - y) <  0.03)
                )
            {
//                qDebug() << "getInfoSamplesX().at(i).getU1() - x" << abs(getInfoSamplesX().at(i).getU1() - x);
//                qDebug() << "getInfoSamplesY().at(i).getU2() - y" << abs(getInfoSamplesY().at(i).getU2() - y);

                str += QString(" (%1; %2)\n Дата: %3\n Место: %4\n Источник: %5")
                        .arg(getInfoSamplesX().at(i).getU1())
                        .arg(getInfoSamplesY().at(i).getU2())
                        .arg(
                            getInfoSamplesX().at(i).getDate().toString(),
                            Names::locations->key(this->getInfoSamplesX().at(i).getLocationId()),
                            Names::water_types->key(this->getInfoSamplesX().at(i).getWaterId()));
                return QString(str);
            }
        }
        return QString("");
}
