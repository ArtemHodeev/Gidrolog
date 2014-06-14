#include "baseflow.h"
#include "alltriangles.h"
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <QVector>
#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <qwt_plot_grid.h>
#include <qwt_point_mapper.h>
#include <qwt_plot_picker.h>
#include <QMouseEvent>
#include <QLabel>
#include <qwt_plot_marker.h>
#include <QMessageBox>
#include "dialogtriangles.h"

Baseflow::Baseflow(QWidget *parent ):
    QwtPlot( parent )
{
    //Добавление канвы
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setPalette( Qt::gray );
    canvas->setBorderRadius( 10 );
    setCanvas( canvas );
    plotLayout()->setAlignCanvasToScales( true );

   // setTitle( "Диаграммы смешения" );
    setCanvasBackground( Qt::white );

    // Параметры осей координат
    setAxisTitle(QwtPlot::yLeft, "U1");
    setAxisTitle(QwtPlot::xBottom, "U2");

    one = new AllTriangles();
    two = new AllTriangles();

    int legenditems_size = one->getSizeTriangles() + 2; //число элементов легенды
    //говорим, какие из элементов будут отображаться, а какие нет
    legenditems.resize(legenditems_size);
    legenditems.fill(false);
    legenditems[0] = true;
    legenditems[legenditems_size-1] = true;
    legenditems[legenditems_size-2] = true;

    addAll(legenditems);
}
void Baseflow::addAll(QVector<bool> legenditems)
{
    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode( QwtLegendData::Checkable ); //Элементы легенды выделяются по клику мыши
    insertLegend( legend, QwtPlot::RightLegend );   //Отображаем легенду справа от графика

    connect( legend, SIGNAL( checked( const QVariant &, bool, int ) ),
        SLOT( showItem( const QVariant &, bool ) ) );

    addPlot();
    replot();

    QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve); // все элементы, которые есть на графике (треугольники + 2 набора точек)

    for ( int i = 0; i < items.size(); i++ )
    {
        if (legenditems.at(i))  //Те элементы, которые в legenditems - true, те отображаются на графике, остальные - нет.
        {
            const QVariant itemInfo = itemToInfo( items[i] );

            QwtLegendLabel *legendLabel =
                qobject_cast<QwtLegendLabel *>( legend->legendWidget( itemInfo ) );
            if ( legendLabel )
                legendLabel->setChecked( true );

            items[i]->setVisible( true );
        }
        else
        {
            items[i]->setVisible( false );
        }
    }
    setAutoReplot( true );
}
Baseflow::~Baseflow()
{

}
void Baseflow::addPlot()
{
    is_mooved = false;
    addCurve();
    addBaseflow();
    addPoints();
    addPlotGrid();
    enableMagnifier();
    enableMovingOnPlot();
    enablePicker();
    replot();
}
void Baseflow::addPlotGrid()
{
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(QPen( Qt::gray, 2 ));                                         // цвет линий и толщина
    grid->attach( this );
}
void Baseflow::addCurve()
{
    for (int i = 0; i<one->getSizeTriangles(); i++)
    {
        curve1 = new QwtPlotCurve();
        curve1->setTitle( "Дождь, Дождь2, Подземные" );                             //Имя элемента легенды
        curve1->setPen( Qt::blue, 6 );                                              // цвет и толщина кривой
        QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
            QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 11, 11 ) );            //Форма, цвет и размер точки на кривой
        curve1->setSymbol( symbol );                                                //Привязать символы к кривой

        QVector<double> X, Y;
        X.append(one->getTriangles().at(i).A.x);
        X.append(one->getTriangles().at(i).B.x);
        X.append(one->getTriangles().at(i).C.x);
        X.append(one->getTriangles().at(i).A.x);
        Y.append(one->getTriangles().at(i).A.y);
        Y.append(one->getTriangles().at(i).B.y);
        Y.append(one->getTriangles().at(i).C.y);
        Y.append(one->getTriangles().at(i).A.y);

        curve1->setSamples(X, Y);                                                   // ассоциировать набор точек с кривой
        curve1->attach( this );                                           // отобразить кривую на графике
    }
}
void Baseflow::addBaseflow()
{
    curve2 = new QwtPlotCurve();
    curve2->setTitle( "Источники питания" );                                        //Имя элемента легенды
    curve2->setPen( Qt::blue, 6 );                                                  // цвет и толщина кривой
    QwtSymbol *symbol1 = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::blue ), QPen( Qt::darkBlue, 2 ), QSize( 13, 13 ) );             //Форма, цвет и размер точки на кривой
    curve2->setSymbol( symbol1 );                                                   //Привязать символы к кривой
    curve2->setSamples(two->getBaseflowX(), two->getBaseflowY());                   // ассоциировать набор точек с кривой
    curve2->setStyle(QwtPlotCurve::Dots);                                           //просто выводит точки
    curve2->attach( this );                                               // отобразить кривую на графике
}
void Baseflow::addPoints()
{
    water_points = new QwtPlotCurve;
    water_points->setPen( Qt::red, 6 );                                             // цвет и толщина кривой
    water_points->setTitle( "Речные воды" );

    QwtSymbol *symbols = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::red ), QPen( Qt::black, 2 ), QSize( 6, 6 ) );
    water_points->setSymbol( symbols );
    water_points->setSamples(one->getSamplesX(), one->getSamplesY());
    water_points->setStyle(QwtPlotCurve::Dots);                                     //просто выводит точки
    water_points->attach( this );                                         // отобразить кривую на графике
}
void Baseflow::enableMagnifier()
{
    QwtPlotMagnifier *magnifier =
            new QwtPlotMagnifier(canvas());
    // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);
}
void Baseflow::enableMovingOnPlot()
{
    QwtPlotPanner *d_panner = new QwtPlotPanner( canvas() );
    // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::RightButton );
}
void Baseflow::enablePicker()
{
    // настройка функций
    QwtPlotPicker *d_picker =
            new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,                                       // ассоциация с осями
    QwtPlotPicker::CrossRubberBand,                                                     // стиль перпендикулярных линий
    QwtPicker::AlwaysOn,                                                                // всегда включен
    canvas() );                                                         // ассоциация с полем
    d_picker->setRubberBandPen( QColor( Qt::red ) );                                    // Цвет перпендикулярных линий
    d_picker->setTrackerPen( QColor( Qt::black ) );                                     // цвет координат положения указателя
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );                       // непосредственное включение вышеописанных функций

    connect( d_picker, SIGNAL( appended( const QPoint & ) ),
            SLOT( click_on_canvas( const QPoint & ) ) );

}
void Baseflow::click_on_canvas( const QPoint &pos )
{
    // считываем значения координат клика
    x = invTransform(QwtPlot::xBottom, pos.x());
    y = invTransform(QwtPlot::yLeft, pos.y());
}
void Baseflow::showItem( const QVariant &itemInfo, bool on )
{
    QwtPlotItem *plotItem = infoToItem( itemInfo );
    if ( plotItem )
    {
        plotItem->setVisible( on );
    }
}
void Baseflow::plotDefault()
{
    one = new AllTriangles();
    two = new AllTriangles();
    detachItems(QwtPlotItem::Rtti_PlotItem, true);
    addAll(legenditems);
}
void Baseflow::exportPlot()
{
    QwtPlotRenderer renderer;
    renderer.exportTo( this, "Диаграмма смешения.jpg" );
}
void Baseflow::mousePressEvent(QMouseEvent *event)
{
    x = invTransform(QwtPlot::xBottom, event->pos().x()) - 1.58702; //Без этих страшных циферок неправильно считывается позиция клика
    y = invTransform(QwtPlot::yLeft, event->pos().y()) + 0.9534;
//    qDebug() << "x " << x << "; y " << y;
    if (!is_mooved)
    {
        for (int i=0; i < one->getBaseflowX().size(); i++)
        {
            if (((double) abs(one->getBaseflowX().at(i) - x) < 0.0150) && ((double) abs(one->getBaseflowY().at(i) - y) < 0.0150))
            {
                old_xy_position = i;        //Запоминаем старую координату точки
                is_mooved = true;           //Кликнули по точке источника питания
            }
        }
    }
}

