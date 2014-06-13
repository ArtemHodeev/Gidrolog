#include "gentriangles.h"
#include <QDebug>
#include <math.h>
#include <alltriangles.h>


bool prefLessThan(const triagnle_struct &p1, const triagnle_struct &p2)
{
    return p1.count_of_points > p2.count_of_points;
}

GenTriangles::GenTriangles(QVector <pair> &massive_pair, QVector <double> &sX, QVector <double> &sY)
{
    mass =  massive_pair;
    samplesX = sX;
    samplesY = sY;
}
GenTriangles::~GenTriangles()
{
}
void GenTriangles::GenerationTreangles(int count)
{
    temp_triangle = new pair[count];
//    mass = item_list;
    num = count;
    genPlural(count, 0);

    qDebug()<<"plurals size: "<< triangles.size();
    delete []temp_triangle;

    GetCountOfPoints(triangles, samplesX, samplesY);
}
void GenTriangles::print(int k)
{
    QString str = "plural: ";
    for (int i = 0; i < k;  i++)
    {
        str += QString("%1 | ").arg(temp_triangle[i].x);
        str += QString("%1 | ").arg(temp_triangle[i].y);
    }
    qDebug()<<str;
}
void GenTriangles::genPlural(int count, int pos)
{
    if (count == 0)
    {
        print(num);
        triagnle_struct struct_of_triagnles;
        struct_of_triagnles.A.x = temp_triangle[0].x;
        struct_of_triagnles.A.y = temp_triangle[0].y;
        struct_of_triagnles.B.x = temp_triangle[1].x;
        struct_of_triagnles.B.y = temp_triangle[1].y;
        struct_of_triagnles.C.x = temp_triangle[2].x;
        struct_of_triagnles.C.y = temp_triangle[2].y;
        struct_of_triagnles.count_of_points = 0;

        triangles.append(struct_of_triagnles);
    }
    else
    {
        for (int i = pos; i < mass.size() - count + 1; i ++)
        {
            temp_triangle[num - count].x = mass.at(i).x;
            temp_triangle[num - count].y = mass.at(i).y;

            genPlural(count - 1, i + 1);
        }
    }
}
void GenTriangles::GetCountOfPoints(QVector<triagnle_struct> &triangles, QVector<double> X, QVector<double> Y)
{
    //unsigned int count = 0;
    bool flag = false;
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < X.size(); j++)
        {
            flag = isPointInTriangle(triangles.at(i), X.at(j), Y.at(j));
            if (flag == true)
            {
                triangles[i].count_of_points++;
            }
        }
    qDebug()<<"triangles size: "<<triangles.at(i).count_of_points;
    qDebug()<<"tri: "<<triangles.at(i).A.x << triangles.at(i).A.y << triangles.at(i).B.x << triangles.at(i).B.y << triangles.at(i).C.x << triangles.at(i).C.y;
    }
    TreanglesSort(triangles);
}
void GenTriangles::TreanglesSort(QVector<triagnle_struct> &triangles)
{
    qSort(triangles.begin(), triangles.end(), prefLessThan);
    for (int i=0; i<triangles.size(); i++)
    {
        qDebug() << "   " << triangles[i].count_of_points;
    }
}
bool GenTriangles::isPointInTriangle(triagnle_struct abc, double Px, double Py)
{
    double Bx, By, Cx, Cy, newPx, newPy;
    double_t m, l; // мю и лямбда

    m = 0;
    l = 0;
    Bx = abc.B.x - abc.A.x;
    By = abc.B.y - abc.A.y;
    Cx = abc.C.x - abc.A.x;
    Cy = abc.C.y - abc.A.y;

    newPx = Px  - abc.A.x;
    newPy = Py  - abc.A.y;
  //

    m = ((newPx*By - Bx*newPy) / (Cx*By - Bx*Cy)) ;
    if ((m >= 0) && (m <= 1))
    {
        l = (newPx - m*Cx) / Bx;
        if ((l >= 0) && ((m + l) <= 1))
        {
            qDebug() << "true" << Px << Py;
            return true;
        }
    }
}
QVector <triagnle_struct> GenTriangles::getResult()
{
    return triangles;
}
