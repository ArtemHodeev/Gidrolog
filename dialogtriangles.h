#ifndef DIALOGTRIANGLES_H
#define DIALOGTRIANGLES_H

#include <QMainWindow>
#include <baseflow.h>
#include <QMouseEvent>

namespace Ui {
class DialogTriangles;
}

class DialogTriangles : public QMainWindow
{
    Q_OBJECT

public:
    explicit DialogTriangles(QWidget *parent = 0);
    ~DialogTriangles();
private:
    Ui::DialogTriangles *ui;
    Baseflow *plot;
    void mouseMoveEvent(QMouseEvent *event);      //Отображение координаты точки при перемещении мыши по графику
};

#endif // DIALOGTRIANGLES_H
