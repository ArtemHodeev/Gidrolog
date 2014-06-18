#ifndef ALLTRIANGLES_H
#define ALLTRIANGLES_H
#include <QVector>
#include <sampleinfo.h>


//struct pair {   //координаты точки
//    double x;
//    double y;
//};
struct triagnle_struct { //треугольник с координатами + число точек внутри треугольника
    SampleInfo A;
    SampleInfo B;
    SampleInfo C;
    int count_of_points;
 };

class AllTriangles
{
public:
    AllTriangles();
    //~AllTriangles();
    QVector<double> getBaseflowX() const;
    void setBaseflowX(const QVector<SampleInfo> &baseflowX);
    QVector <double> getBaseflowY() const;
    void setBaseflowY(const QVector<SampleInfo> &baseflowY);
    QVector <double> getSamplesX() const;
    void setSamplesX(const QVector<SampleInfo> &samplesX);
    QVector <double> getSamplesY() const;
    void setSamplesY(const QVector<SampleInfo> &samplesY);
    QVector <triagnle_struct> getTriangles();
    QVector <SampleInfo> getInfoSamplesX() const;
    QVector <SampleInfo> getInfoSamplesY() const;
    QVector <SampleInfo> getInfoBaseflowX() const;
    QVector <SampleInfo> getInfoBaseflowY() const;
    void setTriangles(const QVector<triagnle_struct> &triangles);
    QVector <SampleInfo> getMass();
    void generateSets(QVector<SampleInfo> baseflowX, QVector<SampleInfo> baseflowY);//генерируется вектор mass
    int getSizeTriangles();
    int getBaseflowSize() const;
    int getSamplesSize() const;

private:
    QVector <SampleInfo> baseflowX; //координаты источников по Х
    QVector <SampleInfo> baseflowY; //координаты источников по Y
    QVector <SampleInfo> samplesX;  //координаты анализируемого типа по Х
    QVector <SampleInfo> samplesY;  //координаты анализируемого типа по Y
    QVector <triagnle_struct> triangles; //все сгенерированные треугольники
    QVector <SampleInfo> mass; // вектор пар координат источников
    SampleInfo pair_of_points;
//    triagnle_struct struct_of_triagnles;

 };

#endif // ALLTRIANGLES_H