void Baseflow::mouseReleaseEvent(QMouseEvent *event)
{
    x = invTransform(QwtPlot::xBottom, event->pos().x()) - 1.58702;
    y = invTransform(QwtPlot::yLeft, event->pos().y()) + 0.9534;

//    qDebug() << "x " << x << "; y " << y;
    if (is_mooved)  //Если по точке источника питания кликнули
    {
        QVector<double> temp_mass_x, temp_mass_y;

        temp_mass_x = one->getBaseflowX();
        temp_mass_y = one->getBaseflowY();

        //Меняем координаты угла треугольника, по которому кликнули
        QVector<triagnle_struct> size_tri = one->getTriangles();
        for (int i = 0; i<size_tri.size(); i++)
        {
            if (((double) abs(one->getTriangles().at(i).A.x - temp_mass_x[old_xy_position]) < 0.0150) && ((double) abs(one->getTriangles().at(i).A.y - temp_mass_y[old_xy_position]) < 0.0150))
            {
                size_tri[i].A.x = x; size_tri[i].A.y = y;
            }
            if (((double) abs(one->getTriangles().at(i).B.x - temp_mass_x[old_xy_position]) < 0.0150) && ((double) abs(one->getTriangles().at(i).B.y - temp_mass_y[old_xy_position]) < 0.0150))
            {
                size_tri[i].B.x = x; size_tri[i].B.y = y;
            }
            if (((double) abs(one->getTriangles().at(i).C.x - temp_mass_x[old_xy_position]) < 0.0150) && ((double) abs(one->getTriangles().at(i).C.y - temp_mass_y[old_xy_position]) < 0.0150))
            {
                size_tri[i].C.x = x; size_tri[i].C.y = y;
            }
        }
        one->setTriangles(size_tri);
        temp_mass_x[old_xy_position] = x;
        temp_mass_y[old_xy_position] = y;

        one->setBaseflowX(temp_mass_x);
        one->setBaseflowY(temp_mass_y);

        //Перед перерисовкой запомним, какие элементы легенды были нажаты
        QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve);
        for (int i=0; i<items.size(); i++)
        {
            if (items[i]->isVisible())
                legenditems[i] = true;
            else
                legenditems[i] = false;
        }
        detachItems(QwtPlotItem::Rtti_PlotItem, true);  // Очистка рабочей области
        addAll(legenditems);                                            //Перерисовка графика
        is_mooved = false;
    }
}
void Baseflow::makeAnaliz()
{
    QMessageBox::about(
              this, tr("Ох, нет!"),
              tr("<h2>Просим прощения,</h2>"
                 "<p>но этот метод еще не реализован."));
}
