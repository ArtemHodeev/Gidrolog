#ifndef ALLTRIANGLES_H
#define ALLTRIANGLES_H
#include <QVector>


struct pair {   //координаты точки
    double x;
    double y;
};
struct triagnle_struct { //треугольник с координатами + число точек внутри треугольника
    pair A;
    pair B;
    pair C;
    int count_of_points;
 };

class AllTriangles
{
public:
    AllTriangles();
    //~AllTriangles();
    QVector <double> getBaseflowX() const;
    void setBaseflowX(const QVector<double> &baseflowX);
    QVector <double> getBaseflowY() const;
    void setBaseflowY(const QVector<double> &baseflowY);
    QVector <double> getSamplesX() const;
    void setSamplesX(const QVector<double> &samplesX);
    QVector <double> getSamplesY() const;
    void setSamplesY(const QVector<double> &samplesY);
    QVector <triagnle_struct> getTriangles();
    void setTriangles(const QVector<triagnle_struct> &triangles);
    QVector <pair> getMass();
    void generateSets(QVector<double> baseflowX, QVector<double> baseflowY);//генерируется вектор mass
    int getSizeTriangles();

private:
    QVector <double> baseflowX; //координаты источников по Х
    QVector <double> baseflowY; //координаты источников по Y
    QVector <double> samplesX;  //координаты анализируемого типа по Х
    QVector <double> samplesY;  //координаты анализируемого типа по Y
    QVector <triagnle_struct> triangles; //все сгенерированные треугольники
    QVector <pair> mass; // вектор пар координат источников
    pair pair_of_points;
//    triagnle_struct struct_of_triagnles;

 };

#endif // ALLTRIANGLES_H
