#ifndef GENTRIANGLES_H
#define GENTRIANGLES_H
#include "alltriangles.h"


class GenTriangles
{
public:
    GenTriangles(QVector <pair> &massive_pair, QVector <double> &sX, QVector <double> &sY);
    ~GenTriangles();
    void GenerationTreangles(int count); //заполняем вектор triangles треугольниками
    QVector <triagnle_struct> getResult();
private:
    void GetCountOfPoints(QVector<triagnle_struct> &triangles, QVector <double> X, QVector <double> Y); //считаем число точек внутри треугольников
    void TreanglesSort(QVector<triagnle_struct> &triangles);
    void genPlural(int count, int pos);
    void print(int k);
    bool isPointInTriangle(triagnle_struct abc, double Px, double Py); //определяется принадлежность точки треугольнику
    QVector <triagnle_struct> triangles; //все сгенерированные треугольники
    pair *temp_triangle; // временный массив пар координат треугольника. Добавляется в triangles
    int num; // размер генерируемых множеств
    QVector <pair> mass; // вектор пар координат источников
    QVector <double> samplesX;
    QVector <double> samplesY;
};

#endif // GENTRIANGLES_H
