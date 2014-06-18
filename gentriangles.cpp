#include "gentriangles.h"
#include <QDebug>
#include <math.h>
#include <alltriangles.h>


bool prefLessThan(const triagnle_struct &p1, const triagnle_struct &p2)
{
    return p1.count_of_points > p2.count_of_points;
}

GenTriangles::GenTriangles(QVector <SampleInfo> &massive_pair, QVector <SampleInfo> &sX, QVector <SampleInfo> &sY)
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
    temp_triangle = new SampleInfo[count];
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
        str += QString("%1 | ").arg(temp_triangle[i].getU1());
        str += QString("%1 | ").arg(temp_triangle[i].getU2());
    }
    qDebug()<<str;
}
void GenTriangles::genPlural(int count, int pos)
{
    if (count == 0)
    {
        print(num);
        triagnle_struct struct_of_triagnles;
        struct_of_triagnles.A.setU1(temp_triangle[0].getU1());
        struct_of_triagnles.A.setU2(temp_triangle[0].getU2());
        struct_of_triagnles.B.setU1(temp_triangle[1].getU1());
        struct_of_triagnles.B.setU2(temp_triangle[1].getU2());
        struct_of_triagnles.C.setU1(temp_triangle[2].getU1());
        struct_of_triagnles.C.setU2(temp_triangle[2].getU2());
        struct_of_triagnles.count_of_points = 0;

        triangles.append(struct_of_triagnles);
    }
    else
    {
        for (int i = pos; i < mass.size() - count + 1; i ++)
        {
            temp_triangle[num - count].setU1(mass.at(i).getU1());
            temp_triangle[num - count].setU2(mass.at(i).getU2());

            genPlural(count - 1, i + 1);
        }
    }
}
void GenTriangles::GetCountOfPoints(QVector<triagnle_struct> &triangles, QVector<SampleInfo> X, QVector<SampleInfo> Y)
{
    //unsigned int count = 0;
    bool flag = false;
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int j = 0; j < X.size(); j++)
        {
            flag = isPointInTriangle(triangles.at(i), X.at(j).getU1(), Y.at(j).getU2());
            if (flag == true)
            {
                triangles[i].count_of_points++;
                qDebug() << "точка (" << X.at(j).getU1() << "; " << Y.at(j).getU2() << ")  в треугольнике  " << triangles.at(i).A.getU1() << triangles.at(i).A.getU2() << triangles.at(i).B.getU1() << triangles.at(i).B.getU2() << triangles.at(i).C.getU1() << triangles.at(i).C.getU2();
            }
        }
    qDebug()<<"triangles size: "<<triangles.at(i).count_of_points;
    qDebug()<<"tri: "<<triangles.at(i).A.getU1() << triangles.at(i).A.getU2() << triangles.at(i).B.getU1() << triangles.at(i).B.getU2() << triangles.at(i).C.getU1() << triangles.at(i).C.getU2();
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
    Bx = abc.B.getU1() - abc.A.getU1();
    By = abc.B.getU2() - abc.A.getU2();
    Cx = abc.C.getU1() - abc.A.getU1();
    Cy = abc.C.getU2() - abc.A.getU2();

    newPx = Px  - abc.A.getU1();
    newPy = Py  - abc.A.getU2();
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
    return false;
}
QVector <triagnle_struct> GenTriangles::getResult()
{
    return triangles;
}
