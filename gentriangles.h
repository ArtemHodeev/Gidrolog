#ifndef GENTRIANGLES_H
#define GENTRIANGLES_H
#include "alltriangles.h"


class GenTriangles
{
public:
    GenTriangles(QVector <SampleInfo> &massive_pair, QVector <SampleInfo> &sX, QVector <SampleInfo> &sY);
    ~GenTriangles();
    void GenerationTreangles(int count); //заполняем вектор triangles треугольниками
    QVector <triagnle_struct> getResult();
private:
    void GetCountOfPoints(QVector<triagnle_struct> &triangles, QVector <SampleInfo> X, QVector <SampleInfo> Y); //считаем число точек внутри треугольников
    void TreanglesSort(QVector<triagnle_struct> &triangles);
    void genPlural(int count, int pos);
    void print(int k);
    bool isPointInTriangle(triagnle_struct abc, double Px, double Py); //определяется принадлежность точки треугольнику
    QVector <triagnle_struct> triangles; //все сгенерированные треугольники
    SampleInfo *temp_triangle; // временный массив пар координат треугольника. Добавляется в triangles
    int num; // размер генерируемых множеств
    QVector <SampleInfo> mass; // вектор пар координат источников
    QVector <SampleInfo> samplesX;
    QVector <SampleInfo> samplesY;
};

#endif // GENTRIANGLES_H
