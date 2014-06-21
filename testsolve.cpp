#include "testsolve.h"
#include "ui_testsolve.h"
#include <names.h>
#include <QHash>
#include "alltriangles.h"
#include <QDebug>

TestSolve::TestSolve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestSolve)
{
    ui->setupUi(this);
}

TestSolve::~TestSolve()
{
    delete ui;
//    plurals.clear();
//    solve.~SolverPCA();
}
void TestSolve::setSamples(QVector<Sample *> s)
{
    solve.setItems(s);
    solve.standart();
}
void TestSolve::setParam(int count, QVector<unsigned int> cust_items)
{
    QHash<QString, unsigned int>::iterator iter;
    QVector<unsigned int> mass;
    QHash<unsigned int*, double**>::iterator plur_iter;
    for (iter = Names::params->begin(); iter != Names::params->end(); iter ++)
    {
        mass.append(iter.value());
    }
    this->count = count;
    solve.makePlurals(count,cust_items, mass);
    for (plur_iter = solve.complete_plurals.begin(); plur_iter != solve.complete_plurals.end(); plur_iter ++)
    {
        plurals.append(plur_iter.key());
    }
}
void TestSolve::setPlurList()
{
    QString str = "";
    QStringList list;

    for (int i = 0; i < plurals.size(); i ++)
    {
        str = "< ";
        for (int j = 0; j < count; j ++)
        {
            str += QString("%1 ").arg(Names::params->key(plurals[i][j]));
        }
        str += ">";
        list.append(str);

    }
    ui->listWidget->addItems(list);
}
void TestSolve::on_listWidget_clicked(const QModelIndex &index)
{
    QHash<QString, unsigned int>::iterator iter;
    QVector<SampleInfo> si;
    QVector<SampleInfo> bi;
    QString str = "";
    AllTriangles one;
    ui->textEdit->setText(str);
    for (iter = Names::water_types->begin(); iter != Names::water_types->end(); iter++)
    {
        double ave_u1 = 0;
        double ave_u2 = 0;
        si = solve.getSamplesInfo(plurals[index.row()],iter.value());
        str += QString("\nИсточник: %1\n").arg(iter.key());
        for (int i = 0; i < si.size(); i ++)
        {
            str += QString("Дата: %1; Место: %2; Тип водной массы: %3; U1: %4  U2: %5\n")
                    .arg(
                        si[i].getDate().toString(),
                        Names::locations->key(si[i].getLocationId()),
                        Names::water_types->key(si[i].getWaterId()))
                    .arg(si[i].getU1())
                    .arg(si[i].getU2());
            ave_u1 += si[i].getU1();    //высчитали среднее для U1
            ave_u2 += si[i].getU2();    //высчитали среднее для U2
         }
        if (iter.value() == Names::analitic_id)
        {
            one.setSamplesX(si);
            one.setSamplesY(si);
        }
        else
        {
            SampleInfo temp_info;
            if (ave_u1 != 0 || ave_u2 != 0)
            {
                temp_info.setU1(ave_u1/si.size());
                temp_info.setU2(ave_u2/si.size());
                qDebug() << "Среднее для U1: " << ave_u1 << "для " << iter.key();
                qDebug() << "Среднее для U2: " << ave_u2;
                temp_info.setWaterId(iter.value());
                bi.append(temp_info);
            }
        }

        //Для каждого Names::water_types != analitic_id считаем среднее для И1 и И2.
        //Передали векторы в Alltiangles

    }
    one.setBaseflowX(bi);
    one.setBaseflowY(bi);
    ui->textEdit->setText(str);


}
