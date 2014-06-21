#ifndef DIALOGTRIANGLES_H
#define DIALOGTRIANGLES_H

#include <QMainWindow>
#include <baseflow.h>
#include <QMouseEvent>
#include <solverpca.h>
#include <QListWidget>
#include <QGridLayout>

namespace Ui {
class DialogTriangles;
}

class DialogTriangles : public QMainWindow
{
    Q_OBJECT

public:
    explicit DialogTriangles(QWidget *parent = 0);
    ~DialogTriangles();
    void setSamples(QVector<Sample*> s);
    void setParam(int count, QVector<unsigned int> cust_items);
    void setPlurList();
private:
    Ui::DialogTriangles *ui;
    Baseflow *plot;
    bool askOnClose();
    int count;
    SolverPCA solve;
    QVector<unsigned int*> plurals; //адреса удачных множеств
    QBoxLayout *layout_triangles;
    QListWidget *itemslist;

private slots:
    void on_listWidget_clicked(const QModelIndex &index);
protected:
//    virtual void mouseMoveEvent(QMouseEvent *event);      //Отображение координаты точки при перемещении мыши по графику
    virtual void closeEvent(QCloseEvent *event);         //Реакция на нажатие кнопки "Закрыть"

};

#endif // DIALOGTRIANGLES_H
