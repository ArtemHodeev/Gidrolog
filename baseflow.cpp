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
#include "names.h"
#include "mypicker.h"
#include <cmath>

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

    is_mooved = false;
    addPlotGrid();
    enableMagnifier();
    enableMovingOnPlot();
    enablePicker();
}
void Baseflow::addAll(QVector<bool> legenditems)
{
    QwtLegend *legend = new QwtLegend;
    legend->setDefaultItemMode( QwtLegendData::Checkable ); //Элементы легенды выделяются по клику мыши
    insertLegend( legend, QwtPlot::RightLegend );   //Отображаем легенду справа от графика

    connect( legend, SIGNAL( checked( const QVariant &, bool, int ) ),
        SLOT( showItem( const QVariant &, bool, int ) ) );

    addPlot();
    replot();

    QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve); // все элементы, которые есть на графике (треугольники + 3 набора точек)

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
    showAllBaseflow();
    addBaseflow();
    addPoints();
    if (one->getSizeTriangles() > 0)
    {
        addCurve();
    }
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
        QString str = "";
        curve1 = new QwtPlotCurve();
        str += QString( "( %1 - %2 %3 %4 )")
                 .arg(one->getTriangles().at(i).count_of_points)
                 .arg(
                    Names::water_types->key(one->getTriangles().at(i).A.getWaterId()),
                    Names::water_types->key(one->getTriangles().at(i).B.getWaterId()),
                    Names::water_types->key(one->getTriangles().at(i).C.getWaterId()));

        curve1->setTitle( str );                                                    //Имя элемента легенды
        curve1->setPen( Qt::blue, 2 );                                              // цвет и толщина кривой
        QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
            QBrush( Qt::green ), QPen( Qt::green, 1 ), QSize( 3, 3 ) );            //Форма, цвет и размер точки на кривой
        curve1->setSymbol( symbol );                                                //Привязать символы к кривой

        QVector<double> X, Y;
        X.append(one->getTriangles().at(i).A.getU1());
        X.append(one->getTriangles().at(i).B.getU1());
        X.append(one->getTriangles().at(i).C.getU1());
        X.append(one->getTriangles().at(i).A.getU1());
        Y.append(one->getTriangles().at(i).A.getU2());
        Y.append(one->getTriangles().at(i).B.getU2());
        Y.append(one->getTriangles().at(i).C.getU2());
        Y.append(one->getTriangles().at(i).A.getU2());

        curve1->setSamples(X, Y);                                                   // ассоциировать набор точек с кривой
        curve1->attach( this );                                           // отобразить кривую на графике
    }
}
void Baseflow::addBaseflow()
{
    curve2 = new QwtPlotCurve();
    curve2->setTitle( "Источники питания" );                                        //Имя элемента легенды
    curve2->setPen( Qt::blue, 2 );                                                  // цвет и толщина кривой
    QwtSymbol *symbol1 = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::blue ), QPen( Qt::blue, 2 ), QSize( 3, 3 ) );             //Форма, цвет и размер точки на кривой
    curve2->setSymbol( symbol1 );                                                   //Привязать символы к кривой
    curve2->setSamples(two->getBaseflowX(), two->getBaseflowY());                   // ассоциировать набор точек с кривой
    curve2->setStyle(QwtPlotCurve::Dots);                                           //просто выводит точки
    curve2->attach( this );                                               // отобразить кривую на графике
}
void Baseflow::addPoints()
{
    water_points = new QwtPlotCurve;
    water_points->setPen( Qt::red, 2 );                                             // цвет и толщина кривой
    QString str = Names::water_types->key(Names::analitic_id);
    water_points->setTitle( str );

    QwtSymbol *symbols = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::red ), QPen( Qt::red, 2 ), QSize( 3, 3 ) );
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

    MyPicker *d_picker = new MyPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,                                       // ассоциация с осями
                QwtPlotPicker::CrossRubberBand,                                                     // стиль перпендикулярных линий
                QwtPicker::AlwaysOn,                                                                // всегда включен
                canvas());
    d_picker->setData(one);
    d_picker->setRubberBandPen( QColor( Qt::red ) );                                    // Цвет перпендикулярных линий
    d_picker->setTrackerPen( QColor( Qt::black ) );                                     // цвет координат положения указателя
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );                       // непосредственное включение вышеописанных функций

    // настройка функций
//    QwtPlotPicker *d_picker =
//            new QwtPlotPicker(
//                QwtPlot::xBottom, QwtPlot::yLeft,                                       // ассоциация с осями
//    QwtPlotPicker::CrossRubberBand,                                                     // стиль перпендикулярных линий
//    QwtPicker::AlwaysOn,                                                                // всегда включен
//    canvas() );                                                         // ассоциация с полем
//    d_picker->setRubberBandPen( QColor( Qt::red ) );                                    // Цвет перпендикулярных линий
//    d_picker->setTrackerPen( QColor( Qt::black ) );                                     // цвет координат положения указателя
//    d_picker->setStateMachine( new QwtPickerDragPointMachine() );                       // непосредственное включение вышеописанных функций

    connect( d_picker, SIGNAL( appended( const QPoint & ) ),
            SLOT( click_on_canvas( const QPoint & ) ) );
    connect( d_picker, SIGNAL(moved(const QPoint & ) ),
            SLOT( click_on_canvas( const QPoint & ) ) );

}
void Baseflow::click_on_canvas( const QPoint &pos )
{
    // считываем значения координат клика
    x = invTransform(QwtPlot::xBottom, pos.x());
    y = invTransform(QwtPlot::yLeft, pos.y());
 }
