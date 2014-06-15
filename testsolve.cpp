#include "testsolve.h"
#include "ui_testsolve.h"
#include <names.h>
#include <QHash>

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
    QString str = "";
    ui->textEdit->setText(str);
    for (iter = Names::water_types->begin(); iter != Names::water_types->end(); iter++)
    {
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
        }

    }
    ui->textEdit->setText(str);

}
