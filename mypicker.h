#ifndef MYPICKER_H
#define MYPICKER_H

#include "qwt_global.h"
#include "qwt_plot_picker.h"
#include <qvector.h>
#include "alltriangles.h"

class QwtPlot;

class MyPicker: public QwtPlotPicker
{
    Q_OBJECT

public:
    //explicit MyPicker( QWidget *canvas );
    virtual ~MyPicker();

    //explicit MyPicker( int xAxis, int yAxis, QWidget * );

    explicit MyPicker(int xAxis, int yAxis,
        RubberBand rubberBand, DisplayMode trackerMode, QWidget * );
    void setData(AllTriangles *tri);

Q_SIGNALS:


protected:

     QwtText trackerText( const QPoint & ) const;
     QwtText trackerTextF( const QPointF & ) const;



private:
    int d_xAxis;
    int d_yAxis;
    AllTriangles *tri;

};

#endif