void Baseflow::showItem( const QVariant &itemInfo, bool on, int index )
{
    QwtPlotItem *plotItem = infoToItem( itemInfo );
    if ( plotItem )
    {
        plotItem->setVisible( on );
    }
    updateLegend();
}
void Baseflow::plotDefault()
{
    //one->generateSets(one->getBaseflowX(), one->getBaseflowY());
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
    if (one->getBaseflowSize() >=3  && event->button() == Qt::LeftButton)
    {
//        x = invTransform(QwtPlot::xBottom, event->pos().x());
//        y = invTransform(QwtPlot::yLeft, event->pos().y());
        //qDebug() << "x " << x << "; y " << y;
        if (!is_mooved)
        {
            for (int i=0; i < one->getBaseflowSize(); i++)
            {
                if (
                        (std::abs(one->getBaseflowX().at(i) - x) < 0.03) &&
                        (std::abs(one->getBaseflowY().at(i) - y) < 0.03)
                    )
                {
                    old_xy_position = i;        //Запоминаем старую координату точки
                    is_mooved = true;           //Кликнули по точке источника питания
                }
            }
        }
    }
}
void Baseflow::mouseReleaseEvent(QMouseEvent *event)
{
//    x = invTransform(QwtPlot::xBottom, event->pos().x());
//    y = invTransform(QwtPlot::yLeft, event->pos().y());

//    qDebug() << "x " << x << "; y " << y;
    if (is_mooved)  //Если по точке источника питания кликнули
    {
        QVector<SampleInfo> temp_mass_x, temp_mass_y;

        temp_mass_x = one->getInfoBaseflowX();
        temp_mass_y = one->getInfoBaseflowY();

        //Меняем координаты угла треугольника, по которому кликнули
        QVector<triagnle_struct> size_tri = one->getTriangles();
        for (int i = 0; i<size_tri.size(); i++)
        {
            if ((std::abs(one->getTriangles().at(i).A.getU1() - temp_mass_x[old_xy_position].getU1()) < 0.03) && (std::abs(one->getTriangles().at(i).A.getU2() - temp_mass_y[old_xy_position].getU2()) < 0.03))
            {
                size_tri[i].A.setU1(x); size_tri[i].A.setU2(y);
            }
            if ((std::abs(one->getTriangles().at(i).B.getU1() - temp_mass_x[old_xy_position].getU1()) < 0.03) && (std::abs(one->getTriangles().at(i).B.getU2() - temp_mass_y[old_xy_position].getU2()) < 0.03))
            {
                size_tri[i].B.setU1(x); size_tri[i].B.setU2(y);
            }
            if ((std::abs(one->getTriangles().at(i).C.getU1() - temp_mass_x[old_xy_position].getU1()) < 0.03) && (std::abs(one->getTriangles().at(i).C.getU2() - temp_mass_y[old_xy_position].getU2()) < 0.03))
            {
               size_tri[i].C.setU1(x); size_tri[i].C.setU2(y);
            }
        }
        one->setTriangles(size_tri);
        temp_mass_x[old_xy_position].setU1(x);
        temp_mass_y[old_xy_position].setU2(y);

        one->setBaseflowX(temp_mass_x);
        one->setBaseflowY(temp_mass_y);

//        //Перед перерисовкой запомним, какие элементы легенды были нажаты
        updateLegend();
        detachItems(QwtPlotItem::Rtti_PlotItem, true);  //Очистка рабочей области
        addAll(legenditems);                            //Перерисовка графика
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
void Baseflow::setData(AllTriangles data)
{
    one->setBaseflowX(data.getInfoBaseflowX());
    one->setBaseflowY(data.getInfoBaseflowX());
    one->setSamplesX(data.getInfoSamplesX());
    one->setSamplesY(data.getInfoSamplesY());
    one->setTriangles(data.getTriangles());
    two = one;

    int legenditems_size = one->getSizeTriangles() + 3; //число элементов легенды
    //говорим, какие из элементов будут отображаться, а какие нет
    legenditems.resize(legenditems_size);
    legenditems.fill(false);
    legenditems[0] = false;
    legenditems[1] = true;
    legenditems[2] = true;
    legenditems[3] = true;
    addAll(legenditems);
}
void Baseflow::showAllBaseflow()
{
    baseflows = new QwtPlotCurve;
    baseflows->setPen( Qt::black, 2 );                                             // цвет и толщина кривой
    baseflows->setTitle("Все источники питания");

    QwtSymbol *symbols = new QwtSymbol( QwtSymbol::Ellipse,
        QBrush( Qt::black ), QPen( Qt::black, 1 ), QSize( 1, 1 ) );
    baseflows->setSymbol( symbols );
    QVector <double> X, Y;
    for (int i=0; i<allBaseflow.size(); i++)
    {
        X.append(allBaseflow.at(i).getU1());
        Y.append(allBaseflow.at(i).getU2());
    }
    baseflows->setSamples(X, Y);
    baseflows->setStyle(QwtPlotCurve::Dots);                                     //просто выводит точки
    baseflows->attach( this );
}
void Baseflow::addAllBaseflow(QVector <SampleInfo> allBaseflow)
{
    this->allBaseflow = allBaseflow;
}
void Baseflow::updateLegend()
{
    QwtPlotItemList items = itemList( QwtPlotItem::Rtti_PlotCurve);
    for (int i=0; i<items.size(); i++)
    {
        if (items[i]->isVisible())
            legenditems[i] = true;
        else
            legenditems[i] = false;
    }
}
