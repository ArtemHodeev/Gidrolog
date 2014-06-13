#include "alltriangles.h"
#include <QDebug>
#include <math.h>
#include "gentriangles.h"


AllTriangles::AllTriangles()
{
//    for (int i=0;i<10;i++)
//    {
//        baseflowX.append(rand()%11);
//        baseflowY.append(rand()%12);
//    }

     for (int i=0;i<10;i++)
    {
        samplesX.append(i+10);
        samplesY.append(i+10);
    }

     baseflowX.append(5);
     baseflowY.append(5);
     baseflowX.append(10);
     baseflowY.append(15);
     baseflowX.append(15);
     baseflowY.append(5);
     baseflowX.append(15);
     baseflowY.append(15);

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

    generateSets(baseflowX, baseflowY);

}

QVector<double> AllTriangles::getBaseflowX() const
{
    return baseflowX;
}

void AllTriangles::setBaseflowX(const QVector<double> &baseflowX)
{
    this->baseflowX = baseflowX;
}
QVector <double> AllTriangles::getBaseflowY() const
{
    return baseflowY;
}
void AllTriangles::setBaseflowY(const QVector<double> &baseflowY)
{
    this->baseflowY = baseflowY;
}
QVector <double> AllTriangles::getSamplesX() const
{
    return samplesX;
}
void AllTriangles::setSamplesX(const QVector<double> &samplesX)
{
    this->samplesX = samplesX;
}
QVector <double> AllTriangles::getSamplesY() const
{
    return samplesY;
}
void AllTriangles::setSamplesY(const QVector<double> &samplesY)
{
    this->samplesY = samplesY;
}
QVector <triagnle_struct> AllTriangles::getTriangles()
{
    return triangles;
}
QVector <pair> AllTriangles::getMass()
{
    return mass;
}
void AllTriangles::generateSets(QVector<double> baseflowX, QVector<double> baseflowY)
{
    for (int i=0; i<baseflowX.size(); i++)
    {
        pair_of_points.x = baseflowX.at(i);
        pair_of_points.y = baseflowY.at(i);
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

