#ifndef TESTSOLVE_H
#define TESTSOLVE_H
#include <solverpca.h>

#include <QDialog>

namespace Ui {
class TestSolve;
}

class TestSolve : public QDialog
{
    Q_OBJECT

public:
    explicit TestSolve(QWidget *parent = 0);
    ~TestSolve();
    void setSamples(QVector<Sample*> s);
    void setParam(int count, QVector<unsigned int> cust_items);
    void setPlurList();
//    void setInfo();


private slots:
    void on_listWidget_clicked(const QModelIndex &index);

private:
    int count;
    Ui::TestSolve *ui;
    SolverPCA solve;
    QVector<unsigned int*> plurals;
};

#endif // TESTSOLVE_H
