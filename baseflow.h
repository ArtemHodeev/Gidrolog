#ifndef BASEFLOW_H
#define BASEFLOW_H
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_point_mapper.h>
#include <QMainWindow>
#include <QBitArray>
#include "alltriangles.h"
#include <QLabel>
#include <sampleinfo.h>


class Baseflow: public QwtPlot
{
    Q_OBJECT
public:
    Baseflow(QWidget * = NULL );
    ~Baseflow();
    void setData(AllTriangles data);
    void addPoints();               //Добавляем пробы анализируемого типа на график
    void addBaseflow();             //Добавить точки - источники питания
    void addCurve();                //Добавить треугольник
    void addAllBaseflow(QVector <SampleInfo> allBaseflow);
private Q_SLOTS:
    void click_on_canvas( const QPoint &pos );      //Считывание координат при клике мыши
public Q_SLOTS:
    void makeAnaliz();                              //Решение уравнения смешения (не реализовано)
    void exportPlot();                              //Экспорт графика в .jpg
    void showItem(const QVariant &, bool on , int index);     //Скрыть/показать точки/кривые на графике
    void plotDefault();                             //Отобразить исходные треугольники (до перемещения их пользователем)
    void showAllBaseflow();         //Показать все источники питания

private:
    double x, y;                    //Координаты
    bool is_mooved;                 //Флаг, показывающий, была ли выделена мышью точка источников питания
    int old_xy_position;            //Старая позиция точки источника питания
    QVector <bool> legenditems;     //Вектор, в котором хранится инф-я, какой элемент легенды выделен
    QVector <SampleInfo> allBaseflow;//Вектор всех источников питания
    void addAll(QVector <bool> legenditems);    //Добавляем легенду на график, отображаем выделенные элементы легенды
    void addPlot();
    void addPlotGrid();             //Добавляем сетку
    QwtLegend *legend;              //Легенда
    QwtPlotCurve *curve1;           //Треугольник
    QwtPlotCurve *curve2;           //Пробы источников
    QwtPlotCurve *baseflows;        //Все источники питания
    QwtPlotCurve *water_points;     //Пробы анализируемого типа
    QPolygonF points1;              //
    QPolygonF points2;              // Точки, которые накладываются на кривые curve1, curve2 и water_points.
    QPolygonF points3;              //
    AllTriangles *one, *two;        //Классы, хранящие координаты треугольников
    void enableMagnifier();         //Масштабирование графика
    void enableMovingOnPlot();
    void enablePicker();            //Отображение координаты
    void updateLegend();

protected:
  void mousePressEvent(QMouseEvent *event);     //Перемещение угла треугольника
  void mouseReleaseEvent(QMouseEvent *event);
};

#endif // BASEFLOW_H
