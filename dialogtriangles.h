#ifndef DIALOGTRIANGLES_H
#define DIALOGTRIANGLES_H

#include <QMainWindow>
#include <baseflow.h>

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
};

#endif // DIALOGTRIANGLES_H
