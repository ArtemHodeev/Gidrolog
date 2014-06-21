#include "mypicker.h"
#include "qwt_plot.h"
#include "qwt_scale_div.h"
#include "qwt_painter.h"
#include "qwt_scale_map.h"
#include "qwt_picker_machine.h"
#include "alltriangles.h"
#include "QMouseEvent"

MyPicker::MyPicker(int xAxis, int yAxis,
        RubberBand rubberBand, DisplayMode trackerMode,
        QWidget *canvas):
    QwtPlotPicker( xAxis, yAxis, rubberBand, trackerMode, canvas )
{
    this->tri = tri;
}
MyPicker::~MyPicker()
{
}
void MyPicker::setData(AllTriangles *tri)
{
    this->tri = tri;
}
QwtText MyPicker::trackerText( const QPoint &pos ) const
{
    return trackerTextF( invTransform( pos ) );
}
QwtText MyPicker::trackerTextF( const QPointF &pos ) const
{
    QString text;
    double x = pos.x();
    double y = pos.y();
    QPoint area;
    area.setX(5);
    area.setY(5);

    QString str = tri->is_baseflow_point(x, y);
    if (str == "")
        str = tri->is_analytic_point(x,y);
    if  (str == "")
    {
        switch ( rubberBand() )
        {
            case HLineRubberBand:
                text.sprintf( "%.4f", y);
                break;
            case VLineRubberBand:
                text.sprintf( "%.4f", x );
                break;
            default:
                text.sprintf( "( %.4f, %.4f )", x, y);
        }
    }
    text += str;
    return QwtText( text );
}
